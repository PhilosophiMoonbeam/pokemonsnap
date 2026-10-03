# Decompilation matching notes

## `func_80371F54_845704`

The window-specific drawbitmap helper now compiles to the target's exact
`0x171C`-byte size and differs in only one adjacent instruction pair. The
previous expanded-command candidate differed in 44 bytes across 11 words.
Wrapping the shared `gDPSetTextureImage`, load-tile, load-sync, load-block, and
pipe-sync sequence in a local `LOAD_TEX_BLOCK_PREFIX` macro reduces that to 8
bytes across 2 words.

The important IDO 7.1 finding is that macro-origin boundaries affect final
instruction scheduling even when the expanded C operations are otherwise the
same. The prefix macro fixes three independent `G_RDPPIPESYNC` opcode
load scheduling regions without altering the function's control flow, stack,
or register allocation. Replacing the whole sequence with
`gDPLoadTextureBlockS`, as the matched donor drawbitmap does, regresses other
regions; the window helper requires a different boundary.

The remaining mismatch is confined to the shuffled 32-bit render-tile path:
the target orders `sw t9, 4(v1)` before `sll t6, t8, 2`, while the candidate
orders the shift before the store. Equivalent arithmetic, temporary locals,
statement movement, comma expressions, same-line forcing, local wrapper
macros, and assembler tuning all preserve that final scheduling choice.

The decisive new localization is after code generation. IDO's `ugen` output
for the two-word candidate already places the store before the shift, exactly
like the target. The final `as1` assembler/reorganizer moves the independent
shift above the store. Marking the display-list stores volatile makes `ugen`
emit `.set volatile` around them, but does not stop this arithmetic hoist and
also perturbs a later store schedule. Future work should therefore target the
assembler's instruction-mobility metadata or a source/control-flow boundary
that survives into binasm, rather than continuing to permute equivalent
render-tile arithmetic. The `NON_MATCHING` guard remains in place.

## `UIMem_Reallocate`

`UIMem_Reallocate` was matched by recovering two source-shape details that are
not obvious from a semantic C translation:

- The retry after merging the following free chunk is tail recursion:
  `return UIMem_Reallocate(data, size);`. At `-O2`, IDO turns this into the
  backward branch in the original function. Writing the same operation as a
  `while` loop allowed the compiler to hoist `GET_CHUNK(data)` into a saved
  register, which changed the prologue and register allocation throughout the
  function.
- The bounds check and predecessor update compute `NEXT_CHUNK(next)` inline.
  A separately assigned `afterNext` pointer changed the temporary-register
  allocation. The otherwise unused `afterNext` declaration is retained because
  it reproduces the original `0x58` frame and places the chunk spill at
  `0x30(sp)`.

This is a useful general IDO matching pattern: when an assembly retry edge
recomputes values that a direct C loop causes the compiler to hoist, test a
tail-recursive source form. Also distinguish a local's declaration from its
use—an unused declaration can still affect historical stack-slot allocation.

## `func_80374714_847EC4`

The window photo downsampler is now integrated as ordinary C in the production
translation unit, with its `GLOBAL_ASM` fallback removed. The configured normal
IDO compiler produces an exact 387-word object (`CURRENT (0)`); this is not a
forced-register or forced-compiler result.

The minimal change from the prior score-10 source removed the tail
`texelCount = sprite->height` assignment and computed the reloaded cache size
directly as:

```c
cacheSize = (s32) ((u64) reloadBitmap->width_img) *
            (s32) ((u64) sprite->height);
```

`clang-format` 21.1.8 was run. Narrow `clang-format off/on` comments preserve
the same-line four-pixel load statements required for IDO scheduling.

The latest height-graph run evaluated 37 candidates. Its metrics identify
`reload-width-cast` as the exact candidate; the baseline is the 387-word
two-register-only near-match. The validated artifacts are
`nonmatchings/orchestrated-202606/window/height-graph-results/metrics.json`,
`nonmatchings/orchestrated-202606/window/height-graph-results/exact-match.c`,
and `nonmatchings/orchestrated-202606/baselines/window-exact.diff`. The
following bullets preserve the historical search record:

