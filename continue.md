# Continue — remaining two assembly fallbacks

Migration commands and private transfer details: `docs/server-migration.md`. Its
`production-checkpoint.tar.gz` contains read-only historical pre-format source metadata;
never overlay it onto production source. The paused checkpoint is authoritative.
Matching requires an explicit request to resume.

## Restored development environment — 2026-10-04

Ubuntu 26.04.1 LTS, x86-64. Restoration and exact-ROM verification are complete;
matching remains paused. Checkout: `hermes/final-stretch` at migration checkpoint
`adfb1ceb2396dd0f489ae38f5aaa7a6a7af5a6c8`, clean after setup. Earlier
dirty-worktree/no-commit statements are historical.

- Public research and private migration state were restored in runbook order.
  All eight private package checksums passed. Staged/restored comparisons found
  no missing paths or content differences outside intentionally excluded metadata.
  `AGENTS.md` is restored and ignored; read it first and never commit it.
- Local Git protections are installed. The refs bundle was verified, not
  imported over branches or anchors. Historical production sources were not overlaid.
- Normal IDO 7.1/5.3 and asm-processor are configured. MIPS binutils 2.42,
  Ninja 1.13.2, pigment64 0.6.3, uv 0.12.13, and Rust/cargo 1.98.1 are available.
  Frozen Python synchronization uses Python 3.13.13; `uv.lock` is unchanged.
- `tools/server-bootstrap.sh` and `sha1sum -c checksum.sha1` passed after private
  restoration. The linker used generated `build/pokemonsnap.undefined_syms.txt`.
  Verifies the ASM-backed ROM, not either outstanding renderer C match.
- Host build tools are user-installed under `~/.local/share/pokemonsnap-host`
  with launchers in `~/.local/bin`. For direct archived binutils invocation,
  include both its `usr/lib/x86_64-linux-gnu` directory and
  `$HOME/.local/share/pokemonsnap-host/usr/lib/x86_64-linux-gnu` in
  `LD_LIBRARY_PATH`; the latter supplies `libsframe.so.1`. Preserve archived tools.
  The 50 known broken binutils documentation/manpage links are unchanged.
- Clang-format 21.1.8 and IDE extras were not installed. Inspect live Format CI
  before installing a formatter; do not bulk-reformat compiler-sensitive source.
- Graft was available but had no usable graph during setup. Try `graft map`,
  then `graft ask "<question>" --source` before source exploration; if unavailable,
  use targeted repository searches rather than making graph setup a prerequisite.

An explicit resume request authorizes matching only—not commits or pushes. Start with
“Current paused checkpoint” and “Next action — only when asked to resume”; older
sections are historical. Do not reopen the exact window match, reset/clean, or use the
production-object scorer without its repair procedure.

## Current paused checkpoint

**Paused at the user's request; do not resume automatically.** The matching goal was
stopped, not completed. No research/compiler jobs are running. Window: exact C.
Renderers: two ASM fallbacks; no candidate integrated. No commits or pushes were made
during matching.

| Normal IDO baseline | Raw / canonical score | Frame / words |
| --- | --- | --- |
| `func_8009E3D0` | 1186 / 1176 | 752 / 1420 |
| `fx_draw` | 3472 / 3462 | 736 / 1374 |

Canonical normalization removes only the proven sprite-bank symbol alias. Best sources
are unchanged. Formatting changed only the guarded renderers' text hashes; originals
remain in the migration runbook's historical `production-checkpoint.tar.gz`, not a
production-source overlay.
**`nonmatchings/orchestrated-202606/resume-20261002/pause-checkpoint.json`** records
exact paths/hashes, measured experiments, compiler arguments, trace proofs, and source
eligibility. It supersedes all historical status/next-action claims below. Preserve
current worktree changes and all private searches.

**Last action:** Closed all ten queued private trials through normal IDO and complete
canonical comparisons. Neither best score improved. Address-only controls fold to both
baselines; particle inline reciprocals also fold to its baseline. Effect inline
reciprocal/radius variants score 3693/736/1374 and emit the reciprocal into F16 with W
at F18, not the target F2/F20. Integer-address and byte-offset cursor trials also fail
the complete match.

**Paused-checkpoint production verification:**
`ninja build/pokemonsnap.ok && sha1sum -c checksum.sha1` reported
`ninja: no work to do.` and `build/pokemonsnap.z64: OK`. All nine recorded
production-source/compiler/scorer hashes remain unchanged. Verifies the preserved
ASM-backed ROM, not either renderer C match. The complete window object comparison also
freshly reports `CURRENT (0)`; its log is
`resume-20261002/verification/pause-window.diff` under the research root. Use the
scorer's dependency-isolated `DIFF` command and `-o` object mode, not plain
`python3 diff.py`; exact arguments are saved in the checkpoint.

### Next action — only when asked to resume

Read the checkpoint JSON and
`nonmatchings/orchestrated-202606/resume-20261002/cfe-source-mapping/source-associations-v3.json`
for current source-to-frontend identity bindings. Trace the controlled effect CSE
counterfactual
`resume-20261002/quotient-expression-control/effect-inline-division-radius-scalar.c`
with existing private `resume-20261002/allocator-full-location/ido7.1/cc`, sequence
**20**. Both paths are relative to `nonmatchings/orchestrated-202606/`. Use checkpoint
compiler arguments/environment and a new private output. Before interpreting range or
assignment endpoints, prove whole-TU `.text` equality with its normal measurement.
Investigate the actual W/raw-X/DIV lifetime change; F2 coloring alone is not an exact
match.

### Corrections that must survive the handoff

- Full-location tracing disproves the old particle raw-X label: bit 323 at
  Mmt/block 377/home -256 is **bottom**, not raw X. W is -184, inverse -240.
  Scalarized matrix fields inside 64-byte frontend aggregates are not new
  generated source locals. Never infer names from bare addresses.
- Effect's baseline already has W at F20. DIV expression bit 212 is globally
  F2, but its scalar destination bit 210/home -292 is F18; BB26 assignment
  endpoints explain why normal output still emits the division into F18.
- `uoptinput`'s `curblk` is the lexical procedure block, **not** a CFG basic
  block. Adding a branch does not change that forwarding condition.
- The one-field inverse struct is byte-identical to baseline and retains
  vreg=1/veqv=0. The one-element array creates an indirect store and a reciprocal
  spill at SP+0x1BC; it is not a fix.
- Decoder **v3**, SHA `2f5cbe12655de2fed487d553e092a8258dc16ab2b0fdb265c5c509b2089d879c`,
  passed four complete captured streams, actual Udup/Uswp/indexed-store graph
  checks, and malformed-input rejection. Ten source-statement associations
  were independently verified. Earlier v1/v2 node IDs are historical.
- The j-carrier S-step experiment reproduces target MFC1 V0 → A1, mirror,
  and single late-store scheduling, but stores at SP+0x23C rather than
  SP+0x200 and has five extra words. Do not add another quotient/carrier.

**Do not:** Integrate a nonzero score; accept register-forced objects; rerun known
negative controls blindly; delete root scratch or `nonmatchings/`; reset/clean this
worktree; reopen the exact window match; or restore linking against raw
`undefined_syms_auto.txt`. Scoring must use distinct canonical
`expected/<private-object-path>` references. If the production-object scorer is used,
repair its overwritten objects and checksum stamp before trusting the ROM. Workers must
leave compiler/probe/build gates to the integration owner.

## Historical blocker audit — superseded by current checkpoint above

**Scorer correction:** An absolute `diff.py -f` path previously bypassed `expected/` and
compared an object with itself. The same object scored 14835 with a relative path and 0
with an absolute path. This is not caused by a missing relative reference. `score()` now
normalizes paths under the repository and rejects missing or aliased references before
compilation. Real normal-IDO scoring with absolute paths now returns 1186/3472, agreeing
with direct relative-path comparisons. Missing-reference and hardlink-alias checks
rejected the operation without writing the output/reference. `diff.py` now fixes that
reference resolution directly and bounds object disassembly to the named symbol,
including all its returns. A smoke pair whose only difference was in the next function
changed from score 5 to 0; a difference after the selected function's first return still
scored 5. Complete renderer comparisons remain 1186/3472 through both path forms. The
bounded diffs are `{particle,effect}-symbol-bounded.diff`.

**Trace correction:** The old particle bit 278/home -248 was `clipZ`, not `invW`. Fresh
1186 tracing identifies the quotient as DIV expression bit 1086, global color 30 /
emitted F20; `invW` at -240 has no global range. `particle-passive.stderr` in the audit
directory emits all 17136 bytes of the normal TU `.text` identically. The old bit-278
mask is not reciprocal interference evidence.

**Allocator restored and verified:** Audit directory contents: pinned IDO runtime
inputs, `build_allocator_probe.py`, `make_allocator_probe.py`, and
`allocator-propagation-toolchain/cc`. Passive compiler reproduces all 17136/21760 normal
particle/effect TU `.text` bytes exactly. Source/toolchain hashes:
`allocator-propagation-equivalence.json`. The initial `f_updateforbidden` hook missed
later inline colored-neighbor propagation; the current probe covers both global-color
propagation arms.

**Actual F2 conflicts:** In particle, size bit 310/home -168 takes F2 at priority 50 and
explicitly forbids F2 for DIV bit 1086. The quotient later gets F20 at priority 16.6667.
In effect, DIV bit 212 takes F2 at priority 5000 and explicitly forbids F2 for its own
destination variable, bit 210/ home -292, which later gets F18 at priority 1666.67.
Effect's final division still emits into the destination's F18 rather than the
expression's F2. Actual graph edges, not overlap inferred from shared basic blocks.
Reduced evidence: `reciprocal-causal-edges.json`; full logs:
`{particle,effect}-propagation.stderr`.

Priority denominator at range+28: compressed live-unit/bitvector count, not declaration
order or neighbor count. `f_compute_save` computes the savings priority at range+48.
Only `phase=priority-selected` reports a computed priority; initial
`phase=mask-initialized` values there are not meaningful. `coloroffset` is the raw
architectural register index; do not interpret its shifted `mapped_slot` diagnostic as a
hardware FPR.

Latest counterfactuals/agent results: `pause-checkpoint.json`. Getting F2 alone is
insufficient: check W/raw-X lifetimes, the radius carrier, spills, and the entire
instruction sequence. Keep normal-IDO whole-function scores as the gate, and prove
passive `.text` equivalence for each candidate whose trace is interpreted.

**Controlled lifetime results:** Fused particle size scaling emits the desired F2
division but introduces an inverse scalar range and SP+0x200 spill: 1312/752/1421
(score/frame/words). A separate raw-size value leaves the inverse home-free but still
F20: 4022/768/1424. Paired position/corner reuse on the current parent gives
3350/760/1423; moving radius calculation before culling gives F2 but 8269/760/1420. None
qualifies for integration.

