#!/usr/bin/env python3
"""Rebind private-state absolute symlinks after restoring a migration overlay."""

import argparse
import json
import os
from pathlib import Path, PurePosixPath
import secrets
import subprocess
import sys
from typing import NoReturn


def fail(message) -> NoReturn:
    raise ValueError(message)


def validate_member_path(value):
    if not isinstance(value, str) or not value or "\\" in value:
        fail(f"invalid inventory path: {value!r}")
    path = PurePosixPath(value)
    if path.is_absolute() or any(part in ("", ".", "..") for part in value.split("/")):
        fail(f"unsafe inventory path: {value!r}")
    return path


def check_parents(base, parts):
    current = base
    for part in parts[:-1]:
        current = current / part
        try:
            current.lstat()
        except FileNotFoundError:
            fail(f"missing parent directory: {current}")
        if not os.path.isdir(current) or os.path.islink(current):
            fail(f"parent is not a real directory: {current}")


def readlink(path):
    try:
        return os.readlink(path)
    except OSError as exc:
        fail(f"cannot inspect {path}: {exc}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", required=True, type=Path)
    parser.add_argument("--root", required=True, type=Path)
    parser.add_argument("--check-only", action="store_true")
    args = parser.parse_args()

    try:
        with args.inventory.open(encoding="utf-8") as stream:
            inventory = json.load(stream)
        old_root = inventory["source_root"]
        members = inventory["private_members"]
        if not isinstance(old_root, str) or not os.path.isabs(old_root):
            fail("inventory source_root must be an absolute path")
        if not isinstance(members, dict):
            fail("inventory private_members must be an object")
        root = args.root.resolve(strict=True)
        if not root.is_dir():
            fail(f"checkout root is not a directory: {root}")
        common_result = subprocess.run(
            ["git", "-C", str(root), "rev-parse", "--path-format=absolute", "--git-common-dir"],
            check=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        )
        common_dir = Path(common_result.stdout.strip())
        if not common_dir.is_absolute():
            fail("git returned a non-absolute common directory")

        planned = []
        preserved = 0
        for raw_path, entry in members.items():
            relative = validate_member_path(raw_path)
            if relative.parts[0] == ".omp":
                continue
            if relative.parts[0] == ".git":
                prefix = (".git", "local-tools", "binutils")
                if relative.parts[:3] != prefix:
                    continue
                destination = common_dir.joinpath("local-tools", "binutils", *relative.parts[3:])
                base = common_dir
                destination_parts = ("local-tools", "binutils", *relative.parts[3:])
            else:
                destination = root.joinpath(*relative.parts)
                base = root
                destination_parts = relative.parts
            if not isinstance(entry, dict) or entry.get("type") != "symlink":
                continue
            expected = entry.get("target")
            if not isinstance(expected, str):
                fail(f"symlink has invalid target metadata: {raw_path}")
            if expected.startswith(old_root + "/"):
                new_target = str(root) + expected[len(old_root):]
            else:
                new_target = expected
            check_parents(base, destination_parts)
            if not os.path.lexists(destination):
                fail(f"missing inventoried symlink: {destination}")
            if not destination.is_symlink():
                fail(f"inventoried symlink path is not a symlink: {destination}")
            current = readlink(destination)
            if current not in (expected, new_target):
                fail(f"unexpected symlink target at {destination}: {current!r}")
            if current == expected and new_target != expected:
                planned.append((destination, expected, new_target))
            else:
                preserved += 1

        if args.check_only:
            print(f"relinks_required={len(planned)} preserved_unchanged={preserved}")
            return 1 if planned else 0

        for destination, expected, new_target in planned:
            if not destination.is_symlink() or readlink(destination) != expected:
                fail(f"symlink changed before relink: {destination}")
            temporary = destination.with_name(destination.name + ".relink-" + secrets.token_hex(8))
            try:
                os.symlink(new_target, temporary)
                os.replace(temporary, destination)
            finally:
                try:
                    temporary.unlink()
                except FileNotFoundError:
                    pass
        print(f"relinked={len(planned)} preserved_unchanged={preserved}")
        return 0
    except (OSError, ValueError, KeyError, json.JSONDecodeError, subprocess.CalledProcessError) as exc:
        print(f"relink-migration-symlinks: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