- Loading the four RGBA5551 texels into signed `s16` scalar locals before the
  row-parity branch makes IDO hoist the loads and reproduce the target's channel
  arithmetic almost instruction-for-instruction.
- Expressing each channel as a four-value sum followed by signed division by
  four is important; helper calls or pre-averaged unsigned expressions produce
  a substantially different instruction sequence.
- The target handles an odd leading output pixel and then processes two output
  pixels per loop iteration. Mirroring that unrolling recovered the target's
  main control-flow shape.
- Volatile row-dimension locals reproduce several target stack spills, although
  the remaining frame size, load order, failure-path cache arguments, and loop
  register allocation still need work.
- Scoping each set of four pixel temporaries to one output calculation reduced
  the generated frame from `0x50` to `0x48`; the target frame is `0x40`. This
  applies the allocator lesson that declaration lifetime and stack allocation
  can matter even when the values themselves stay in registers.
- IDO unrolls the failure-path clear loop only when the loop header and store
  share a source line. This recovers the target's four-halfword store group.
- Splitting `srcRow1 = srcPixels + src->width` into an assignment followed by
  `+=` changes the MIPS `addu` operand order to match the target at all three
  pixel sites.
- The target computes bitmap-row parity once and reuses it for both pixels in
  the unrolled pair. An explicit scoped `rowParity` local documents that reuse
  and avoids relying on repeated-condition common-subexpression elimination.
- Keeping the four source-pixel assignments on one source line changes only
  the second pixel's load schedule in the unrolled pair: IDO loads the two
  row-1 pixels first, then the two row-0 pixels around the parity branch, as in
  the target. This reduces the direct comparison score from 2,030 to 1,990.
- A failure-only clear index prevents the clear loop from perturbing the hot
  path's source-row counter allocation. Chaining the width through the
  bitmap-row local before resetting that local gives the best current
  allocation without changing behavior.
- Exhaustively permuting plain initialization statements did not change the
  decisive register cycle. The best current source shape instead copies width
  through the bitmap-row local, resets that local, and then initializes height,
  bitmap, source row, destination width, and tile height. This keeps the source
  row in the target's `ra` register.
- A two-element height temporary currently supplies the closest allocation,
  but it also explains the remaining extra stack slot. The other principal gap
  is the failure-path cache writeback: the target reloads the sprite bitmap and
  recomputes its byte count, while straightforward C makes IDO retain the first
  values or reserve a second saved register.
- In the full `847B60.c` translation unit, reusing the success-path `y` local as
  the failure clear-loop counter, moving `width` after `bitmapRow` in the outer
  declarations, and retaining an optimized-away `if (bitmapRow && bitmapRow)`
  inside the first paired-pixel block reduces the object score from 2,060 to
  2,003. The empty condition changes no instructions directly, but it corrects
  the important `width = t5`, `bitmapRow = t4`, `y = ra` allocation cycle.
- A scalar volatile height under the improved declaration ordering reproduces
  the target's `0x40` frame and `0x44(sp)` saved sprite argument while keeping a
  live tile-height value spilled at `0x20(sp)`. Its height and destination-width
  spills land too low and its hot-loop allocation regresses, so it is structural
  evidence rather than the current best candidate.
- Declaration-only permutation with a frame-focused scorer found another
  scalar-height layout that simultaneously emits the `0x40` frame and places
  destination width and tile height at the target's `0x28(sp)` and `0x20(sp)`;
  height remains at `0x2C(sp)` instead of `0x34(sp)`. Thousands of declaration
  permutations did not produce all three target slots together, indicating
  that declaration order alone is insufficient.
- The target reuses `t5` for the failure texel count and success width, `ra` for
  the failure clear index and success source row, and `v1` for the bitmap in both
  paths. Reusing the corresponding C locals is therefore a strong source-shape
  lead, but the first combined experiment disturbed the hot-loop allocation and
  scored worse; it needs to be paired with a different declaration/lifetime
  arrangement rather than adopted directly.
- A chained scalar lifetime finally reproduces the target's three otherwise
  incompatible stack facts at once. Assigning a long-lived `height` from a
  short-lived `initialHeight`, branching on the short-lived value, and comparing
  the loop back-edge against the long-lived value emits one height store rather
  than the two stores from `height[2]`. With either of two discovered declaration
  orders, IDO emits the exact `0x40` frame and places height, destination width,
  and tile height at `0x34(sp)`, `0x28(sp)`, and `0x20(sp)` respectively while
  preserving `width = t5`, `bitmapRow = t4`, and `y = ra`.