Effect's existing 3481 control demonstrably removes the DIV expression's global range;
the inverse variable itself then gets F2. Further causal splits are under
`effect-quotient-conflict/`, not new best sources. Best complete normal score: 3472.

**Integer store correction:** The old late S-step scalar carrier (4726) has no late
machine store: IDO forwards the copy away and keeps two early stores. The new integer
probe (`allocator-integer-toolchain/cc`, `IDO_TRACE_ALLOC_CLASS=1`) reproduces the
normal effect TU text exactly. An addressable one-element S-step home with a
sprite-pointer carrier gives one late SP+0x200 store and 736/1374 frame/words, but uses
V1 instead of V0 and scores 5510. The sprite index is the actual V0-forbidding neighbor.
Reusing a packed widened sprite index instead changes the late store to A0
(5465/736/1374), not target V0. See `step-causal-{edges,probes}.json` and
`additional-causal-probes.json`; no renderer source integrated.

Latest isolation smoke: `ninja build/pokemonsnap.ok && sha1sum -c checksum.sha1`
reported no work and `build/pokemonsnap.z64: OK`. Verifies the unchanged ASM-backed
production build, not either C match.

**Do not:** Integrate a nonzero-score C candidate, trust a scratch score without its
`expected/<private-object-path>` reference, delete the untracked `nonmatchings/`
research, or use `hermes/final-stretch` as an upstream PR base. Research history and
scratch paths follow.

## Branch and ownership

Work on `hermes/final-stretch`, the cumulative private branch. It is not an upstream PR
base. No commits were made for this historical checkpoint. The worktree was
intentionally dirty at the checkpoint. Any dirty-worktree assertions below describe that
historical state only; after migration commits, they are not instructions to manufacture
dirty state. Historical checkpoint changes: scorer/reference safety, symbol-bounded
`diff.py`, handoff/matching notes, and untracked diagnostics. Production renderer source
was unchanged. Do not reset or clean the worktree.

## Historical renderer status and experiment ledger

The window function `func_80374714_847EC4` remains integrated as exact C. Both renderer
fallbacks remain **non-exact**; neither integrated. Latest research changed no renderer
source, normal compiler, or build configuration. No commits or pushes were made.

| Candidate | Full-function diff score | Frame | Words |
|---|---:|---:|---:|
| `func_8009E3D0`: `nonmatchings/orchestrated-202606/particle/improved-effects-decl-grid/best-1186.c` | 1186 | 752 | 1420 |
| `fx_draw`: `nonmatchings/orchestrated-202606/effect/projection-range-next/001-reversed-product-operands/candidate.c` | 3472 | 736 | 1374 |

Both configured-IDO candidates preserve target instruction counts and frame sizes;
whole-function diffs still differ in register allocation, stack slots, and scheduling.
Do not integrate either nonzero-score source. The new particle best retains the X/Z/Y
position initialization and projection expression structure from the earlier 1228
source, but keeps an `effects` pointer separate from the parameter and uses a single
TLUT pointer with a compiler-sensitive self-assignment in the cache comparison. This
aligns the target's s3-effects/s2-matrix roles and reproduces the 752-byte frame without
the later TLUT diff explosion of the bare single-pointer form. Declaring `effects`
directly before `tlutData` shifts the sprite-data stack slot from 0x1e0 to 0x1dc,
reducing score from 1198 to 1186. Complete aligned diff:
`particle/improved-effects-decl-grid/last-diff.txt` beneath
`nonmatchings/orchestrated-202606/`.

The 36-source particle TLUT/declaration/product grid scored no better than 1518; the
17-source effect S-step/list-pointer/inverse-product grid scored no better than 3472.
Later single-variable probes of position register qualifiers, loop-index scope,
reciprocal scope, projection normalization order, and explicit constant lifetime did not
beat 1228. Effect projection addition order, declaration placement, pointer scheduling,
sequential accumulation, and reciprocal assignment-expression probes did not beat 3472.
Generators and manifests: each renderer's `systematic-final-search/` directory.

Historical passive UOPT traces:
`global-colour-trace/{particle-1228-passive,effect-3472-passive}/compile.stderr` beneath
`nonmatchings/orchestrated-202606/`. Their `.text` matched corresponding normal IDO
builds byte-for-byte. In the **3472** effect candidate, inverse variable bit 210/address
-292 has a global range (BB26 and BB34); its division is bit 212, and normalized-X is
bit 214 without a pre-global range. The earlier bit-219 normalized-X trace belongs only
to a different **4010** diagnostic TU. The particle **1228** trace was misread: bit
278/address -248 is `clipZ`; its quotient is DIV bit 1089, color 30. The current
**1186** trace in `blocker-audit-20260927/particle-passive.stderr` instead has DIV bit
1086, color 30, and `clipZ` bit 280/color 32. In both, `invW` at -240 has no global
range. Source-specific identities, not original C.

Follow-up planner probes improved neither score. Particle position union/array overlays
scored at least 15516, immutable per-effect position initializers at least 1508, split
conversion/scale assignments at least 1718, and reciprocal-multiplication operand
reversals 1228 or 1233. `register` on the effect inverse emitted byte-identical text;
reversing Y/Z normalization products retained 3472. Generated C sources:
`particle/{position-union-identity,immutable-position-locals,position-split-assignment,normalized-product-order}/`
and `effect/{projection-range-next/002-register-inverse,normalization-operand-grid}/`
under the same research root.

Staging any one of the four particle projection components as successive same-order `+=`
assignments instead of one expression scored 2110–3618; see
`particle/staged-projection/`. Reversing binary operands in the later viewport additions
emitted an identical 1228-score object.

Separating raw particle X from normalized X scores 3190; adding an erased inverse
observation and/or clearing W scores at least 4448. Retaining a named 0.125 position
scale with later erased uses scores at least 1632. See
`particle/{separate-raw-X-best,late-position-scale-identity}/`; neither produced the
target saved-FPR web.

Moving the existing erased `if (!proj)` guard across position assignments and the W
guard scored 1228–3014. Erased X/Y/Z Boolean observations scored at least 1243; comma
observations emitted the unchanged 1228 object. Moving the existing erased camera-value
observation scored 1228 or 1288, while deleting it scored 5768. See
`particle/{erased-proj-boundary,erased-position-boundary,erased-camera-boundary}/`.

Another effect diagnostic combines an explicit `rawX` home, clearing W after its
reciprocal, and an erased reciprocal observation: `effect/combined-rawx-lifetime/111.c`
scores 3486. Reversing its normalization multiply to `rawX * temp_f2` scores 3481. It
obtains the target inverse F2 but keeps raw X in F20 and emits an extra F18→F12 move;
neither is an integration candidate. Delaying the converted integer S-step stack
assignment until after the S or T mirror branch scores 4726, not 3472.

On the 3481 diagnostic, erased inverse `||` and `(temp_f2 && rawX)` observations
retained 3481; other reciprocal observations or normalized-X observations scored at
least 3651 or 6809. `register` hints on the integer S/T steps left the 3472 object score
unchanged. Boundary probes:
`effect/{erased-inverse-boundary,normalized-x-boundary,step-register-hint}/`.

Historical observational color traces:
`global-colour-trace/{effect-bit210-color,particle-color-decision}/compile.stderr`. They
correspond to effect 3472 and particle 1228, not particle 1186. Their reported `.text`
equality does not establish correct trace interpretation. Effect raw-X/normalized-X
carrier bit 179/address -176 selected internal color 26 (physical F12): primary colors
24–25 were forbidden, while 26 was the first available. The inverse bit 210 selected
color 29 (physical F18): colors 24–28 were forbidden, and 29 was first available. Target
uses raw X in F18 and the inverse in F2. Both bits' passive color-candidate records are
in `effect-bit210-color/compile.stderr`; the target's original C/UOPT graph is unknown,
so target colors and interfering ranges cannot be inferred from its assembly alone.
Shared-live-basic-block reports are **not** by themselves interference proof. Particle
`clipZ` bit 278 selected color 32 with forbidden low mask `0x000000ff`; the old claim
that this described reciprocal F20 was incorrect. The actual quotient was DIV bit
1089/color 30. Internal colors are not hardware registers.

Passive UGEN store tracing corrected the original probe's operand mapping: `f_emit_rob`
receives the effective stack displacement in `a2`, not its fifth argument. Isolated
tracer: `global-colour-trace/step-ra-probe/ugen-step-ra-passive.c` compiled the best
3472 effect TU with byte-identical normal-IDO `.text`; its `trace.stderr` records
`sw v0,0x200(sp)` and `sw t7,0x200(sp)` as opcode 87, base 29, offsets 512 (emission
indices 499 and 516). The earlier UOPT endpoint trace on a related 3477 source assigns
integer S-step IChain bit 358/address -224 in BB44 and BB45; the frame maps 736 - 224 =
512. Supports source-local mutation/lifetime, not proven cross-run pointer identity.
Corrected probe details:
`global-colour-trace/step-emission-probe/emission-hook-spec.txt`.

Further normal-IDO negatives: changing particle position initialization X/Z/Y to X/Y/Z
fixed the two halfword load-order differences but scored 1238; declaring a separate
`effects` pointer scored 1698 with an 8-byte larger frame. All 36 position declaration
group placements/orderings scored at least 1228. Deferring effect mirror-S scaling to
the final texture-rectangle consumer scored 14958. On the 3481 effect raw-X diagnostic,
20 placements of erased W-clearing/inverse observation scored at least 3481; nine raw-X
declaration positions scored at least 3481. Scratch results:
`particle/{coordinate-order-probe,effects-pointer-probe,declaration-placement-probe}/`
and
`effect/{late-mirror-factor-probe,erased-boundary-order-probe,rawx-declaration-probe}/`.
Production objects were restored and `sha1sum -c checksum.sha1` reported
`build/pokemonsnap.z64: OK` afterward. Neither renderer C integrated.

Additional compiler negatives: particle `register invW` compiled to an identical 1228
object; assigning the reciprocal inside the first multiply scored 1760. Moving the
`cameraValue` union declaration among eight positions scored at least 1228; swapping the
two matrix declarations moved their stack homes away from the target and scored 2646.
The separate `effects` pointer plus removal of `selectedTLUT` restored the 752-byte
frame and target s3-effects/s2-projection GPR roles but scored 3131 due later TLUT
codegen. Effect reuse of later `var_f18` as raw X scored 3603; a `register` raw-X hint
retained 3481, and nine list-cursor declaration placements scored at least 3472.
Scratch:
`particle/{inverse-register-probe,reciprocal-assignment-probe,union-declaration-probe,matrix-declaration-probe,effects-pointer-probe}/`
and
`effect/{rawx-varf18-reuse,rawx-register-probe,list-cursor-declaration-probe,nonzero-w-arm-probe}/`.
Explicitly nesting the effect reciprocal in a nonzero-W arm scored 4376. Both production
renderer objects restored; full ROM checksum passed after scoring.

