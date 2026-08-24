# Drawbitmap window/donor relationship

Current state on `hermes/final-stretch`:

- Upstream matched `src/26D80.c::drawbitmap`; it is now active C with no
  `NON_MATCHING` guard or assembly fallback.
- `src/window/8456E0.c::func_80371F54_845704` remains a separate
  `NON_MATCHING` candidate whose default build path uses assembly.
- The helpers implement the same broad sprite-texture operation, but their C
  source is not interchangeable. Upstream's matched donor implementation uses
  the complete SDK texture-block macros in several shuffled and 32-bit cases.
  The window candidate now uses a local macro for only the common load prefix,
  then emits the render-tile and tile-size commands separately.
- The surrounding `spDraw` traversal also remains different.

Helper symbol mapping

- `func_80371F54_845704` <-> `drawbitmap`
- `D_803A6640_879DF0` <-> `prev_bmbuf`
- `D_803A6644_879DF4` <-> `sp_attr`
- `D_803A6648_879DF8` <-> `scissor_xmin`
- `D_803A664C_879DFC` <-> `scissor_ymin`
- `D_803A6650_879E00` <-> `scissor_xmax`
- `D_803A6654_879E04` <-> `scissor_ymax`

Key divergence window

Window path (`src/window/8456E0.c`):
- starts at `b = s->bitmap + s->istart`
- computes `b_end = b + s->nbitmaps`
- advances with `b += s->istep`

Donor path (`src/26D80.c`):
- starts at `b = s->bitmap`
- iterates with `i < s->nbitmaps`
- advances with `b++`

How upstream matched the donor helper

- The earlier fork candidate manually emitted `gDPSetTextureImage`,
  `gDPSetTile`, load-sync, load-block, pipe-sync, render-tile, and tile-size
  commands for shuffled 8-bit, shuffled YUV 16-bit, and both 32-bit paths.
- Upstream expressed those paths with `gDPLoadTextureBlockS`,
  `gDPLoadTextureBlockYuvS`, and `gDPLoadTextureBlock`.
- Those macros describe the same intended RDP setup at a higher level, but
  they also produce the compiler/code-generation shape needed for the original
  ROM. That is the decisive decompilation difference: upstream's function is
  byte-matching, while the fork's expanded version remained only a candidate.
- Upstream consequently removed the `NON_MATCHING` guard and
  `drawbitmap.s` fallback.

Why the window helper still differs

- Replacing the window helper's expanded commands with the same complete SDK
  macros does not match its ROM function. The two helpers were compiled from
  different source shapes despite their shared behavior.
- Grouping just the first five commands in `LOAD_TEX_BLOCK_PREFIX` reproduces
  IDO's target scheduling in the shuffled 8-bit, shuffled YUV 16-bit, and
  shuffled 32-bit paths. This reduces the window candidate from 44 differing
  bytes in 11 words to 8 differing bytes in 2 adjacent words, while preserving
  the exact `0x171C`-byte function size.
- The sole remaining difference is an instruction-order tie in the shuffled
  32-bit render-tile path. The target stores the second `gDPSetTile` word before
  shifting `tex_width - 1`; the candidate schedules that shift first. There is
  no control-flow, stack, or register-allocation difference.
- The assembly fallback remains authoritative until those last two words
  match. The partial C improvement does not change the default ROM.

Current live/default context

- `src/window/element.c` initializes donor-style traversal defaults:
  - `istart = 0`
  - `istep = 1`
- `src/window/845160.c` constructors copy Sprite templates verbatim.
- Current live layout callers do not patch `istart/istep` after construction.

Implication

- Treat upstream's donor implementation as the proven source-shape reference,
  not merely as a semantically equivalent helper.
- Window matching work should preserve the local load-prefix boundary; using
  the donor's complete SDK macro regresses three already-solved scheduling
  regions.
- Do not copy donor `spDraw` wholesale: the `istart`/`istep` traversal remains a
  real window-specific distinction even when current callers use `0` and `1`.