- That scalar-lifetime result is structural evidence, not a match: its success
  initializer uses `v1` for height and loads the bitmap directly into `s0`,
  whereas the target uses `a2` for height, `v1` for the bitmap, and then
  `move s0, v1`. An exhaustive sweep of all 2,240 dependency-valid initializer
  statement orders and 1,920 relevant signedness/`register` type combinations
  did not produce the target register pair. The next search should change the
  value-lifetime/control-flow graph rather than repeat declaration, statement,
  or scalar-type permutations.
- A compiler-erased `(bitmapRow && bitmapRow);` expression between the success
  width copy and bitmap-row reset changes the live-range graph without emitting
  an instruction. On the scalar-height layout it reduces register penalties
  from 495 to 71 and the full-object score from 3,855 to 1,735 while retaining
  the exact `0x40` frame and `0x34(sp)`, `0x28(sp)`, and `0x20(sp)` slots.
- The target's `mflo a1; move t5, a1` reveals two failure-path bound lifetimes.
  Computing a short-lived `texelCount`, copying it to the success-path `width`
  local, testing the former, and comparing the clear-loop back edge against the
  latter reproduces that split allocation. A compiler-erased
  `(width && width);` changes the generated pretest from `blezl` to the target's
  ordinary `blez` and lets IDO fill its delay slot with the loop-remainder mask.
- Reloading the bitmap after the clear loop and recomputing a block-local
  `cacheSize` reproduces the target's second size calculation without reserving
  another saved register. Combined with the two no-op expressions, this is the
  1,470 candidate: stack penalty 0, register penalty count 82, one reordering,
  three insertions, and seven deletions. The remaining failure
  allocation is `v0/a2/v1` where the target uses `v1/t7/a1`; the success
  initializer still loads height into `v1` and the bitmap directly into `s0`
  instead of `a2` and `v1` followed by `move s0, v1`.
- Expressing bitmap progression as `bitmap[bitmapIndex]` and incrementing the
  index at each tile boundary lets IDO strength-reduce that index into the
  target's `s0` pointer cursor. It emits the previously missing
  `move s0, v1` and reduces the full-object score from 1,470 to 1,375. A
  compiler-erased `(bitmap && bitmap);` immediately after loading the bitmap
  then gives the initializer its target `v1` base register and lowers the score
  to 1,355.
- The target failure branch goes directly to `osWritebackDCache` when the texel
  count is nonpositive. Its bitmap reload and byte-count recomputation occur
  only after a clear loop that actually ran. Moving those assignments inside
  the positive-count block reproduces the complete target failure-path length,
  including `lw a0` followed by `move t1, a0`, `mflo a1` followed by
  `move t5, a1`, an ordinary `blez`, and the two multiply-delay `nop`s. Sharing
  the outer bitmap local also gives the initial failure bitmap its target `v1`
  register. That more faithful control flow currently perturbs the success-path
  allocation and scores 3,005, so it remains a structural lead rather than the
  checked-in best arrangement. A `while (y != width)` form restores much of the
  old allocation and scores 1,580, but adds an unwanted pretest and changes the
  unrolled store schedule.
- Preserving that exact failure CFG while copying `sprite->height` through the
  outer `texelCount` local restores the success entry's saved-sprite allocation.
  Reusing the now-dead `photo` parameter as the success-height carrier then
  gives the target's `a2` height load without emitting an extra instruction.
  Together with `if (bitmapRow) {}` in the first paired-pixel block, these
  live-range changes reduced the full-object score from 1,355 to 640 while
  retaining the exact frame, stack slots, branches, and pixel arithmetic.
- A redundant `cacheSize = width` immediately inside the positive failure-size
  branch is optimized away, but makes IDO use the target's copied `t5` bound for
  both the clear-loop remainder and its early equality test. This lowers the
  score from 640 to 630.
- Moving the odd-pixel destination increment into the comma condition
  `if (dst++, x != width)` is behaviorally equivalent to incrementing before
  the condition. It makes IDO place that increment in the target branch's delay
  slot and removes the last general instruction-reordering penalty, lowering
  the score from 630 to 570.