Complete 8-axis projection addition-operand grid, previously untested higher-order
combinations: 219 additional particle sources (3–8 reversed operands) scored no better
than 1265. Together with the existing singleton/pairwise scores,
`particle/full-projection-operand-grid/results.txt` rules out this isolated syntax
family as the missing match. Production objects restored; ROM checksum passed after the
grid.

Combining the corrected X/Y/Z halfword initialization order with all 256 projection
operand masks also scored no better than 1238; see
`particle/xyz-projection-operand-grid/results.txt`. Production object and ROM checksum
restored afterward.

The subsequent particle pointer/TLUT identity probe improved the score to **1198**
without changing the 752-byte frame or 1420-word function. A separate `effects` local
restores target s3/s2 assignments, while replacing the two TLUT locals with a single
`tlutData` plus `loadedTLUT != (tlutData = tlutData)` recovers most of the target TLUT
scheduling. The self-assignment must not be removed without rechecking the object:
omitting it scored 3131. Full aligned diff:
`particle/effects-pointer-probe/self-assign-diff.txt`. All 63 tested six-self-assignment
combinations at position/W/inverse boundaries scored at least 1198; see
`particle/self-identity-grid/results.txt`. The best source was independently
scratch-compiled; `nm` reported 0x1630 bytes (1420 instructions), with
`addiu sp,sp,-752`. Production was restored and the full ROM checksum passed after the
tests.

Best particle score at this stage: **1186**. changing only the `effects` declaration
position from the 1198 source moved the sprite-data stack home to 0x1dc, toward target
0x1d8; see
`particle/improved-effects-decl-grid/{best-1186.c,last-diff.txt,results.txt}`. Moving
`tlutData` or the loop-index declaration independently did not beat 1186 (the loop-index
width `u16` scored 8291). The 127 further projection operand masks on the 1198 source
scored at least 1230, and seven erased effects-pointer self-assignments remained 1198.
The 1186 source independently compiled to a 0x1630-byte function with a 752-byte frame.
Scorer-overwritten objects were restored; `sha1sum -c checksum.sha1` reported
`build/pokemonsnap.z64: OK`.

Subsequent isolated negatives against 1186: 14 positions of `textureData`, 63 embedded
scalar self-assignments, and five adjacent viewport assignment swaps scored at least
1186; removing `volatile` from `vpTransZ` scored 1776. Effect S-step self-assignment and
rewriting `*= 2` as `<<= 1` both retained 3472, as did self-assigning its list-base
pointer at the first index. See
`particle/{improved-texture-decl-grid,expression-identity-grid,viewport-order-probe,viewport-z-qualifier-probe}/`
and `effect/{step-self-assignment-probe,list-base-self-probe}/`. Production objects
rebuilt; ROM checksum passed after each scoring batch.

Additional effect reciprocal probes kept the named inverse but inlined its pre-clip uses
individually or in all seven combinations; each compiled to an identical 3472-score
object. Moving inverse among 14 other local declarations scored at least 3520. Inlining
the inverse again after the clip branch instead scored 3703, so the compiler does not
retain the pre-branch value across that boundary without a named local. Replacing the 12
X/W/Z projection matrix coefficients individually with named-union field access scored
either 3472 or worse (3997–4287); translation-column fields are compiler-sensitive. See
`effect/{inline-reciprocal-probe,selective-inline-reciprocal,inverse-declaration-grid,matrix-field-probe}/`.
The particle loop-index types `u32` and `register s32` retained 1186; `volatile s32`,
`volatile u32`, and `s64` were substantially worse. See `particle/index-type-probe/`.
Production objects rebuilt; ROM checksum passed after these probes.

An unused scalar local, whether volatile or not, disrupted particle stack allocation
(scores 1502–1674), so reserving an artificial home does not fix the loop index at
SP+0x94 versus target SP+0x98. All 36 combinations of position declaration/evaluation
orders scored at least 1186: declaration order is inert; X/Z/Y initialization remains
lowest. Reversing the `projScaleX`/`projScaleY` declarations scored 1194, while
reversing their initialization scored 2149 or 2141. See
`particle/{volatile-unused-home-probe,position-declaration-assignment-grid,projection-scale-priority-probe}/`.
Production object rebuilt; checksum verified after each batch.

Read-only allocation analysis identified a possible phase reuse of target F24 for
position scale and later normalized Z. Explicitly assigning 0.125 to the existing
`clipZ` and using it for all three position conversions in each of six orders matched
scores of corresponding literal-constant orders (minimum 1186): IDO folded the carrier.
Removing named scaled positions and repeating their expressions scored 4938. See
`particle/{clipz-scale-reuse,inline-scaled-components.c}`.

Effect `rawX` split from normalized `temp_f12`, followed by a destructive multiply,
scored 7331 when function-scoped or 7167 when declared at the start of the particle loop
(both much worse than 3472); switching the multiplication operand order did not help.
See `effect/rawx-destructive-normalization/`. Compiler rejects scalar declarations after
executable loop statements; those placements are unscored. All scorer-overwritten
objects rebuilt; full ROM checksum passed.

The three then-unscored `effect/step-use-next/` full-TU S-step-boundary candidates
compiled with IDO: branch-local assignment scored 7482, integer factor before the single
definition scored 4985, and conditional pre-scaled definition scored 6805. Their parent
is the older 3477 candidate; none approaches the current 3472 best. Production was
rebuilt and the ROM checksum passed after this batch.

Fourteen particle load/scale split variants (each nonempty subset of X/Y/Z, both X/Y/Z
and X/Z/Y initial load orders) scored 1841–2612: assigning raw coordinates first and
scaling later does not reproduce the target's FPR register web. See
`particle/position-preload-grid/results.txt`. Production renderer rebuilt; ROM checksum
passed afterward.

Particle `effects` pointer qualifiers: `register`, const-qualified data, and a
const-qualified initialized pointer all retained 1186; volatile-qualified data scored
5929. See `particle/effects-pointer-qualifier-probe/results.txt`; production was rebuilt
and the ROM checksum passed.

An unused 4-byte scalar inserted before the view matrix shifts the entire particle stack
frame to 760 bytes, including both matrix bases, so it cannot supply the missing index
slot at 0x98. Fifteen unused locals of three scalar types placed across the existing
inner block all scored 1686, not 1186. See
`particle/{volatile-unused-home-probe/pad-before-view-diff.txt,inner-unused-slot-probe/results.txt}`.
Production rebuilt; ROM checksum passed.

All seven nonempty `register`-qualifier subsets for particle `posX/posY/posZ` compiled
to the unchanged 1186 score. IDO also produced identical scores for all 18 combinations
of the six coordinate assignment orders and one/two same-line statement boundaries; the
compiler-sensitive window match's same-line issue does not apply to these plain
assignments. See
`particle/{position-register-combined,position-sameline-grid}/results.txt`. Production
rebuilt; ROM checksum passed after both batches.

Reordering the three independent effect per-camera initializers (`var_s2`, `camObj`, and
`sp1C8`) across all five nonbaseline permutations scored 3472 or 3496, leaving the early
instruction scheduling mismatch unresolved. See
`effect/camera-prologue-order-grid/results.txt`; production was rebuilt and the ROM
checksum passed.

A 36-way particle cross-grid combined three position-load orders, six `effects`
declaration placements, and both projection-scale declaration orders. Scores were
additive; the minimum remained 1186. Correct X/Y/Z source evaluation order still scores
at least 1196. See `particle/effects-position-scale-combination/results.txt`. Production
rebuilt; ROM checksum passed.

The related SSB64 renderer `lbParticleDrawTextures` at
`VetriTheRetri/ssb-decomp-re/src/lb/lbparticle.c` uses projection,
reciprocal normalization, clipping, and sprite geometry structurally
similar to both remaining functions; a lead, not a Pokémon Snap match. On the current particle candidate, replacing any subset of
the three `continue` statements with a shared `goto` loop-exit label
retained 1186. Inverting the first two guards into nested accepted
branches retained 1186; nesting the clip-bounds guard scored at least
1281. See `particle/{goto-loop-exit-grid,nested-guard-control-grid}/results.txt`.
Production rebuilt; ROM checksum passed.

Reusing the particle projection denominator as the reciprocal carrier scored 2770;
reusing the inverse as the later scaled radius scored 3274. Using `size` for both phases
scored 3190; reusing denominator for inverse and radius scored 5702. Unlike the related
SSB64 source's `tm` carrier, none preserves the current register/stack allocation. See
`particle/reciprocal-radius-carrier-grid/results.txt`. Production rebuilt; ROM checksum
passed.

Nested accepted-branch control flow in `fx_draw` did not improve the 3472 best: wrapping
the size-zero guard scored 7416; wrapping the clip-bounds guard scored 3997; both scored
7416. See `effect/nested-guard-control-grid/results.txt`. Effect production object
rebuilt; ROM checksum passed.

The effect camera-byte-flag pointer must retain indexed-array spelling: three equivalent
pointer-addition forms each scored 6327; an explicitly cast indexed address retained
3472. Replacing the normalized-X product with compound multiplication or an X-first
product scored 3477; self-assignment of the compound result likewise scored 3477, while
mutating the reciprocal scored 4407. See
`effect/{camera-pointer-spelling-grid,normalized-x-destructive-probe}/results.txt`.
Production rebuilt; ROM checksum passed after both batches.

An isolated decomp-permuter particle input at `particle/permuter-best1186/` now compiles
the current 1186-score candidate as one translation-unit function with the same
0x1630-byte text; exactly two word immediates differ from the full-TU object, both
jump-table `.rodata` relocation offsets (0x208 displacement from the prefix data). The
permuter's independent base score is 3631 with stack differences enabled; this is
**not** the repository's 1186 objdiff score or an integration gate. A scoped random
search targets declarations and coordinate projection only. Any interesting output must
be copied back into the full-TU source and rechecked with
`tools.score_remaining_asm.score` / `diff.py`.

An independent standalone `fx_draw` input now lives in `effect/permuter-3472/`. It
compiles the full-TU 3472 candidate as a 0x1578-byte function; only two `.rodata`
relocation addends differ from the full-TU text. Its isolated original-assembly target
assembles to the same 0x1578-byte size. Decomp-permuter reports an independent base
score of 6072 with stack differences enabled, **not** the repository objdiff score.
Search randomizes declarations and projection/rectangle geometry; transfer any
improvements back into the full effect translation unit and score the complete function
before considering integration. Whole-production-object permuter targets are invalid:
unrelated functions inflate deletion penalties.

