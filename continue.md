# Continue — final three decompilation matches

Read the “Final three-function checkpoint” in
`docs/decompilation-matching-notes.md` before changing source. The current
branch is the private cumulative `hermes/final-stretch`, not an upstream PR
branch. The configured IDO 7.1 compiler, `uv`, and `mips-linux-gnu-as` are
available locally.

## Last action

Improved `func_8009E3D0` from 1722 to 1707 by moving the texture-frame lookup
after depth/playback setup. Stopped an exhaustive late-float declaration sweep
cleanly at the user's requested pause; no matching process remains running.

## Next action

Run `python3 tools/score_remaining_asm.py` to reproduce all three baselines.
Then restart `nonmatchings/search_8009e3d0_late_float_orders.py` from the
committed 1707 source and let all 5,040 variants finish. If none beats 1707,
compare the target/current interference graph around `0x2DF8..0x30D8` and
design a search coupling the 32-bit counter lifetime to the projection floats.

## Why

The target and candidate are identical before the loop except for frame size.
Inside the loop, the target's `0x98(sp)` 32-bit counter and FPR reuse pattern
must be solved together; declaration position alone did not do it.

## Current evidence

- `src/window/847B60.c` — `func_80374714_847EC4`: score 50; only ten
  register operands remain.
- `src/app_render/47380.c` — `func_8009E3D0`: score 1707; target frame
  `0x2F0`, current `0x300`.
- `src/app_render/effect.c` — `fx_draw`: reproducible score 4586; target frame
  `0x2E0`, current `0x2E8`.
  The older alias-normalizing custom scorer reports 4576 for the same object.
- Exactly three `GLOBAL_ASM` fallbacks remain.

## Local donor

The complete SSB64 decompilation is available at
`/home/bbferko/repos/ssb-decomp-re`. Its matched `lbParticleDrawTextures` in
`src/lb/lbparticle.c` is the primary donor for both rendering functions. Treat
it as a source-shape lead; Pokémon Snap objects must still be scored separately.

## Do not

- Do not use the permuter's raw 1636 candidate: it drops `size *= invW` and is
  behaviorally wrong.
- Do not repeat the exhausted 847 declaration/type/reload/permuter searches
  without a new live-range hypothesis.
- Do not stage generated objects, scratch sources, `nonmatchings/`, or local
  `AGENTS.md` guidance. Build upstream PRs from fresh `pr/*` worktrees only.
- Do not delete or clean the existing untracked research artifacts; they hold
  candidate sources, search scripts, and permuter results for this worktree.
- Do not edit expected objects or accept semantic regressions for a lower score;
  exact generated code and the final ROM checksum remain the acceptance gates.