- Dummy comma operands can also perturb IDO's operand allocation without
  changing generated operations. Using `(0, texelCount)` in the failure-path
  recomputation corrects one multiply operand, and wrapping both dimensions in
  the final cache-size product corrects the final height register. The current
  full-object score is 560: zero stack differences, zero branch differences,
  zero reorderings, 32 register-field differences, two inserted instructions,
  and two deleted instructions. The displaced operations are the success-loop
  width-parity mask and the final cache-size multiply; the remaining register
  differences form one initializer/hot-loop color cycle plus the two failure
  dimension reloads.
- Giving the final cache-size expression its own `volatile Bitmap*` alias is a
  register-allocation lever even though the pointer value is unchanged. The
  declaration must occur after `tileHeight` (earlier placements grow the frame
  to `0x48`). Reading width through that pointer-to-volatile alias, reversing
  the dummy-comma product operands to height-then-width, and reading the buffer
  through `sprite->bitmap` gives the target's final `v1/a2/a3/a0/a1` registers.
  This reduced the score from 560 to 400; the final tail then differed only by
  the order of the independent buffer load and multiply.
- An empty three-term condition on the volatile destination width immediately
  inside the positive-width block is another IDO graph-shaping artifact:
  `if (((!dstWidth) && (!dstWidth)) && (!dstWidth)) {}`. One- or two-term forms
  do not have the same effect. It schedules the width-parity mask at the target
  location and removes the corresponding target deletion, reducing the
  independently reproduced full-translation-unit score from 400 to 295. The
  current candidate has zero stack and branch differences, 27 register-field
  differences, one reordering, and one inserted volatile-width reload.
- IDO's `-Wo,-zdbug:6` optimizer trace explains why that otherwise empty
  condition matters: it creates four extra optimizer expressions before the
  parity mask, shifting the global coloring order. A control-equivalent goto
  chain produces the same 295 object, confirming that the control graph rather
  than the boolean value is the lever. Guarding those volatile reads behind a
  condition known false inside the positive-width block removes their reload,
  but leaves two inserted and two reordered operations and scores 455.
- A corrected thread-local scorer search evaluated 8,864 valid, nonconflicting
  two-feature combinations without finding a score below 295. A separate
  exhaustive pass over all 4,096 subsets of `register` qualifiers for the 12
  principal locals also found no improvement. Row-setup statement order,
  `do`/`for` loop-header formulations, typed versus byte-address arithmetic,
  and 64 combinations of destination/source row-address syntax likewise did
  not beat 295. These negative results narrow the remaining work to a more
  structural live-range or optimizer-graph difference, not a simple local
  declaration, qualifier, operand-order, or pointer-syntax tweak.
- Public provenance searches found one historical decomp.me family for this
  exact symbol (`6UcgY` and fork `wGfq8`), but both contain the same older
  29,263/38,700 reconstruction. None of the 339 Pokémon Snap preset scratches
  or any public fork branch contains a newer implementation. The old scratch
  does corroborate the target's natural `sp34`, `sp28`, `sp20`, `t4`, `s0`,
  `t1`, and `t0` local roles, but does not improve the current object.
- A further 1,428 compiler-erased boolean-tree variants and 1,200 volatile
  algebraic identities did not beat 295. One-element-array and address-taken
  storage forms preserved destination-width semantics but regressed stack or
  register allocation, and a natural single-pixel source loop did not trigger
  IDO's required odd-pixel/two-pixel unroll. This independently reinforces that
  the remaining blocker is a structural interference-graph shape rather than
  boolean spelling or storage syntax.

Clean-source reconstruction now reproduces the compiler diagnostic: the
baseline has 387 words and two mismatches; the exact source and a narrowly
forced build each have 387 words and zero mismatches. The force changes only
function 7, evaluation 68's desired-register argument from `v1` to `t7`;
changing `f_ureg` itself also changes parent state and does not match.