Effect inverse-F2 diagnostic `combined-rawx-refinement/10.c`
scored 3481 with `rawX` declared next to the inverse. Moving
`rawX` to 19 other local-declaration sites scored 3529–3781;
moving the compiler-sensitive dead W clear after reciprocal
observation or a later projected component scored 3836; placing
it inside the clip rejection effectively removed it and scored
8347. See `effect/{rawx-inversef2-decl-grid,w-clear-lifetime-grid}/results.txt`.
Production objects restored; ROM checksum passed.

Moving the effect particle `dataID` load after the sprite-bank lookup retained 3472;
inlining `particle->dataID` at both sprite and palette uses scored 3512. See
`effect/data-id-loading-probe/results.txt`. Production object rebuilt; ROM checksum
passed.

Both standalone permutation searches completed (900 seconds each). Particle found no
candidate below its independent 3631 baseline. Effect found independent scores 5990 and
5852 versus its 6072 baseline, but transferring the sole substantive declaration swap
(`sp208`/`sp204`) to the complete translation unit scored 3624, worse than 3472.
Transferring either whole generated function scored 6440 or 6297: pre-expanded
display-list macros change full-TU compilation. Isolated permutations are not matches.
See `{particle/permuter-best1186,effect/permuter-3472}/output-*/source.c` and
`effect/permuter-3472/full-tu-results.txt`. Effect production object rebuilt; ROM
checksum passed after these probes.

Normal-scorer results for five previously uncompiled particle hypotheses:
`projection-refine-next/{001,002}` scored 3772 and 4926;
`frame-home-order-next/{001,002,003}` scored 1450, 4104, and 2868. None beats 1186.
Exact paths/scores: `particle/preserved-uncompiled-scores.txt`. Particle production
object rebuilt; ROM checksum passed.

All seven reversals of the independent coordinate-scale multiplication operands (`0.125f
* pos{X,Y,Z}` instead of the reverse spelling) emitted the unchanged 1186-score particle
function. See `particle/coordinate-mul-operand-grid/results.txt`. Production object
rebuilt; ROM checksum passed.

On the 1186 particle best, erased `if (posX/Y/Z) {}` observations immediately after
scaling or just before the W guard scored 1186 for X and 1206 for Y/Z; observations
after inverse computation scored 3048. See
`particle/erased-position-observation-best1186/results.txt`. Packing the three scalars
into a `Vec3f` compiled to an identical 1186-score object; a `f32[3]` array scored
15479. Scratch sources: `particle/{position-aggregate-probe,position-array-probe}.c`.
Production object rebuilt; ROM checksum passed.

Initializing `fx_draw`'s `particleLists` in its declaration or adding a `register` hint
retained the 3472-score complete object. Scratch sources:
`effect/{list-pointer-initializer-probe,list-pointer-register-probe}.c`. Effect
production object rebuilt; ROM checksum passed.

Moving any nonempty subset of particle `posX/posY/posZ` declarations from function scope
to the beginning of the effect loop scored 1274 for singletons, 1370 for pairs, and 1466
for all three, versus 1186 at function scope. See
`particle/position-loop-scope-grid/results.txt` and
`particle/position-loop-scope-probe.c`. Particle production object rebuilt; ROM checksum
passed.

Combining target X/Y/Z coordinate-load order with all eight
function-scope/effect-loop-scope subsets was additive: 1196 at function scope, 1284 for
single scoped values, 1380 for pairs, and 1476 for all three. See
`particle/target-load-order-scope-combination/results.txt`. Introducing signed-halfword
`rawX/rawY/rawZ` carriers before scaling scored 4724 (X/Y/Z) or 4699 (X/Z/Y); those
scratch candidates are `particle/raw-halfword-scaled-{XYZ,XZY}.c`. Production object
rebuilt; ROM checksum passed.

A `register f32` hint on `fx_draw`'s reciprocal `temp_f2` retained 3472; see
`effect/inverse-register-probe.c`. Effect production object rebuilt; ROM checksum
passed.

Adjacent swaps of 34 simple particle declarations scored no lower than 1186; matrix
swaps worsened to 1639/2604. Moving the `i` declaration across matrix and union
boundaries scored 1186–1348. See
`particle/{adjacent-local-declarations-grid,loop-index-wide-declaration-placement}/results.txt`.
The prior unscored effect S-step branches scored 7625 and 7477
(`effect/step-branch-next/scores.txt`). Five mirror-S assignment and self-assignment
forms emitted the unchanged 3472 (`effect/mirror-step-identity-grid/results.txt`). Both
renderer production objects rebuilt; ROM checksum passed.

Narrowing the particle loop index to the surrounding local block scored 1274, and the
palette `tlutData` to outer/inner loops scored 1366/1414; all combinations were worse
(1430/1490). See `particle/index-tlut-loop-scope-grid/results.txt`. For effect, widening
the dead `dataID` sprite index from u8 to s32 or u32 scored 3816; reusing the widened
s32 as the S-step carrier scored 3874. Reusing widened later alpha locals instead scored
5687 or 4977. See
`effect/{step-dataid-reuse,wide-only-dataid,unsigned-wide-only-dataid}.c` and
`effect/step-alpha-carrier-reuse/results.txt`. Production objects rebuilt; ROM checksum
passed.

Wrapping particle `proj` in a 64-byte matrix/bits union emitted an identical 1186-score
function; 18 erased integer observations of selected coefficients at three boundaries
scored 1774–3747, not a source of the target FPR allocation. See
`particle/projection-union-bit-observation/results.txt`. Particle production object
rebuilt; ROM checksum passed.

Private full-TU scoring requires `expected/<repository-relative private object path>` to
hold the original configured reference object. An absolute `diff.py -f` path previously
discarded the `expected/` prefix and compared the candidate with itself, reporting false
zero; object-mode reference resolution is now fixed directly. A missing **relative**
reference fails; the earlier attribution to a missing mirror was incorrect.
`tools/score_remaining_asm.py` now normalizes its object argument to a
repository-relative path and rejects missing or aliased references before compilation.
Baselines 1186/3472 were reverified from their exact source files in
`blocker-audit-20260927/`. Use private paths so scoring does not overwrite production
objects.

While two task agents ran concurrently, the parent persistent Python Eval global `root`
unexpectedly changed from the repository root to
`nonmatchings/orchestrated-202606/particle/induction-shape-probe` (reported to
`xd://report_issue`). The parent restored it from `Path(mod.ROOT)`. Six subsequent
scratch suites landed under that nested
`particle/induction-shape-probe/nonmatchings/orchestrated-202606/` prefix; their
explicit links below reflect the real paths. Confirm `root == Path(mod.ROOT)` before
generating more scratch candidates.

Reordering the three `fx_draw` projection-union members across all six permutations
emitted the unchanged score 3472. Permuting the names of its three Y coefficients while
preserving field offsets also emitted 3472.
`effect/{matrix-union-member-order-grid,named-column-field-identity-grid}/results.txt`.
Eight S-step declaration placements scored 3492–4511; see
`effect/step-declaration-boundary-grid/results.txt`. Private expected-object references
were present for every scratch score.

Reusing `gobj` itself as the photo-effect pointer moved the particle loop counter to
target SP+0x98 but swapped the saved effect/matrix GPRs and shifted the sprite-data home
to SP+0x1e0, scoring 1220. Reintroducing a separate effects alias in 13 declaration
positions scored 3950–4196; an erased gobj alias on the 1186 best was inert. Fusing the
effects definition into its declaration was also inert. Scratch sources:
`particle/{gobj-as-effect-pointer-probe,gobj-plus-effects-decl-grid,input-alias-erased-boundary,effects-declaration-initializer}.c`.
Moving the compiler-sensitive erased `if (!proj)` guard across seven position/projection
boundaries scored 1186–2972; see
`particle/projection-guard-position-best1186/results.txt`.

Swapping the position load order to X/Y/Z on the `gobj` alias source scored 1230, still
worse than 1186 (`particle/gobj-alias-xyz-position.c`). Moving the erased `if (sp220);`
across seven locations in `fx_draw` emitted an identical 3472-score object every time
(`effect/first-norm-erased-guard-positions/results.txt`).

Using a two-field union for the effect-base pointer (either member order, including
cross-member read) was byte-identical to the 1186 candidate;
`particle/effect-pointer-union-representation/results.txt`. All six permutations of the
three initial `fx_draw` camera statements scored 3472 or 3496
(`effect/camera-start-initializer-order/results.txt`). Keeping a separate integer S-step
carrier through the mirror branch and storing it afterward scored 4726
(`effect/step-single-late-carrier.c`), reproducing the previously negative late-store
result rather than fixing the early spill.

Swapping 44 eligible adjacent `fx_draw` scalar/pointer declarations scored no better
than 3472; see `effect/adjacent-local-declarations-best3472/results.txt`. Matrix/union
blocks were intentionally excluded to avoid invalid C.

Reusing `fx_draw`'s later `var_f18`/`var_f16` as raw projected X on the 3472 parent
scored 8648. Reusing `temp_f14` or `temp_f28` was inert at 3472; see
`effect/rawx-{varf18-best3472,dead-float-carrier-grid}`. Naming X/W/Z projection
coefficients through the same matrix union was inert except that naming the
first-normalization coefficient Z raised the score to 4027–4059; see
`particle/induction-shape-probe/nonmatchings/orchestrated-202606/effect/{all-projection-named-field-aliases,first-norm-partial-named-coefficients}/results.txt`.

Four particle `i` induction spellings (condition, increment, and initialization
placement) emitted the unchanged 1186-score objects. Pointer induction through
`cameraValue.value` scored 8882; see `particle/induction-shape-probe/`. No register/home
improvement.

All seven subsets of commuted scalar/0.125 position products in the particle candidate
emitted an identical 1186 score. Dividing X by 8.0 instead scored 1996; see
`particle/induction-shape-probe/nonmatchings/orchestrated-202606/particle/{position-scale-operand-order,position-x-divide-by-eight.c}`.

Narrowing `fx_draw`'s list cursor to the per-list loop preserved the 736-byte frame /
1374-word function but scored 3967. Its cursor home remained SP+0x94 versus target
SP+0xb4, inverse and raw-X registers remained wrong, and its S-step still had two early
stores. Candidate: `effect/lexical-list-cursor-scope-20260926/candidate.c`.

