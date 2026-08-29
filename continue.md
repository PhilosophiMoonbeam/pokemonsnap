# Continue — final three decompilation matches

## Last action

Improved `func_8009E3D0` from 1722 to 1707 by moving the texture-frame lookup
after depth/playback setup. Stopped an exhaustive late-float declaration sweep
cleanly at the user's requested pause; no matching process remains running.

## Next action

Restart `nonmatchings/search_8009e3d0_late_float_orders.py` from the committed
1707 source and let all 5,040 variants finish. If none beats 1707, compare the
target/current interference graph around `0x2DF8..0x30D8` and design a search
that couples the 32-bit counter lifetime with the seven projection floats.

## Why

The target and candidate are identical before the loop except for frame size.
Inside the loop, the target's `0x98(sp)` 32-bit counter and FPR reuse pattern
must be solved together; declaration position alone did not do it.

## Current evidence

- `func_80374714_847EC4`: score 50; only ten register operands remain.
- `func_8009E3D0`: score 1707; target frame `0x2F0`, current `0x300`.
- `fx_draw`: score 4576; target frame `0x2E0`, current `0x2E8`.
- Exactly three `GLOBAL_ASM` fallbacks remain.

## Do not

- Do not use the permuter's raw 1636 candidate: it drops `size *= invW` and is
  behaviorally wrong.
- Do not repeat the exhausted 847 declaration/type/reload/permuter searches
  without a new live-range hypothesis.
- Do not stage generated objects, scratch sources, `nonmatchings/`, or local
  `AGENTS.md` guidance. Build upstream PRs from fresh `pr/*` worktrees only.