The descriptor producer is `f_build_u1`: incoming Ucode word `+12` becomes tree
word `+44`, value `0x0c`, which `f_ureg` converts to register 3. The readable
IDO `uoptutil` `coloroffset` table excludes `t6`–`t9`. Thus the old path was
Uopt global-register metadata consumed by Ugen, not an alternate Uopt global
color for `t7`. Reconstructed proof logs and parsed metrics are preserved under
`nonmatchings/orchestrated-202606/compiler/verified-traces/` as
`reconstruction-final-{baseline,exact,forced}`. Production acceptance uses only
the normal configured compiler.

The natural-reload and compiler-erased no-op permutations in this section are
historical evidence, not current work items. The validated 37-candidate
height-graph run supersedes those stale handoff suggestions.

## `func_80003530`

The in-memory VPK0 decoder is an exact match in C. Its decisive donor was the
same unused `0x54C`-byte routine in the Super Smash Bros. 64 decompilation,
where the shared HAL implementation was already matched. Adapting that source
exposed several mistakes in the earlier semantic reconstruction:

- The temporary Huffman pool is 64 nodes, not 65. The target places the pool at
  `0x124(sp)` and its two 20-entry pointer stacks at `0xBC(sp)` and `0x5C(sp)`.
- The allocator pointer and the later length-tree traversal pointer are the
  same source local (`lengthsNode`). Reusing it, as the matched streaming
  decoder does, is closer than introducing a dedicated `poolPtr`.
- Header parsing alternates two macros that shift in a halfword while adding or
  subtracting 16 from the bit count. Although the count changes cancel, this
  source shape makes IDO emit the target load schedule and leaves eight bits in
  the reservoir after consuming the sample method.
- Dynamic-width extraction uses `32 - (width + numBits)` rather than the more
  natural `(32 - width) - numBits`. Together with intentionally repeated
  assignments after macro expansion, this produces the target's single shared
  `32` constant and `s0`-through-`s3` allocation.
- A pointer-typed temporary is deliberately used as the one-bit traversal
  result. IDO warns about the pointer/integer assignment, but changing it to an
  integer changes register allocation and loses the match.

The final direct object comparison score is zero for all `0x54C` bytes. This is
a useful late-stage strategy: search other games from the same studio for
identical-size utility routines before spending more time coercing a plausible
semantic rewrite.

The repository currently has a separate clean-link regression: the generated
undefined-symbol inputs are incomplete, and a compatibility link does not
reproduce the checked ROM checksum. This is independent of the decoder. Linking
once with the original assembly object and once with the C object produces the
same full-ROM SHA-1 (`e50e6b78190869e4a951381178f7fa11f75f2535`), while the
decoder's ROM range at `0x4130..0x467B` matches the target byte-for-byte in both
images.

## `func_80000F40`

The scheduler VI-mode update is an exact match in C. Compiling Super Smash
Bros. 64's matched `sySchedulerUpdateViMode` with Pokémon Snap's IDO 7.1
toolchain produced a `0x7A8`-byte function whose raw bytes were identical to
Pokémon Snap's target. Adapting that shared HAL source exposed four behavioral
errors in the earlier reconstruction: the anti-alias mode condition was
inverted, `unk_b04` selected the wrong out-of-bounds mode, horizontal offsets
used arguments 4/5 instead of 3/4, and the second-field origin inverted the
in-bounds scale.

The remaining matching details were:

- `ViSettings` uses `u32` bitfields, not `u8` bitfields. Both declarations have
  the observed packed bit layout, but only the 32-bit base type gives IDO the
  target's whole-word loads and register lifetimes.
- The donor's `not_phi_v1` and `not_res_in_bounds` locals induce the target's
  reused boolean values without expanding the stack frame.
- Three deliberate `if (1);` statements emit no instructions but affect IDO's
  control-flow graph and register allocation. Removing them changes the VI
  origin calculation despite preserving behavior.
- Parenthesizing the second-field origin as one nested product is decisive.
  Reassociating the multiplication makes IDO reload `width`, shortens the
  function by four bytes, and changes the final instruction schedule.

The final direct object comparison score is zero for all `0x7A8` bytes. As with
the VPK0 decoder, an identical-size routine in another HAL game should be
compiled and compared directly before attempting a source-level rewrite; type
declarations and no-op control-flow artifacts may be part of the match.

## Final two-function checkpoint