**Reference-object symbol caveat:** `expected/build/src/app_render/{47380.c,effect.c}.o`
references undefined `D_800BE288` for the sprite-bank array, while production C defines
`fx_SpriteBanks` at address 0x800BE288 and the tracked assembly refers to that latter
name. Replacing only the one candidate function's array reference with an `extern`
declaration of `D_800BE288` reduced its private score by exactly 10, to 1176/3462,
without resolving register/stack mismatches; see
`particle/induction-shape-probe/nonmatchings/orchestrated-202606/{particle,effect}/legacy-sprite-bank-relocation.c`.
This is relocation-naming noise, **not** a match or an integrable source change: the
production linker currently has no `D_800BE288` definition.

The blocker audit now supplies separate canonical references in
`expected/nonmatchings/orchestrated-202606/blocker-audit-20260927/canonical-reference/{particle,effect}.o`,
created with `objcopy --redefine-sym D_800BE288=fx_SpriteBanks`. Original references
untouched. The single alias preserves all allocated bytes and relocation meanings; see
`blocker-audit-20260927/canonical-reference/manifest.json`. Normal best sources score
1176/3462 there, with no machine-code improvement. Identical canonical controls score
zero; an unrelated array symbol still scores 10. Keep historical 1186/3472 scores
labeled as raw; use canonical references to judge a future zero, then verify complete
normal-IDO code and the final linked ROM checksum.

A planner proposed a same-sized integer/float union to carry the 0.125f bits and later
reuse the float view for normalized Z, hoping to recreate target F24 phase reuse. The
normal private scorer returned 14835 instead of 1186
(`particle/scale-to-clipz-union-probe.c`); the type-pun did not preserve the matching
code shape.

Another planner proposed changing `fx_draw`'s 16-list indexed loop to a post-tested
pointer cursor incremented at the end of each list. Normal private scoring gave 6874
with dead `j` retained, 6678 with it removed; neither approached the 3472 parent
(`effect/posttested-list-cursor-planner/results.txt`).

The particle stack agent scoped sprite/index carriers in three ways; all retained the
752-byte frame and target s2-matrix/s3-effects roles, but scored 1274–1370 and kept `i`
at SP+0x94. See `particle/stack-repack-frontier/`. The effect integer agent found
pointer-cursor forms that *do* move the list home to target SP+0xb4, but they scored
5834–7933, emitted 1376–1377 words instead of 1374, or shrank the frame to 728 bytes.
See `effect/step-cursor-frontier/`. A manually expressed `goto` backedge scored the same
6678 as the post-tested cursor (`effect/cursor-backedge-target-shape/goto-loop.c`).
Cursor representation alone is insufficient.

The effect float-web agent tried a raw/normalized-X float/integer union and a `register`
hint on raw X; both emitted byte-identical 3472-score `.text`
(`effect/float-web-frontier/results.txt`). The particle float-web agent tested
projection assignment order, scalar position struct, repeated reciprocal expressions,
and three reciprocal carrier choices; none scored below 1186. The repeat-inverse
candidate scored 5945 while still allocating inverse to F20. Artifacts:
`particle/float-web-frontier/20260926-fpr-lifetime/`.

**Next action:** derive a compiler-sensitive source change that places the particle's
scaled X/Y/Z in F20/F22/F16, its 0.125 scale in F24, and its inverse in F2 without
changing the emitted 1420 words. Independently resolve effect raw X F12→F18 and inverse
F18→F2. Compare complete functions at score zero before integrating. The latest
production smoke check ran `ninja build/pokemonsnap.ok && sha1sum -c checksum.sha1`:
Ninja reported no work and `build/pokemonsnap.z64: OK`. Private scratch scoring does not
overwrite production objects; rebuild before trusting the checksum after any scoring
that uses a production object name.

## Earlier renderer experiment

The earlier `fx_draw` candidate scored **3477 / frame 736 / 1374 words** at
`nonmatchings/orchestrated-202606/effect/viewport-y-single-volatile-read/000-ea1f3ebc4457/candidate.c`.
The complete 12-word second-norm sequence, including both call delay slots, matches the
target after excluding the normal JAL relocation. Evidence:
`effect/viewport-y-single-volatile-read/best-verification.json`. Full function remains
non-exact; nothing integrated.

The matrix union now has a named scalar float struct in addition to `Mtx4f matrix` and
`u32 bits[4][4]`. All Y coefficients use named.xy/yy/zy. Three erased integer
observations precede: `sqrtf(SQ(named.zy) + (SQ(named.xy) + SQ(named.yy)))`. This is not
another array spelling: the passive trace proves the named coefficients are direct isvar
nodes with veqv 1 and no live ranges, rather than Uilod nodes with a globally allocated
base address. Both one-field and three-field traces have zero changed words against
normal IDO; see
`global-colour-trace/effect-named-{one,three}-coefficient*/compile.stderr`.

The Boolean LUT cache correction changes exactly two instructions, retaining the actual
G_TT_RGBA16 command. Reading volatile sp210 once into the dead temp_f12 after X sorting,
then using that scalar twice, removes the duplicate read. Latest candidate: exactly one
SP+0x210 load and store, like the target. Pure/nonvolatile/addressable carriers instead
move computations or change allocation; see the viewport-y-equivalence/addressability
families.

Remaining visible differences include early S-step stores, list cursor SP+0x94 versus
target 0xb4, and the projection register web. Fetch-index widening and staging did not
improve 3477; explicit sentinel/direct-index loops shrink the frame to 728. The
per-camera indexed-base variant is identical 3477.

`effect/projection-lifetime-boundary/002-1e4adf9d3050/` reaches inverse F2 and target
extent F16/F18, but gives raw F20/W F12 and a normalized-X F18-to-F12 copy
(4010/736/1376). Repeating inverse before clearing W gives 4335; without clearing W it
is worse. Diagnostic alternatives; no improvement over 3477.

## Integrated window match

Integrated `func_80374714_847EC4` into `src/window/847B60.c` as ordinary C and removed
its `GLOBAL_ASM` fallback. With the configured normal IDO compiler, the complete
387-word object now reports `CURRENT (0)`. Normal-compiler match; no forced registers or
compiler.

The minimal change from the score-10 candidate removed the tail
`texelCount = sprite->height` assignment and computed the reloaded cache size directly:

```c
cacheSize = (s32) ((u64) reloadBitmap->width_img) *
            (s32) ((u64) sprite->height);
```

`clang-format` 21.1.8 was run. The three same-line pixel-load statements remain on one
source line through narrow `clang-format off/on` comments because IDO's schedule depends
on that boundary.

Prior window-integration verification (not rerun during this pause):

- The normal ninja-built window object reports `CURRENT (0)` for the complete
  387-word function.
- `tools/score_remaining_asm.py` now compiles only the two remaining targets and
  reports `func_8009E3D0 = 1707` and `fx_draw = 4366`.
- `ninja build/pokemonsnap.ok` completes with `build/pokemonsnap.z64: OK`.
- The legacy `nonmatchings/func_80374714_847EC4-fulltu/splice_full_tu.py` helper
  now uses the next-function boundary rather than the removed fallback, and its
  recompiled exact candidate still reports `CURRENT (0)`.
- Local `.omp/compile_commands.json` has the corrected clangd include paths.
  Window diagnostics are 0 errors and 1 pre-existing unused-include warning.

## Historical goal and mechanism research

Only these two production fallbacks remain:

- `func_8009E3D0`: production score **1707**.
- `fx_draw`: production score **4366**. The older **4586** figure is stale.

Continue mechanism-driven research for those two functions only. The diagnostic and
source runners are frozen dependencies; do not edit them while an experiment uses them.
Remaining renderer work does not reopen the completed window match. Current research is
under `nonmatchings/orchestrated-202606/`, principally `global-colour-trace/` and the
renderer-specific experiment directories.

Earlier renderer experiments established (superseded by the bests above):

- Particle ordinal indexing with `s32`/`u32` produces an unmasked,
  compiler-generated byte counter using `lw t6` and `addiu t7`, unlike the
  manually reconstructed masked counter. Best ordinal score is **1716**,
  frame `0x308`, counter `0x94`; removing the projection-row alias reaches the
  target counter slot `0x98` at score **2052**, but not the target frame `0x2f0`.
  Union4 plus ordinal indexing gives **7703**, frame `0x2f8`, counter `0x90`.
  Evidence: `particle/mechanism/ordinal-results-full/metrics.json` beneath
  `nonmatchings/orchestrated-202606/`.
- Effect second-norm operand ordering improves the research score to **4334**:
  swap the first two `SQ` terms in the `temp_f0` assignment. Candidate:
  `nonmatchings/orchestrated-202606/effect/mechanism/norm-rqhp27ag/008-0d38708bd667/candidate.c`.
  The 24 scope, 13 norm-load, and 7 list-loop experiments found no exact match.
  The latter loop family was entirely worse than the production baseline.

Neither result integrated into renderer sources.

### Renderer frame evidence

The repaired allocation probe was compiled and exercised against four normal particle
objects, with **zero changed instruction words** each. Sources and metadata:
`particle/highwater/uopt-alloc-diag/`; logs:
`particle/highwater/trace-probes/alloc-{baseline,ordinal,drop-proj,compact}/`. Filter
`UOPT_SPILLTEMP` by `proc=38`, not its allocation-event `seq`.

| Particle source | CFE local bytes | New spill slots | Pre-reemit bytes |
| --- | ---: | ---: | ---: |
| Production candidate | 620 | 9 | 656 |
| Canonical ordinal | 624 | 10 | 664 |
| Ordinal without projection-row alias | 620 | 10 | 660 |
| Compact nonoverlapping carriers | 596 | 10 | 636 |

The first current-block `Urlda` snapshots preallocated spill space into the final Mmt
definition. Later `gettemp` allocation does not explain these frame differences. Decode
big-endian VariableLocation fields; never read inactive `temploc` union members for
islda/isvar.

Frame size also includes callee saves. The compact reciprocal/radius carrier saves an
additional `$f26` pair: **760 bytes / 1422 words**, despite the 636-byte optimizer
definition. Reusing `left` or `top` for the radius instead removes that pair. The
split-assignment candidates have **752-byte frames, counter at `0x98`, and 1420 words**,
but still score **9342 / 9307**:
`particle/compact-float-carriers/runs/{009-left-split,011-top-split}/candidate.c`.
Structural equality is not a whole-function match.

Measured matrix positions constrain the first-matrix prefix to 20 bytes for particle and
24 bytes for effect; they do not identify original variable names. Particle's target
view/projection slots are `0x29c/0x25c`. Effect's are `0x288/0x248`.

Effect camera-carrier research now reaches **4316**, with the exact **736-byte frame**
but **1375 words** versus target 1374:
`effect/camera-carriers/008-14a02f4f6c25/candidate.c`. Removing the existing erased
`if (sp220);` gives 1374 words but score 4536. Its second matrix norm still uses
globally coloured FPRs where the target uses scratch FPRs. The passive eligibility trace
below now identifies the specific live-range exclusion that differs between the two
norms.

