#!/usr/bin/env python3
"""Create, verify, and restore explicit migration file bundles."""

import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import stat
import sys
import tarfile
import tempfile


MANIFEST_NAME = "manifest.json"
MAX_FILE_SIZE = 50 * 1024 * 1024
MAX_ARCHIVE_SIZE = 100_000_000
CHUNK_SIZE = 1024 * 1024
FORBIDDEN_EXTENSIONS = {".z64", ".n64", ".v64", ".o", ".bin", ".text"}
FORBIDDEN_PREFIXES = ("assets/", "expected/", "build/", ".git/", ".omp/")
ARCHIVE_SIGNATURES = (
    b"\x7fELF", b"PK\x03\x04", b"PK\x05\x06", b"PK\x07\x08",
    b"\x1f\x8b", b"BZh", b"\xfd7zXZ\x00", b"7z\xbc\xaf\x27\x1c",
    b"Rar!\x1a\x07", b"\x28\xb5\x2f\xfd", b"!<arch>\n",
)


class ArchiveError(Exception):
    pass


def validate_path(value):
    if not isinstance(value, str) or not value or "\\" in value:
        raise ArchiveError("manifest paths must be non-empty POSIX relative paths")
    path = PurePosixPath(value)
    if path.is_absolute() or any(part in ("", ".", "..") for part in value.split("/")):
        raise ArchiveError("unsafe path: {}".format(value))
    if path.as_posix() != value:
        raise ArchiveError("non-canonical path: {}".format(value))
    if value == MANIFEST_NAME:
        raise ArchiveError("reserved manifest path: {}".format(value))
    if value.startswith(FORBIDDEN_PREFIXES) or value in (".git", ".omp", "AGENTS.md"):
        raise ArchiveError("restricted path: {}".format(value))
    if any(part in (".git", ".omp", "AGENTS.md") for part in path.parts):
        raise ArchiveError("restricted path: {}".format(value))
    if path.suffix.lower() in FORBIDDEN_EXTENSIONS:
        raise ArchiveError("restricted file extension: {}".format(value))
    return value


def validate_path_set(paths):
    path_set = set(paths)
    for value in paths:
        parts = PurePosixPath(value).parts
        for end in range(1, len(parts)):
            if PurePosixPath(*parts[:end]).as_posix() in path_set:
                raise ArchiveError("file path is an ancestor of another file: {}".format(value))
    return paths


def hash_source(path):
    digest = hashlib.sha256()
    size = 0
    first = b""
    with path.open("rb") as source:
        while True:
            chunk = source.read(CHUNK_SIZE)
            if not chunk:
                break
            if b"\0" in chunk:
                raise ArchiveError("binary NUL byte in {}".format(path))
            if len(first) < 512:
                first += chunk[:512 - len(first)]
            size += len(chunk)
            if size > MAX_FILE_SIZE:
                raise ArchiveError("file exceeds 50 MiB limit: {}".format(path))
            digest.update(chunk)
    if first.startswith(ARCHIVE_SIGNATURES) or (len(first) >= 262 and first[257:262] == b"ustar"):
        raise ArchiveError("binary/archive signature in {}".format(path))
    return size, digest.hexdigest()


def source_file(root, relative):
    path = root.joinpath(*PurePosixPath(relative).parts)
    current = root
    for part in PurePosixPath(relative).parts:
        current = current / part
        try:
            info = current.lstat()
        except OSError as exc:
            raise ArchiveError("cannot access {}: {}".format(current, exc))
        if stat.S_ISLNK(info.st_mode):
            raise ArchiveError("symlink source path: {}".format(current))
    if not stat.S_ISREG(info.st_mode):
        raise ArchiveError("not a regular file: {}".format(path))
    return path, info


def read_file_list(path):
    try:
        values = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise ArchiveError("cannot read file list: {}".format(exc))
    if not isinstance(values, list):
        raise ArchiveError("file list must be a JSON array")
    files = [validate_path(value) for value in values]
    if len(set(files)) != len(files):
        raise ArchiveError("duplicate paths in file list")
    validate_path_set(files)
    return files


def write_manifest(tar, entries):
    data = json.dumps({"version": 1, "files": entries}, sort_keys=True, separators=(",", ":")).encode("utf-8")
    info = tarfile.TarInfo(MANIFEST_NAME)
    info.size = len(data)
    info.mode = 0o644
    info.mtime = 0
    import io
    tar.addfile(info, io.BytesIO(data))


class CheckedReader:
    def __init__(self, source, expected):
        self.source = source
        self.expected = expected
        self.size = 0
        self.digest = hashlib.sha256()

    def read(self, size=-1):
        data = self.source.read(size)
        self.size += len(data)
        self.digest.update(data)
        return data

    def check(self, path):
        if self.size != self.expected["size"] or self.digest.hexdigest() != self.expected["sha256"]:
            raise ArchiveError("source changed while archiving: {}".format(path))