The window photo downsampler is complete in C and is no longer a remaining
fallback. Exactly two production `GLOBAL_ASM` fallbacks remain:
`func_8009E3D0` and `fx_draw`.

The scorer uses the configured IDO 7.1 compiler and repository `diff.py`
with the expected translation-unit object and `difflib`. The freshly updated
`python3 tools/score_remaining_asm.py` compiles only those two targets and
reports:

- `func_8009E3D0`: **1707**.
- `fx_draw`: **4366**. The earlier **4586** figure is stale.

Neither remaining renderer is an exact C match. Donor code, including SSB64's
matched `lbParticleDrawTextures`, remains research evidence rather than an
acceptance claim.

Fresh parent verification restored the production build and reported
`build/pokemonsnap.z64: OK`. The legacy full-TU splice helper now cuts at the
next function boundary instead of relying on the removed window fallback; its
recompiled exact window candidate still reports `CURRENT (0)`.

The local `.omp/compile_commands.json` has corrected clangd include paths.
Window diagnostics are 0 errors and 1 existing unused-include warning. The
source renderers remain hash-identical to their recorded entries in
`nonmatchings/orchestrated-202606/entry-hashes.json`; that manifest is baseline
evidence, not a reason to rewrite unrelated renderer sources.

The upstream anchor is `1978bb52`, equal to its remote anchor. Only two lockfile
patches are applied locally, without commits: `gitpython` 3.1.60 and
`mapfile-parser` 2.13.2.

The completed ordinal-index experiment produces the particle target's unmasked
compiler-generated counter class (`lw t6` / `addiu t7`). Best ordinal source:
1716, frame `0x308`, counter `0x94`; removing the projection-row alias reaches
counter `0x98` at 2052, but still misses frame `0x2f0`. These are structural
observations, not improvements over the production candidate's full score.
The old effects-union ordinal family is unsafe: overwriting the effects-base
storage inside the loop can invalidate later iterations. Do not integrate it.
Evidence: `nonmatchings/orchestrated-202606/particle/mechanism/ordinal-results-full/metrics.json`.

For `fx_draw`, swapping the first two squared terms of the second matrix norm
improves the research score to 4334. The full function still has 1375 words and
frame `0x2e8`, versus target 1374 words and frame `0x2e0`. The 24 scope, 13
norm-load, and 7 list-loop experiments did not produce an exact match.
That norm-only source is preserved at
`nonmatchings/orchestrated-202606/effect/mechanism/norm-rqhp27ag/008-0d38708bd667/candidate.c`.
Later camera-carrier work reaches 4316 with the exact `0x2e0` frame, still
1375 words: `effect/camera-carriers/008-14a02f4f6c25/candidate.c` beneath the
same research root. Removing the erased `if (sp220);` reaches 1374 words but
worsens the score to 4536. No partial renderer candidate was integrated.

The isolated Uopt allocation diagnostic was verified against four normal
particle objects with zero changed instruction words. CFE local bytes and
pre-reemit spill bytes are separate: baseline 620+36, ordinal 624+40,
no-projection-alias ordinal 620+40, compact carriers 596+40. The first
current-block `Urlda` snapshots that preallocated space into the final Mmt
definition. A later `gettemp` does not account for the excess frame.

Callee saves are a third component. The compact reciprocal/radius carrier
adds an `$f26` save/restore pair, increasing the frame by eight bytes and
code by two words. The `left`/`top` split radius carriers instead reach
the exact particle frame `0x2f0`, counter `0x98`, and 1420-word size, but
remain nonmatches at 9342/9307. Their sources are under
`particle/compact-float-carriers/runs/{009-left-split,011-top-split}/`.
Fixed matrix offsets constrain a 20-byte particle declaration prefix and a
24-byte effect prefix; these are storage constraints, not recovered names.

Allocation logs are under `particle/highwater/trace-probes/alloc-*`.
Filter spill records by `proc=38`; their `seq` counts allocation events.
The logger uses big-endian packed VariableLocation fields and must not read
inactive `temploc` union members for islda/isvar.

Continue mechanism-driven research under `effect/float-probe/` and
`particle/layout-refinement/`. Earlier mechanism and allocation runners are
frozen dependencies. This work does not change the completed window checkpoint.