Additional completed negative families are preserved in their metric files: particle
`ordinal-erased-uses/` (256), `ordinal-coalescing/` (62), `cache-types/` (20),
`matrix-prefix/` (9), `scalar-carriers/` (4); effect `register-webs/` (18),
`matrix-prefix/` (6), `direct-camera/` (10), and `wide-norm-addresses/` (56). Wide
matrix-address conversions added instructions; plain pointer spelling folded back to the
control.

Do not integrate the old effects-union ordinal experiments: reusing the effects-base
storage during the loop can invalidate later iterations.

### Particle storage refinements

Numerical best at this stage: **1414 / frame 768 / counter 152 / 1420 words**:
`particle/bounded-local-widths/runs/002-cache0-tiles1-next0/candidate.c`. Only
`sFlags/tFlags/sMask/tMask` were narrowed to `u8`; their values are bounded by 8. Keep
the three state caches full-width: `G_TT_RGBA16` is **32768**, not 2, and the initial
sentinel is -1.

Frame/counter-correct best at this stage: **3283 / frame 752 / counter 152 / 1420
words**: `particle/camera-lifetime-packing/runs/000-camera-and-effect/candidate.c`. It
reuses gobj for the effects base, shares the completed camera lifetime with the
current-effect pointer, removes the redundant projection-row pointer, and keeps
sprites/palette separate. It also retains the single TLUT pointer instead of a redundant
copy. SHA-256: `a4fe5d5abf101849f4201c5a64a29989479f453e1728e6e0a16801d15b7d1578`.
`camera-lifetime-packing/verified-scalar-homes.json` records the seven matching homes:
0x22c,0x224,0x21c,0x228,0x220,0x218,0x234. Saved FPRs are F20/F22/F24.
Camera/textureData and camera/tlutData alternatives score 3663 and 3483 with identical
frame/counter/words.

The previous **3294/752/152/1420** source remains a useful diagnostic parent:
`particle/packing-refinement/runs/000-pair-after-floats/candidate.c`. It uses camera/row
and sprites/palette unions. Older experiments below use that parent, not the 3283 best.
Neither is exact.

New negative particle families are preserved with normal-compiler metrics:
`bounded-pointer-refinement/`, `bounded-fused-radius/`, `radius-operand-eligibility/`,
`balanced-projection-homes/`, `late-copy-operand-identities/`,
`all-inverse-uses-nested/`, and `inverse-definition-boundaries/`. Merely balancing a
separate raw-X home against a merged reciprocal/radius home did not fix the float
register web. Volatile radius operands add seven words; fused radius expressions add
one.

Normal-IDO best at this stage: **1358 / frame 752** at
`particle/packed-web-correction/003-tlut-pointer-temporary/candidate.c`. Its
one-TLUT-pointer compare-and-carry keeps the `loadedTLUT`/`selectedTLUT` roles, while
all three texture-state caches remain full-width. The target projection web uses
F20/F22/F16 for scaled position X/Y/Z, F18 for raw clip-X, F14 first for clip-W and
later normalized clip-Y, F2 for reciprocal W, F12 for normalized clip-X, and F24 for
normalized clip-Z; target saved FPRs remain F20/F22/F24. Thus the residual float
mismatch is in expression and value-live-range allocation around projection, not the
verified frame, TLUT pointer, or saved-FPR set. This mapping comes from the target
assembly; no candidate assembly/register result is claimed.

`particle/projection-refine-next/` contains two full-TU ordinary-C variants based on
that 1358 source: `001-divided-coordinates.c` forms each normalized coordinate with
division by `clipW` while retaining `invW` for the size path; `002-inverse-reuse.c`
computes raw X/Y/Z before creating and reusing `invW`. They probe reciprocal
lifetime/register pressure versus direct coordinate division without touching the
projection matrix, row aliases, TLUT logic, cache widths, or sprite loop. The earlier
projection-row alias/reorder families are intentionally not repeated. Both variants were
scored after this note was written: 3772 and 4926, respectively.

`particle/frame-home-order-next/` holds three full-TU variants based on the 1414
numerical parent. They reorder pointer carriers (`001-pointer-order.c`), reorder
matrix/pointer carriers and end `view`'s lexical lifetime after `guMtxCatF`
(`002-matrix-order.c`), or combine pointer and matrix declaration ordering
(`003-pointer-matrix-order.c`). These target the numerical parent's seven scalar homes,
each four bytes below the verified homes, and its 16-byte frame excess. The view/proj
matrices remain distinct and simultaneously live at the multiply; pointer roles and
computation semantics are retained. Desired homes are
0x22c,0x224,0x21c,0x228,0x220,0x218,0x234, with F20/F22/F24 saved as in the
frame-correct parent. Pointer roles/lifetimes are unchanged; view/proj remain distinct
because `guMtxCatF` reads both. Scores: 1450, 4104, and 2868, respectively; none beats
1186. See `particle/preserved-uncompiled-scores.txt`.

### Historical allocator mechanism evidence

Production renderer sources unchanged. No new exact renderer match. A whole-function
source-line packet reaches effect score **4221** in `effect/geometry-line-packets/`;
this is not an integration candidate.

The current private `global-colour-trace/toolchain/uopt` contains passive copy,
variable-identity, live-unit, and pre-allocation diagnostics generated by
`patch_copy_probe.py` and `patch_colour_identity_probe.py`. The latest generator tag is
`A944`; its generated image and metadata are `uopt-identity.c` and
`uopt-identity.metadata.json`. Both unset and enabled eligibility controls reproduce the
normal **1375-word** effect object with **zero changed words**. Evidence:
`eligibility-probe-metrics.json` and `eligibility-{unset,enabled}/compile.stderr` in
that directory.

The decisive observed difference is `Graphnode.indiracc`, not coefficient recurrence
through the particle loop:

- First-norm X coefficients at local addresses `-152/-136/-120` have
  `count=2`, `vreg=0`, `veqv=0`, basic block **6**, `indirect=1`, and no
  live ranges.
- Second-norm Y coefficients at `-148/-132/-116` have the same count and
  flags, but basic block **9**, `indirect=0`, and live ranges **96/99/104**.
  Each has exactly one live unit, in block 9, with two loads.
- `uoptreg1.c` excludes variables present in `node->indiracc` when creating
  these live units. The target second norm uses FPRs
  `{0,4,6,8,10,12}`; the control uses `{0,2,4,6,8,12,14}`.

For effect, the copy probe separately proved `has_ilod` rejects raw-X propagation and
`no_nested_ops` rejects reciprocal propagation at the radius multiply. Relaxing those
guards in an explicitly non-acceptance compiler did **not** produce the target float
roles. Those experiments are retained as `copy-policy-{0,1,2,3}/`; the disabled control
changed zero words. Do not accept or integrate any object from an enabled policy
experiment.

Recent ordinary-C negative families and metrics are retained:

- Effect: `explicit-raw-common-expression/`, `projection-stage-joins/`,
  `projection-row-aliases/`, `scoped-projection-row-aliases/`,
  `coupled-norm-projection-aliases/`, `norm-call-context/`,
  `norm-indirect-access/`, `norm-result-indirect-store/`, and
  `norm-single-read-caches/`. Pointer aliases, including a coupled norm/loop
  alias, did not fix float roles. Inlining the second sqrt argument did not
  remove its coefficient live ranges. Discarded camera reads, indirect
  result stores, and single-read volatile coefficient caches also missed.
- Particle: `scalar-pointer-homes/` proves that replacing the six-member
  pointer union with one `void*` and typed reads is instruction-identical:
  **9994 / frame 760 / counter 160 / 1422 words**. A `u32` carrier is worse.
  Aggregate-versus-scalar aliasing is therefore not the missing mechanism.

The alias construction is now traced in `uoptcopy.c:2889-2910` and
`uoptkill.c:1453-1513`. Unknown non-heap indirect accesses can mark escaped stack
variables through `aliaswithptr`; exact stack addresses instead use an overlap check.
Moving one required viewport read between the two sqrt calls gives the target
second-norm FPR set, proving this mechanism, but moves real instructions and misses the
complete function (`effect/norm-viewport-access/`). Production unchanged.

Further effect negatives: `norm-alias-identities/`, `norm-equivalent-address-joins/`,
`sqrt-call-contracts/`, `norm-duplicate-viewport-access/`,
`raw-projection-lifetime-barriers/`, `raw-copy-bridges/`, `joint-copy-eligibility/`,
`alias-construction/`, `destructive-norm-staging/`, and `erased-float-alias-access/`.
All four aggregate alias-construction variants score 4788 with 1375 words, frame 736,
and the unchanged second-norm register set.

Particle's passive trace is
`global-colour-trace/particle-all-nested-copy/compile.stderr` (procedure 38). The
disabled/enabled private controls are word-identical. Against the normal object, only
two `.rodata` relocation addends differ; the exact disassembler evidence is
`particle-copy-relocation-evidence.json`. In this candidate, reciprocal variable bit
263/address -248 is correctly blocked by `no_nested_ops` at the radius multiply.
Normalized-coordinate copies nevertheless already contain W bit 244/address -192 and a
DIV expression rather than reciprocal bit 263. X/Y/Z bits are 267/272/284. Unlike
effect, particle's cached-position projection expressions have `has_ilod=0`; do not
transfer effect's copy diagnosis blindly.

The earlier forwarding pass is now identified: `uoptinput.c:1658-1752` pushes an
available local assignment RHS while reading Ucode, before `copypropagate`. Its
rejection predicate is
`bigtree || treekilled || (isop && count == 1 && (!eligible_vreg_or_copy_or_trap || has_ilod) && !constexp)`.
**`has_ilod=true` blocks forwarding**, not the reverse. `store_av` is initialized from
`!veqv` at lines 3257-3258. The inverse is observed as a same-procedure `vreg=1`; graph
BB numbers are not the lexical Ucode block number (376 in this particle trace).

New storage controls: particle's inverse struct and discarded address observation are
identical to 3294; a one-element inverse array gives 6584/752/152/1424. Reverse cache
struct is identical; reverse cache array gives 3304/752/152/1420. Evidence is in
`inverse-local-storage/` and `cache-storage/`. Ten/twenty multiplication identities do
not disappear and are rejected (`input-tree-threshold/`).

Dead W reassignment after division exercises `treekilled` but does not match. Adding an
erased inverse use to the balanced raw-X/inverse candidate produces
**6197/752/152/1422**, with reciprocal F2, but normalized X is computed into F16 then
copied to F12. Its raw projection also moves after the W guard. Preserving the raw use
gives 5031/752/152/1424 with a raw spill. Mechanism probes; no improvement on the
numerical best. See `inverse-operand-killing/`, `killed-operand-value-uses/`, and
`normalized-X-lifetime/`.

