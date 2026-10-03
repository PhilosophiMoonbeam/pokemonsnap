#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
ROM="$ROOT/pokemonsnap.z64"
EXPECTED_ROM_SHA1=edc7c49cc568c045fe48be0d18011c30f393cbaf
ASM_REL=asm/nonmatchings/window/847B60/func_80374714_847EC4.s
WINDOW_OBJECT=build/src/window/847B60.c.o
CHECKSUM_TARGET=build/pokemonsnap.ok
CHECK_ONLY=0

usage() {
    cat <<'EOF'
Usage: tools/server-bootstrap.sh [--check-only] [--help]

  --check-only  Report prerequisites and ROM readiness without changing files.
  --help        Show this help.

Full mode uses already-installed host tools, synchronizes the frozen uv
environment, runs configure setup/extraction, then builds with ninja. It does
not install OS packages or bootstrap uv/Rust.
EOF
}

while (($#)); do
    case "$1" in
        --check-only) CHECK_ONLY=1 ;;
        --help|-h) usage; exit 0 ;;
        *) printf 'Unknown option: %s\n' "$1" >&2; usage >&2; exit 2 ;;
    esac
    shift
done

missing=()
for command in mips-linux-gnu-as mips-linux-gnu-ld \
               mips-linux-gnu-nm mips-linux-gnu-objcopy mips-linux-gnu-strip \
               cpp bash iconv ninja uv cargo pigment64 sha1sum cut cmp cp mktemp; do
    if ! command -v "$command" >/dev/null 2>&1; then
        missing+=("$command")
    fi
done
if [[ ! -f "$ROOT/uv.lock" ]]; then
    missing+=("uv.lock (frozen dependency lockfile)")
fi

if ((${#missing[@]})); then
    printf 'Missing prerequisites:\n' >&2
    printf '  - %s\n' "${missing[@]}" >&2
    printf 'Install the listed host tools and Rust/pigment64 yourself; this script does not install packages or toolchains. See docs/server-migration.md.\n' >&2
fi

rom_ready=0
if [[ -f "$ROM" ]]; then
    if command -v sha1sum >/dev/null 2>&1 && command -v cut >/dev/null 2>&1; then
        actual_sha1="$(sha1sum "$ROM" | cut -d ' ' -f 1)"
        if [[ "$actual_sha1" == "$EXPECTED_ROM_SHA1" ]]; then
            rom_ready=1
        else
            printf 'ROM checksum mismatch: %s has SHA-1 %s; expected %s.\n' \
                "$ROM" "$actual_sha1" "$EXPECTED_ROM_SHA1" >&2
        fi
    else
        printf 'Cannot verify ROM: sha1sum and cut are required.\n' >&2
    fi
else
    printf 'Required legal ROM is missing: %s (US SHA-1 %s).\n' \
        "$ROM" "$EXPECTED_ROM_SHA1" >&2
fi

if ((CHECK_ONLY)); then
    if ((${#missing[@]} == 0)); then
        printf 'Host prerequisites: ready.\n'
    else
        printf 'Host prerequisites: not ready.\n'
    fi
    if ((rom_ready)); then
        printf 'ROM: ready (expected US SHA-1).\n'
    else
        printf 'ROM: not ready.\n'
    fi
    ((${#missing[@]} == 0 && rom_ready))
    exit $?
fi

if ((${#missing[@]})); then
    exit 1
fi
if ((!rom_ready)); then
    printf 'Refusing to extract or build without the verified US ROM.\n' >&2
    exit 1
fi

cd "$ROOT"
# Keep configure.py's project-relative paths rooted at the checkout.
uv sync --frozen
if [[ ! -x tools/ido7.1/cc || ! -x tools/ido5.3/cc || ! -x tools/asm_proc/asm-processor ]]; then
    uv run --frozen ./configure.py --setup
else
    printf 'Configured IDO and asm-processor tools already exist; skipping setup download.\n'
fi

# The extraction step can regenerate this tracked research assembly. Preserve
# its exact bytes without using git reset/clean, then invalidate its object and
# checksum if restoration was needed so the subsequent full build refreshes both.
asm_path="$ROOT/$ASM_REL"
asm_backup=''
if [[ -f "$asm_path" ]]; then
    asm_backup="$(mktemp)"
    cp -p -- "$asm_path" "$asm_backup"
fi
restore_asm() {
    status=$?
    trap - EXIT INT TERM
    if [[ -n "$asm_backup" ]]; then
        if [[ -f "$asm_path" ]] && ! cmp -s -- "$asm_backup" "$asm_path"; then
            cp -p -- "$asm_backup" "$asm_path"
            rm -f -- "$ROOT/$WINDOW_OBJECT" "$ROOT/$CHECKSUM_TARGET"
        elif [[ ! -f "$asm_path" ]]; then
            cp -p -- "$asm_backup" "$asm_path"
            rm -f -- "$ROOT/$WINDOW_OBJECT" "$ROOT/$CHECKSUM_TARGET"
        fi
        rm -f -- "$asm_backup"
    fi
    if ((status == 0)); then
        (cd "$ROOT" && ninja)
        status=$?
    fi
    exit "$status"
}
trap restore_asm EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
uv run --frozen ./configure.py