def create_archive(root, file_list, output):
    root = root.resolve(strict=True)
    if not root.is_dir():
        raise ArchiveError("root is not a directory")
    files = read_file_list(file_list)
    entries = []
    sources = []
    for relative in files:
        path, info = source_file(root, relative)
        size, digest = hash_source(path)
        mode = stat.S_IMODE(info.st_mode)
        if mode > 0o777:
            raise ArchiveError("special mode bits are not allowed: {}".format(path))
        entries.append({"path": relative, "sha256": digest, "size": size, "mode": mode})
        sources.append((relative, path))

    output = output.absolute()
    if any(path.resolve() == output.resolve() for _, path in sources):
        raise ArchiveError("output must not replace a selected source file")
    output.parent.mkdir(parents=True, exist_ok=True)
    fd, temp_name = tempfile.mkstemp(prefix="." + output.name + ".", suffix=".tmp", dir=str(output.parent))
    os.close(fd)
    try:
        with open(temp_name, "wb") as raw:
            with tarfile.open(fileobj=raw, mode="w:gz") as tar:
                write_manifest(tar, entries)
                for item, (relative, path) in zip(entries, sources):
                    info = tarfile.TarInfo(relative)
                    info.size = item["size"]
                    info.mode = item["mode"]
                    info.mtime = 0
                    info.type = tarfile.REGTYPE
                    with path.open("rb") as source:
                        checked = CheckedReader(source, item)
                        tar.addfile(info, checked)
                        checked.check(path)
        if os.path.getsize(temp_name) >= MAX_ARCHIVE_SIZE:
            raise ArchiveError("compressed archive must be smaller than 100 MB")
        os.replace(temp_name, output)
    finally:
        try:
            os.unlink(temp_name)
        except FileNotFoundError:
            pass
    total = sum(item["size"] for item in entries)
    print("Created {} files ({} bytes): {}".format(len(entries), total, output))


def load_archive(archive):
    if os.path.getsize(archive) >= MAX_ARCHIVE_SIZE:
        raise ArchiveError("archive must be smaller than 100 MiB")
    try:
        tar = tarfile.open(archive, mode="r:gz")
    except (OSError, tarfile.TarError) as exc:
        raise ArchiveError("cannot open gzip tar archive: {}".format(exc))
    return tar


def archive_manifest(tar):
    members = tar.getmembers()
    if not members or members[0].name != MANIFEST_NAME:
        raise ArchiveError("archive must begin with its manifest")
    if any(not member.isfile() or member.issym() or member.islnk() or member.sparse for member in members):
        raise ArchiveError("archive may contain regular files only")
    names = [member.name for member in members]
    if len(set(names)) != len(names):
        raise ArchiveError("duplicate archive member")
    manifest_member = members[0]
    if manifest_member.size > MAX_FILE_SIZE:
        raise ArchiveError("manifest is too large")
    stream = tar.extractfile(manifest_member)
    try:
        data = stream.read(MAX_FILE_SIZE + 1)
    finally:
        stream.close()
    if len(data) > MAX_FILE_SIZE:
        raise ArchiveError("manifest is too large")
    try:
        manifest = json.loads(data.decode("utf-8"))
    except (UnicodeError, json.JSONDecodeError) as exc:
        raise ArchiveError("invalid archive manifest: {}".format(exc))
    if not isinstance(manifest, dict) or manifest.get("version") != 1 or not isinstance(manifest.get("files"), list):
        raise ArchiveError("unsupported or malformed archive manifest")
    expected = {}
    for entry in manifest["files"]:
        if not isinstance(entry, dict) or set(entry) != {"path", "sha256", "size", "mode"}:
            raise ArchiveError("malformed manifest entry")
        path = validate_path(entry["path"])
        size, mode, digest = entry["size"], entry["mode"], entry["sha256"]
        if type(size) is not int or size < 0 or size > MAX_FILE_SIZE:
            raise ArchiveError("invalid size for {}".format(path))
        if type(mode) is not int or mode < 0 or mode > 0o777:
            raise ArchiveError("invalid mode for {}".format(path))
        if not isinstance(digest, str) or len(digest) != 64 or any(c not in "0123456789abcdef" for c in digest):
            raise ArchiveError("invalid sha256 for {}".format(path))
        if path in expected:
            raise ArchiveError("duplicate manifest path: {}".format(path))
        expected[path] = entry
    validate_path_set(expected)
    if set(names) != {MANIFEST_NAME, *expected.keys()}:
        raise ArchiveError("archive member list does not match manifest")
    return members, expected