The research scripts and generated candidates under `nonmatchings/` are local
scratch artifacts and are intentionally excluded from contribution commits.

### Safe candidate scoring

Use the scoring helper with a private output object and a mirrored original
reference under `expected/`. The helper normalizes absolute output paths to
repository-relative paths and rejects missing references or candidate/reference
aliases before compiling. Object-mode `diff.py` also normalizes absolute paths
before resolving the expected object. Previously, an absolute path discarded
the reference prefix and produced a false zero by comparing an object to itself.

The blocker audit reproduced 14835 versus false zero for the same object,
then verified the corrected helper with real normal-IDO builds: the current
best sources score 1186 and 3472 through both absolute and relative paths.
Neither renderer is exact. Preserve the source
hash with each complete diff; a shared scratch object holds only the last
experiment, not necessarily the best candidate.

Object comparison now asks objdump for the complete named symbol rather than
everything after its label. A two-function smoke object demonstrated the
boundary bug: changing only the later function previously scored 5 and now
scores 0. A difference after the selected function's first return still scores
5, proving the fix does not truncate multiple-return functions. Both renderer
scores remain 1186/3472 with the corrected symbol boundaries.

The historical references also use `D_800BE288` where current C uses
`fx_SpriteBanks`, linked at 0x800BE288. This contributes 10 points without
an instruction difference. Private canonical references under
`expected/nonmatchings/orchestrated-202606/blocker-audit-20260927/canonical-reference/`
rename only that symbol with `objcopy --redefine-sym`. All allocated section
bytes and all relocation meanings were verified unchanged after that alias.
Normal best-source scores are 1176/3462 against these references; this is
metric normalization, not matching progress. Historical scores remain
1186/3472. Identical canonical objects score zero; changing the array to an
unrelated symbol still scores 10. See `canonical-reference/manifest.json`
for hashes and controls. Do not rename production C or ignore arbitrary
symbols; canonical zero still requires the complete normal-IDO comparison
and final linked ROM checksum.

### Bind compiler traces to operations

Do not infer a compiler node's identity from a familiar stack offset or carry
bit numbers between source variants. The old particle trace labeled home -248
as the reciprocal; assignment endpoints identify it as normalized Z. In the
current 1186 source, `invW` at -240 has no global range. The reciprocal is a
division-expression range (bit 1086, internal color 30, emitted F20).
The corrected passive trace emits the complete 17136-byte translation-unit
text identically to the configured compiler.

An assigned color or a shared basic block does not identify the interference
edge that excluded another register. Keep hook source and build dependencies
with the trace, record actual operands and assignment endpoints, and prove
zero changed code before using diagnostic output to guide source changes.

The restored allocator probe records both initial constraints and the inline
colored-neighbor propagation in `globalcolor`. The initial `updateforbidden`
hook alone missed those later edges. Its complete TU text matches normal IDO
for both best candidates (17136/21760 bytes). The archived float probe inputs
and hashes are in `blocker-audit-20260927/allocator-propagation-equivalence.json`.

The actual particle F2 blocker is the size range at -168, selected at priority
50 before the division's 16.6667. For effect, the division expression itself
takes F2 at priority 5000 and forbids it for the destination variable, later
selected at 1666.67. `compute_save` divides frequency-weighted savings by a
compressed live-unit/bitvector count. Trace priority values are meaningful
only after computation (`phase=priority-selected`), not at initialization.

Changing one allocation is not sufficient: fusing particle size scaling makes
the division emit into target F2, but introduces a global inverse variable
and an extra spill at SP+0x200. It scores 1312 with 1421 words, not an exact
match. The new range, absent from the 1186 baseline, explains that spill.

### Check generated store boundaries

A source assignment after a branch does not prove a late machine store.
Effect's separate late S-step carrier (4726) still emits two early stores:
the final scalar copy is forwarded away. A one-element addressable S-step
home retains a single late SP+0x200 store and the 736-byte/1374-word shape,
but the tested sprite-pointer carrier uses V1 instead of target V0 and
scores 5510. Actual neighbor propagation identifies the sprite-index range
as that carrier's V0 blocker. Neither representation is an integration
candidate. Integer traces are passive-equal to normal objects; reduced
evidence and source/object hashes are in `step-causal-{edges,probes}.json`.