Effect's real matrix union plus an erased integer observation of one Y-column
coefficient makes the other two coefficients `indirect=1` with no live ranges. The
observed coefficient becomes an indirect floating load through a globally allocated
matrix address in S3. Thus the apparently favorable FPR set is **not** a scratch-only
target norm: its F0 coefficient load, S3 base, and ten-word norm block still differ.
Normal score is 6766/736/1376; the passive trace is word-identical
(`global-colour-trace/effect-one-matrix-equivalence/compile.stderr`). Metrics:
`matrix-equivalence-flags/`, `matrix-equivalence-location/`,
`matrix-observation-spelling/`, and `matrix-float-views/`. Void observations and plain
float row views return to the control.

The original CFE contains all three purity spellings: `no side effects`,
`no_side_effects`, and `sgi_no_side_effects`. The three-token spelling and duplicate
viewport reads still miss (`actual-side-effect-pragma/`). `intrinsic(sqrtf)` replaces
the two sqrt calls, reducing five calls to three; it is not a matching route. Removing
the unused render-state helper does not change the effect function; moving it after the
renderer changes the score to 4516 without fixing the norm (`unused-helper-context/`).

Additional normal-compiler mechanism results:

- `effect/matrix-partial-overlap/`: byte/halfword observers reproduce the
  whole-word union behavior; they do not remove the address register.
- `effect/asymmetric-square-addresses/`: one pointer and one direct operand
  retains the coefficient globals, 5981/736/1377.
- `effect/late-folded-square-address/`: an always-zero camera-derived offset
  folds, but direct and indirect operands retain two distinct loads and an
  address ADDIU. Best 5671/736/1376; no instruction-level norm match.
- `effect/redundant-indirect-store/`: self-storing a camera/flag field gives
  scratch coefficient allocation but retains the extra memory operations
  (4931 or 5886, frame 736,1376 words). Matrix self-store does not fix allocation.
- `effect/dead-alias-branch/`: a constant-dead self-store or volatile-read
  arm has no surviving eligibility effect. All four controls are identical,
  6187/736/1373, with the original coefficient globals.
- `particle/viewport-copy-operands/`: volatile operands and float/double
  round-trips do not change the unwanted F16-to-F12 normalized-X copy.
  The round-trip actually emits CVT.D.S and CVT.S.D; it is not a free barrier.

The prior array-union trace gives the observed floating Uilod expression count 2 and a
global range (bit 98, `compile.stderr` lines 99–100). The other two direct coefficients
have `indirect=1`, no ranges. The new named-float view above resolves this distinction
with direct veqv variables.

`effect/purity-behavior-probe/` appends a small pointer-read/sqrt/pointer-read function.
None of the three purity spellings eliminates its second heap read; all four normal
probe functions have identical instructions. The isolated read-only
`global-colour-trace/purity-metadata-probe/` confirms `Proc.no_sideeffects=0` for the
spaced and plain underscore spellings, but `=1` for `sgi_no_side_effects(sqrtf)`. All
four traced probes match normal-object words exactly. The correct SGI attribute still
leaves the renderer and duplicate-viewport experiments unchanged
(`effect/sgi-pure-norm-alias/`). Ordinary and previous passive toolchains are unchanged.
A new private driver must be copied, not symlinked: the symlinked driver selects its
original directory's optimizer.

The `effect/iload-square-frontier/runs/` batch disproves a base-kind hypothesis. In the
existing union trace, Uilod `op1=1018b458` (line 99) is already `kind=1/islda`, but is
globally allocated (bit 1024/reg 17, line 534). `uoptemit` requires both simple islda
and `!inreg` for direct Ulod emission. Seven pointerless overlays all give
6565/736/1376; three observed words give 5792/736/1377. Those variants changed unsigned
overlay spelling while retaining array float reads; named scalar float reads differ.

`particle/reciprocal-frontier/runs/`: compound division and `register` remain 3294;
narrower lexical scopes 3298. Reusing clipW gives 5146/744/148/1424 with inverse F20;
assignment-expression reuse gives 5392/760/160/1427 with inverse F22. No
reciprocal-register fix.

The reciprocal array actually has a global live range in its definition block:
`global-colour-trace/particle-inverse-array-eligibility/compile.stderr` shows bit
263,addr-240,vreg 0,veqv 0,indirect 0 and one BB15 unit (loads 3,stores 1). It is still
stored to SP and reloaded for radius. The newer read-only
`global-colour-trace/array-block-probe/` identifies the missing later unit: BB23's size
heap read (Uilod, base variable addr-272) sets the array's `indiracc=1`. Caching size
before the W guard makes BB23 `indiracc=0` and adds a later array live unit plus
reaching units, but the reciprocal still spills. Array/scalar union views do not remove
that alias. Normal comparisons for all three block-probe subjects differ only at
R_MIPS_LO16 `.rodata` addends; disabled versus enabled probe is exactly 1424 words with
zero changes. See `array-block-probe/equivalence.json`.

Further bounded negatives (all under the session research root):

- `particle/denominator-indirect-views/`: W integer-view observers give
  7829/752/152/1425 with inverse F20; an array W gives 8073/752/152/1426.
- `particle/raw-single-predicate/`: simple truth/self-equality raw uses
  reproduce 5031/752/152/1424, including the raw spill. They are not a
  short-circuit-only effect.
- `particle/raw-post-normalization-use/`: keeping raw X alive later
  increases frame/word counts; no destination coalescing fix.
- `particle/x-assignment-boundary/`: copying raw X then compound-multiplying
  is identical 6197; copying inverse first 6202; modifying raw then copying 6270.
- `particle/near-fused-reciprocal-consumer/`: repeating only the radius
  reciprocal expression or reusing its variable does not improve matching.
  The old 1546 fused candidate uses SWC1/LWC1 for its reciprocal, not a
  simple MOV. Its F2 reciprocal alone is not evidence of a correct lifetime.
- `effect/fresh-coefficient-staging/`: fresh scalar coefficients give
  5816/744/1374; volatile reads give 6856/744/1376. Coefficients remain globals.
- `effect/first-norm-indirect-store/`: using the already-required result
  store through a direct/late-folded pointer does not mark the norm
  coefficients indirect. Best5606/736/1376; no norm-register fix.
- `effect/precall-second-norm-argument/`: precomputing the Y argument
  before the X sqrt keeps it in F20 across that call. Both normal and
  SGI-pure versions are5173/736/1375, not the target computation order.
- `effect/ordered-coefficient-type-views/`: erased float observations
  before integer observations do not change the one-word union result.

The purity limit is now grounded in compiler source: `uoptkill.c:741–742`
unconditionally kills C non-variable load expressions before the purity check at
749–750. Thus a true purity flag does not preserve Uilod availability across sqrt. The
proposed extern-global contrast also lowers to Uilod in this configuration, so it
remains unchanged; see `effect/purity-direct-global-contrast/` and the matching private
`purity-metadata-probe/global-sgi-pure/` trace (purity 1, bit 2/opcode 54). Do not
assume a new Expression means a new IChain bit without tracing it.

Emission evidence: `global-colour-trace/emission-endpoint-probe/`. The passive hook at
generated L42c9b8 reads the actual Ustr RHS/LHS and block register table. Particle BB17:
normalized Umpy RHS bit 1111/ichain 1014a1f8 is reg 28/F16, clipX LHS bit 270 is reg
26/F12. Effect BB28: Umpy RHS bit 219 is reg 29/F18, LHS bit 217 is reg 26/F12. Both RHS
expressions have global ranges. The scout's earlier bit 322 hypothesis was wrong: that
is a later extent variable which happens to reuse F16, not the normalized-X RHS.

New particle hook: zero changed words against prior passive compiler. Against normal
IDO, only two .rodata LO16 addends differ (indices 503/555); both 124-byte switch tables
are byte-identical. Effect has zero changed words against normal IDO. See
`emission-endpoint-probe/equivalence.json`.

Preserving inverse in a separate radius variable removes/rearranges the problem but
loses register/frame matching (`immutable-inverse-radius/` in both renderer
directories). Clearing rawX after normalization is the direct input-forwarding kill; it
also changes the register web, not an exact fix (particle8262/744/144/1420,
effect5739/736/1370). Early raw observations plus that kill are still worse;
normalized-X array/addressed carriers likewise fail. Keep these as bounded negatives,
not candidate improvements.

The external reference-shape manifests have now been compiled; all are negative.
Particle scores are 9295/7612/7971/7971/9517/7062; effect scores are
11548/6425/10938/9484/8860/10446/10903. Frozen parent-build sources and hashes are
authoritative, especially effect's `source-versions.json`: the agent revised its
originals before compilation, then was stopped. Source lead, not matching evidence:
https://raw.githubusercontent.com/VetriTheRetri/ssb-decomp-re/master/src/lb/lbparticle.c

Latest bounded negatives:
- `effect/list-dual-induction/`: 7281/728/1377 and 6818/736/1381.
- `particle/material-scalar-lifetimes/`: removing the material union alone
  gives 3287/752/148/1420. Reusing posX/left or posY/top worsens it;
  posZ/depth reuse also loses the frame. Do not narrow alpha/blend cache
  locals casually: their addresses reach a render-state macro, and blend
  alpha can be any u8 value.
- `particle/frame-without-material-alias/parent-builds/`: pointer order/
  counter-scope changes remain 3287 or 3367/752/148/1420. Retaining the
  effects base gives 3653/760/152/1420. Neither beats a qualified best.
- `effect/projection-work-scalar/`: real W or extent reuse for rawY/Z
  gives 7948–9155, with 1383–1386 words. It is not a copy-kill solution.
- `effect/step-dataflow-frontier/parent-builds/`: control 3477; inner-step
  temporary 4711/736/1377; tail-scope step 3857/736/1374; rectangle step
  aliases 6313 or 6421/744/1379. The inner temporary still stores early,
  now at SP+0x198. `effect/explicit-register-step/` is word-neutral.

Further completed source probes, all negative:
- `particle/direct-raw-projection-fields/`: named X or X/W fields give
  1459/1439 on the numerical parent and 3339/3319 on the old packed parent.
- `effect/direct-raw-projection-fields/`: X-only 3817/736/1374; explicit
  position-Z carriers 8153–8238/736/1383. This did not solve raw forwarding.
- `effect/consumed-copy-kill/`: actual position/inverse copies consumed by
  Y/Z normalization give 8177/736/1383 or 5739/736/1370. Initial records 0/2
  contain a generator typo; corrected token-fixed records 4/5 compile.