def verify_tar(tar):
    members, expected = archive_manifest(tar)
    by_name = {member.name: member for member in members}
    total = 0
    for path, entry in expected.items():
        member = by_name[path]
        if member.size != entry["size"] or stat.S_IMODE(member.mode) != entry["mode"]:
            raise ArchiveError("size or mode mismatch for {}".format(path))
        stream = tar.extractfile(member)
        digest = hashlib.sha256()
        size = 0
        try:
            while True:
                chunk = stream.read(CHUNK_SIZE)
                if not chunk:
                    break
                size += len(chunk)
                if size > entry["size"]:
                    raise ArchiveError("oversized member: {}".format(path))
                digest.update(chunk)
        finally:
            stream.close()
        if size != entry["size"] or digest.hexdigest() != entry["sha256"]:
            raise ArchiveError("checksum or size mismatch for {}".format(path))
        total += size
    return expected, total


def verify_archive(path):
    with load_archive(path) as tar:
        expected, total = verify_tar(tar)
    print("Verified {} files ({} bytes): {}".format(len(expected), total, path))


def check_destination(root, relative):
    destination = root.joinpath(*PurePosixPath(relative).parts)
    current = root
    for part in PurePosixPath(relative).parts[:-1]:
        current = current / part
        if current.exists() or current.is_symlink():
            if current.is_symlink() or not current.is_dir():
                raise ArchiveError("unsafe destination ancestor: {}".format(current))
    if destination.is_symlink():
        raise ArchiveError("destination is a symlink: {}".format(destination))
    if destination.exists() and not destination.is_file():
        raise ArchiveError("destination is not a regular file: {}".format(destination))
    return destination


def identical_file(path, entry):
    if path.stat().st_size != entry["size"] or stat.S_IMODE(path.stat().st_mode) != entry["mode"]:
        return False
    digest = hashlib.sha256()
    with path.open("rb") as source:
        while True:
            chunk = source.read(CHUNK_SIZE)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest() == entry["sha256"]


def restore_archive(archive, root):
    with load_archive(archive) as tar:
        expected, total = verify_tar(tar)
        root = Path(os.path.abspath(root))
        current = Path(root.anchor)
        for part in root.parts[1:]:
            current = current / part
            if current.is_symlink():
                raise ArchiveError("restore root has a symlink ancestor: {}".format(current))
        root.mkdir(parents=True, exist_ok=True)
        if not root.is_dir():
            raise ArchiveError("restore root must be a real directory")
        destinations = {}
        existing = set()
        for relative, entry in expected.items():
            destination = check_destination(root, relative)
            destinations[relative] = destination
            if destination.exists():
                if not identical_file(destination, entry):
                    raise ArchiveError("refusing to overwrite differing file: {}".format(destination))
                existing.add(relative)
        by_name = {member.name: member for member in tar.getmembers()}
        for relative, entry in expected.items():
            if relative in existing:
                continue
            destination = destinations[relative]
            destination.parent.mkdir(parents=True, exist_ok=True)
            check_destination(root, relative)
            fd, temp_name = tempfile.mkstemp(prefix="." + destination.name + ".", suffix=".tmp", dir=str(destination.parent))
            try:
                digest = hashlib.sha256()
                size = 0
                with os.fdopen(fd, "wb") as output:
                    source = tar.extractfile(by_name[relative])
                    if source is None:
                        raise ArchiveError("missing regular-file payload: {}".format(relative))
                    try:
                        while True:
                            chunk = source.read(CHUNK_SIZE)
                            if not chunk:
                                break
                            size += len(chunk)
                            digest.update(chunk)
                            output.write(chunk)
                    finally:
                        source.close()
                    if size != entry["size"] or digest.hexdigest() != entry["sha256"]:
                        raise ArchiveError("archive changed during restore: {}".format(relative))
                    os.fchmod(output.fileno(), entry["mode"])
                os.link(temp_name, destination)
                os.unlink(temp_name)
            finally:
                try:
                    os.unlink(temp_name)
                except FileNotFoundError:
                    pass
    print("Restored {} files ({} bytes): {}".format(len(expected), total, root))


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    create = commands.add_parser("create")
    create.add_argument("--root", type=Path, required=True)
    create.add_argument("--files", type=Path, required=True)
    create.add_argument("--output", type=Path, required=True)
    verify = commands.add_parser("verify")
    verify.add_argument("archive", type=Path)
    restore = commands.add_parser("restore")
    restore.add_argument("archive", type=Path)
    restore.add_argument("--root", type=Path, required=True)
    args = parser.parse_args(argv)
    try:
        if args.command == "create":
            create_archive(args.root, args.files, args.output)
        elif args.command == "verify":
            verify_archive(args.archive)
        else:
            restore_archive(args.archive, args.root)
    except (ArchiveError, OSError, tarfile.TarError, gzip.BadGzipFile) as exc:
        print("migration_archive: {}".format(exc), file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
