#!/usr/bin/env python3
"""Compile and score the remaining GLOBAL_ASM C candidates."""

from __future__ import annotations

import re
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
INCLUDES = (
    "include",
    "src",
    "ultralib/include",
    "ultralib/include/ido",
    "ultralib/include/PR",
    "ultralib/src",
    "build/include",
    "build",
    ".",
)
INCLUDE_ARGS = [arg for path in INCLUDES for arg in ("-I", path)]
COMPILE = [
    "tools/asm_proc/asm-processor",
    "--input-enc=utf-8",
    "--output-enc=EUC-JP",
    "tools/ido7.1/cc",
    "--",
    "mips-linux-gnu-as",
    "-G",
    "0",
    *INCLUDE_ARGS,
    "-EB",
    "-mtune=vr4300",
    "-march=vr4300",
    "--",
    "-G",
    "0",
    "-non_shared",
    "-fullwarn",
    "-verbose",
    "-Xcpluscomm",
    "-nostdinc",
    "-Wab,-r4300_mul",
    "-O2",
    "-DNON_MATCHING",
    "-mips2",
    *INCLUDE_ARGS,
    "-DF3DEX_GBI_2",
    "-D_LANGUAGE_C",
    "-DNDEBUG",
    "-D_FINALROM",
    "-DBUILD_VERSION=VERSION_I",
    "-c",
]
DIFF = [
    "uv",
    "run",
    "--with",
    "watchdog",
    "--with",
    "levenshtein",
    "--with",
    "cxxfilt",
    "./diff.py",
]
TARGETS = (
    ("func_8009E3D0", "src/app_render/47380.c", "build/src/app_render/47380.c.o", ()),
    ("fx_draw", "src/app_render/effect.c", "build/src/app_render/effect.c.o", ("-I", "src/app_render")),
)


def score(symbol: str, source: str, object_name: str, extra_args: tuple[str, ...]) -> int:
    object_path = (ROOT / object_name).resolve()
    # Validate the mirrored reference before compilation can overwrite the output.
    object_name = str(object_path.relative_to(ROOT))
    expected_path = ROOT / "expected" / object_name
    if not expected_path.is_file():
        raise FileNotFoundError(f"missing reference object: {expected_path}")
    if object_path.exists() and object_path.samefile(expected_path):
        raise ValueError(f"candidate aliases reference object: {object_path}")
    object_path.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(
        [*COMPILE, *extra_args, "-o", str(object_path), source],
        cwd=ROOT,
        check=True,
        stdout=subprocess.DEVNULL,
    )
    result = subprocess.run(
        [
            *DIFF,
            symbol,
            "-o",
            "-f",
            object_name,
            "--no-pager",
            "--format",
            "plain",
            "--algorithm",
            "difflib",
            "-U",
            "0",
            "--max-size",
            "15000",
        ],
        cwd=ROOT,
        check=True,
        text=True,
        stdout=subprocess.PIPE,
    )
    first_line = result.stdout.splitlines()[0]
    match = re.search(r"CURRENT \((\d+)\)", first_line)
    if match is None:
        raise RuntimeError(f"could not parse score for {symbol}: {first_line}")
    return int(match.group(1))


def main() -> None:
    for symbol, source, object_name, extra_args in TARGETS:
        print(f"{symbol}: {score(symbol, source, object_name, extra_args)}", flush=True)


if __name__ == "__main__":
    main()
