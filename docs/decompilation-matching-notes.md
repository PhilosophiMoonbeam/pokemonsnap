# Decompilation matching notes

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
match, but its comparison score was reduced from roughly 28,000 to 2,210. The
latest pass improved the previous 3,377 baseline without changing the pixel
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
- The best initialization order found by exhaustive permutation is width,
  height, bitmap, source row, destination width, bitmap row, then tile height.
  This places the source-row counter in the target's `ra` register.
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

The same donor search also found SSB's matched `sySchedulerUpdateViMode`, whose
`0x7A8` size and behavior correspond to `func_80000F40`. Unlike the VPK0
decoder, however, transplanting its source under Pokémon Snap's types expands
the output by `0x10` and increases the byte delta. The scheduler implementations
are therefore related HAL variants, not interchangeable binary donors; exact
size plus semantic identity is a strong lead, but still requires a direct
object comparison before replacing a guarded candidate.