- `effect/raw-normalization-aggregate/`: the named-field inverse-boundary
  diagnostic scores 3481/736/1375 at `005-dd92bb27b511/candidate.c`.
  It STILL emits MUL F18 followed by MOV F12, with raw F20/W F12. Arrays
  add raw spills and are worse. Do not promote3481 as a solved geometry.
- `effect/rectangle-coordinate-consumers/`: local-right3521/736/1374,
  local-extents4330/744/1374; reusing dead material scalars remains 3477.
- `effect/immutable-normalized-x/`: 9537–13033, frames 736/744,
  1385/1389 words. Delaying bottom calculation does not fix the extra range.

### Fresh copy-propagation evidence and implemented passive probe

The frozen passive compiler was rerun with `IDO_TRACE_COPY=1` on three sources. All
match normal-object instruction words exactly: original normalized-copy diagnostic 1376,
effect best 1374, and scoped-Sstep 1377. Evidence:
`global-colour-trace/emission-endpoint-probe/copy-decisions/equivalence.json`. Each
named subdirectory there contains `compile.stderr` and `candidate.o`.

In `effect-original-normalized-copy/compile.stderr:158-167`:
- LHS use: orig 1017c7f0, chain 1017d168, bit 217.
- Actual RHS predicates: has_ilod(expr1017e0f8)=0, is_incr=0,
  countvars(chain1017cfd8)=2.
- COPY_END returns chain 1017cfd8, kind 4, bit 219: the normalized-X variable
  use really IS replaced by its Umpy RHS in copypropagate.
- Lines 452–453 show bit 219's independent pre-global range 100a77b8;
  line 817 assigns allocator color 29/F18. Line 889 joins that same RHS to
  BB28's LHS bit 217/color 26/F12. This is the extra MOV's causal path.

Do not infer Expression.count from PRE_CHAIN's count 0 when its expr field is null: that
zero is a logger default. The operator Expression exists separately at 1017e0f8.
Existing COPY hooks filter variable kinds 3/6.

Next passive hook is implemented, unbuilt, in
`global-colour-trace/emission-endpoint-probe/next-passive-probe/uopt-emission-next-passive.c`;
the frozen source remains unchanged.
1. At generated `L45f758`, `UOPT_EXPR_ENDPOINT` logs the actual
   `MEM_U32(MEM_U32(sp+92)-4)` Expression fields, filtered to procedure 20,
   dtype 13/float, opcode 91, and reports the readable range gate.
2. At `L45f818`, `UOPT_RANGE_PROBE` records bit 219's slot before and after
   the normal `f_formlivbb` call. It dereferences the Bittab base at
   `MEM_U32(0x1001c3a0)` before adding `bit*8+4`.
3. Passive hooks observe normal execution only; no optimizer-helper calls or decision changes.

For the distinct original normalized-X diagnostic, the generated probe copy is
`global-colour-trace/diagnostic-range-probe/uopt-diagnostic-range-probe.c`. It filters
procedure 20's kind 4 operator at bit 219, logs the Expression at `L45f758`, the bit 219
Bittab slot before/after `f_formlivbb` at `L45f818`, and the range-slot creator writes
in `f_formlivbb`. Each logger is read-only and capped (16 Expression records, one
before/after pair, 16 creator writes). Use this exact normal candidate-source compile
invocation via `tools.score_remaining_asm.COMPILE` (with `src/app_render` included):

```sh
python3 -c 'from tools.score_remaining_asm import COMPILE; import subprocess; subprocess.run([*COMPILE, "-I", "src/app_render", "-o", "nonmatchings/orchestrated-202606/global-colour-trace/diagnostic-range-probe/candidate.o", "nonmatchings/orchestrated-202606/effect/projection-lifetime-boundary/002-1e4adf9d3050/candidate.c"], check=True)'
```

Use the 1376-word `candidate.c` above, not the then-best 3477 candidate. This probe copy
is research-only; the original `next-passive-probe/uopt-emission-next-passive.c` remains
untouched.

Assignment endpoints now cover integer and floating expressions and scan allocator
colors 1–35. Identity live-unit details no longer filter by dtype, so integer ranges are
included. Scoped trace `effect-scoped-sstep/compile.stderr:751-752` has chain 10172de0,
original identity bit 358, split-range bit 1186, dtype 6/Jdt, vreg 1, veqv 0, color 6.
This is NOT yet correlated with the early Ustr; do not claim it explains the SP+0x198
store. Identify the emitting node and its exact IChain map. IChain.location.addr is not
Temploc.disp; the isvar temploc pointer is IChain+0x20, with disp at Temploc+4.

The next passive probe must span UOPT and UGEN; UOPT does not know final machine
encodings. The copied UOPT hook and exact stage/field specification are in
`nonmatchings/orchestrated-202606/global-colour-trace/step-emission-probe/`. Filter the
UOPT map to proc 20 bit 358 at BB44/BB45; log IChain, its link, `IChain+0x20` Temploc
pointer, and signed `Temploc+4` displacement. UGEN is a separate binary in the private
toolchain; no generated UGEN source, emission dispatch mapping, or actual UOPT-to-UGEN
stream contract is available here. The machine-opcode/register/SP-displacement record
therefore belongs in the matching UGEN emitter once its generated source and unchanged
UOPT intermediate input are available. Do not guess a stream filename/format or
attribute the SP+0x198 store to bit 358 without the displacement correlation.

Allocator colors are not hardware register enum numbers. Apply uoptutil.c:2295–2308:
color 1→v0,4→a1,5→a2,6→a3; FP mapping follows below. The scout's claim that logged color
6 means a2 was incorrect.

Existing private toolchain: `emission-endpoint-probe/toolchain/`. For another toolchain,
COPY cc (never symlink); other stages may symlink to the normal tools, with only uopt
replaced. Previous host build used gcc -std=c11 -O2 -fno-strict-aliasing, linked
`version_info.o`, `libc_impl_71.o`, and `-lm`. Required
`/tmp/snap-ido-research/header.h` and both objects were absent; this pass found no
checked-in regeneration source. No build/trace run during that pass.

Encoded allocator FPR numbers skip reserved scratch registers:
F0=24,F2=25,F12=26,F14=27,F16=28,F18=29,F20=30,F22=31,F24=32, F26=33,F28=34,F30=35. Keep
whole-function scores separate from frame, slot, instruction-count, and register-set
probes.

Three full-TU S-step mutation-boundary hypotheses are archived under
`nonmatchings/orchestrated-202606/effect/step-use-next/`, per-candidate
arithmetic-preservation notes: `manifest.json`. They separately test branch-local
converted definitions, integer scaling before a single final definition, and a
conditional choice between converted/final integer values. All preserve the original
float quotient; MIRROR_S doubling follows s32 conversion only. Texture masks/rectangle
arguments unchanged. Explicit-cast variants convert to s32 before multiplying; no
narrowing or float/double reorder. At this historical stage, these were
uncompiled/unscored hypotheses: BB44 bit 358 and BB45's repeated bit 358 IChain are
mapped, but their relation to the early SP+0x198 store remains unproven.

## Window evidence

Latest window height-graph run: 37 candidates. Metrics:
`nonmatchings/orchestrated-202606/window/height-graph-results/metrics.json`;
`reload-width-cast` is the exact candidate and the normal baseline is the
two-register-only 387-word near-match. Exact source:
`nonmatchings/orchestrated-202606/window/height-graph-results/exact-match.c`.

Production source: `src/window/847B60.c`, uncommitted at this historical stage. Object
evidence: `nonmatchings/orchestrated-202606/baselines/window-exact.diff`:

```text
TARGET                                                   CURRENT (0)
<518 lines>
```

Baseline hash manifest: `nonmatchings/orchestrated-202606/entry-hashes.json`. It is
checkpoint evidence; the deliberate updates to this handoff and the matching notes
change those two document hashes. Renderer hashes match recorded entries.

## Historical window research (do not restart)

Diagnostic compiler reconstructed from clean generated IDO sources and independently
exercised. Baseline: 387 words, two mismatches. Source fix: 387 words, zero mismatches.
Forcing only function 7, evaluation 68's desired-register argument from 3 (`v1`) to 15
(`t7`): 387 words, zero mismatches. The `f_ureg` return remains 3; forcing it instead
changes parent state and fails.

`f_build_u1` copies incoming Ucode word `+12` into tree word `+44`, yielding descriptor
value `0x0c`; `f_ureg` divides that register byte offset by four. The readable IDO
`uoptutil` `coloroffset` table excludes `t6`–`t9`. This identifies the old path as Uopt
global-register metadata consumed by Ugen, not a choice between two Uopt global colors.

Reconstruction scripts: `nonmatchings/orchestrated-202606/compiler/`. Final logs/parsed
metrics: `compiler/verified-traces/` under that research root:
`reconstruction-final-{baseline,exact,forced}.log` and matching `.metrics.json` files.
Forced builds are diagnostic evidence only.

The natural-reload, no-op replacement, and broad global-color sweeps recorded in the
older notes are historical search evidence. The validated 37-candidate run supersedes
them; do not resume window sweeps.

Original 339-line handoff (exhaustive search counts, compiler-trace map, reproduction
commands): `nonmatchings/orchestrated-202606/entry-snapshot.zip:continue.md`. Consult
the archive for historical detail; do not restore stale completion status here.

## Historical repository anchor

Upstream anchor `1978bb52` equals the remote anchor. At this historical checkpoint, two
lockfile patches were applied locally without commits: `gitpython` 3.1.60 and
`mapfile-parser` 2.13.2.

## Verification invariant

The scorer overwrites the two renderer objects with candidate C builds. After scoring,
remove those objects and the checksum stamp before rebuilding the production ROM.
Removing the window object too is safe:

```sh
rm -f \
  build/src/window/847B60.c.o \
  build/src/app_render/47380.c.o \
  build/src/app_render/effect.c.o \
  build/pokemonsnap.ok
ninja build/pokemonsnap.ok
```

The linker must consume generated `build/pokemonsnap.undefined_syms.txt`, never raw
`undefined_syms_auto.txt`. Running `configure.py --clean` can regenerate tracked window
research assembly. Restore `asm/nonmatchings/window/847B60/func_80374714_847EC4.s`
before the final build, remove `build/src/window/847B60.c.o`, and rebuild the checksum
target.

## Do not

- Do not reinstate the window `GLOBAL_ASM` fallback or rerun window integration.
- Do not treat a forced compiler object or the old `4586` renderer score as
  current evidence.
- Do not normalize unrelated linker, `func_8009E3D0`, effect, or scratch files.
- Do not stage `AGENTS.md` or local research artifacts for upstream.
- `continue.md` is already tracked; this checkpoint made no commits.
