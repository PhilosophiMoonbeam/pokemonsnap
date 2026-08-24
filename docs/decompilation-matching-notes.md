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
macros, and assembler tuning all preserve that final scheduling choice. The
`NON_MATCHING` guard therefore remains in place.

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

The current guarded candidate for the window photo downsampler is not yet a
match, but its comparison score was reduced from roughly 28,000 to 1,990. The
latest pass improved the previous 2,030 baseline without changing the pixel
result.
Reusable findings from that work are:

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
