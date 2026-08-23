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
match, but its comparison score was reduced from roughly 28,000 to 3,377.
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
