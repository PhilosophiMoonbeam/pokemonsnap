# pokemonsnap

[![US Bytes]](https://decomp.dev/ethteck/pokemonsnap/us)
[![US Functions]](https://decomp.dev/ethteck/pokemonsnap/us)

[US Bytes]: https://decomp.dev/ethteck/pokemonsnap/us.svg?mode=shield&label=US+Bytes&measure=matched_code_percent
[US Functions]: https://decomp.dev/ethteck/pokemonsnap/us.svg?mode=shield&label=US+Functions&measure=matched_functions_percent

A WIP decomp of Pokemon Snap.

Note: To use this repository, you must already have a rom for the game.

For Python dependencies:
`uv sync`

## Setup

1. Place the US Pokemon Snap rom (sha1: `edc7c49cc568c045fe48be0d18011c30f393cbaf`) into the root of the repository as "pokemonsnap.z64".
2. (one-time) Set up tools: `uv run configure.py --setup`
3. Run extraction and disassembly: `uv run configure.py`
4. Rebuild the rom: `ninja`

## Audio banks and wave tables

Extraction generates the complete SDK bank graphs in `assets/audio/bank1.json`
and `bank2.json`, table layouts in `table1.json` and `table2.json`, and
self-contained Nintendo AIFCs under `assets/audio/waves/bank1/` and `bank2/`.
These ROM-derived sources, generated C/binaries, and authored overrides are
ignored; never commit or distribute game assets. Keep the extracted JSON and
AIFCs as the stock baseline. Rebuilding uses those sources, not the ROM.

Configuration rescans segments rather than reusing Splat's segment cache, so
generated undefined linker symbols remain complete on repeated runs. Authored
overrides are outside the extraction outputs and are retained.

To replace an existing sample, copy an AIFC to the matching stable sample ID:

```sh
mkdir -p src/audio/instruments/bank1
# Substitute an actual sample_<table offset>_<wavetable offset> ID from table1.json.
cp /path/to/replacement.aifc src/audio/instruments/bank1/<sample_id>.aifc
ninja audio-mod
```

Use `bank2/` for IDs from `table2.json`. Only existing IDs with the `.aifc`
extension are accepted; unknown names fail instead of silently doing nothing.
Creation, edits and removal are discovered on plain Ninja invocations, including
when the override directory did not exist at configuration time. The always-run
inventory uses Ninja [restat](https://ninja-build.org/manual.html#ref_rule), so an
unchanged inventory/content does not rewrite generated data or recompile it.
No manual JSON/table placement edits, clean, or reconfiguration are needed.

Inputs must be mono Nintendo VADPCM (`VAPC`, with `VADPCMCODES` and optional
`VADPCMLOOPS`) or big-endian PCM16 (`NONE`) AIFC at the existing bank sample rate.
PCM16 sustain loops may use standard `MARK`/`INST` chunks; forward loops are
supported, while ping-pong and separate release loops are rejected. Active
native loops must have positive length.
There is no WAV importer, ADPCM encoder, retuning, or new sound-ID allocation.
Different encoded lengths and codebook/loop geometry are supported; shared
metadata is retained when equal and isolated when an override diverges.
Non-overridden sound/envelope/keymap fields and flags are preserved. Native
partial-frame zero tails remain represented for stock samples; AIFC sample
counts do not reconstruct unknown precompression trim.

The generator emits an actual typed C aggregate with SDK records, exact-count
wrappers, inline alignment padding, compile-time layout assertions and
`offsetof` disk pointers. Configured IDO 7.1 compiles it directly into
`build/assets/audio/bankN.bin.o`'s `.data`; this is not a C byte dump or a
Python bank binary wrapped as an object. Table payloads are rebuilt from AIFCs,
with 8-byte sample/book alignment. Existing linker object identities and runtime
ROM-label loading are retained; no game audio runtime changes are required.

`ninja audio-mod` writes `build/pokemonsnap.mod.z64`, a 16 MiB ROM with corrected
CIC-6103 header checksums and a trimmed/extended `0xFF` tail. Oversized ROMs fail.
The normal `ninja`/`build/pokemonsnap.ok` matching checksum gate is unchanged and
intentionally rejects modified audio. Existing runtime audio memory remains
fixed (heap `0x4B000`); substantially larger bank graphs need a capacity review,
not an assumption that any ROM-fitting payload is runtime-safe.

Remove overrides to restore the exact stock output, without re-extraction:

```sh
rm src/audio/instruments/bank1/<sample_id>.aifc
ninja build/pokemonsnap.ok
```

For isolated generation (the bank output is C, not a bank `.bin`):

```sh
uv run tools/build/audio_banks.py generate assets/audio/bank1.json assets/audio/table1.json src/audio/instruments/bank1 build/assets/audio/bank1.bin.c build/assets/audio/table1.bin
uv run python -m unittest discover -s tests -p 'test_audio_banks.py'
```

`tools/audio_bank.py` remains the lossless extraction/fixed-layout codec for
diagnostics. Production bank generation uses `tools/build/audio_banks.py`; the
Ninja compile edge uses the same configured IDO toolchain as the project.

## MIDI sequence authoring

Extraction writes an ignored `assets/audio/sbk.json` manifest covering the
complete `0xAEFC10`–`0xAFEEE0` sequence region. Its 37 indexed slots retain
stock compressed tracks, physical fragment padding and logical table lengths.
With no overrides the generated `audio/sbk.bin.o` preserves the complete stock
region. The sequence table ROM label remains the native `ALSeqFile` identity.

Use a standard synchronous Type 0 or Type 1 MIDI file with PPQN timing:

```sh
mkdir -p src/audio/midis
cp /path/to/authored.mid src/audio/midis/0.mid
ninja audio-mod
# Remove the override to restore the stock build, without cleaning:
rm src/audio/midis/0.mid
ninja build/pokemonsnap.ok
```

Override filenames are existing numeric song IDs `0` through `36`, with `.mid`
or `.midi` extensions; duplicate IDs and unknown filenames are errors. Plain
Ninja discovers additions, edits and removals automatically. Variable-size songs
repack the table and move later banks through their linked ROM labels. `audio-mod`
produces `build/pokemonsnap.mod.z64` with authentic CIC6103 header checksums;
the normal matching checksum gate is deliberately unchanged. ROM output is
16 MiB, with dynamic FF tail padding; overflow is rejected.

```sh
uv run tools/audio_sequence_bank.py export assets/audio/sbk.json /path/to/song.mid --slot 0
uv run tools/audio_midi.py export /path/to/compact.bin /path/to/song.mid
uv run tools/audio_midi.py import /path/to/song.mid /path/to/compact.bin
uv run tools/audio_seq.py extract /path/to/compact.bin /path/to/sequence.json
uv run tools/audio_seq.py build /path/to/sequence.json /path/to/rebuilt.bin
```

Export creates a standard Type 1 MIDI preview with native timing hints. Protected
`snap_native_v1_cseq_track_NN` names preserve compact track slots; versioned
sequencer-specific metadata binds native note durations, generated preview
releases and native EOT timing. Keep this metadata when reimporting an export:
native duration timers survive loop jumps and are not ordinary MIDI note-offs.
The importer removes only bound preview releases and preserves real explicit
releases. Partial, stale or conflicting hints fail rather than silently changing
playback. Visible channel messages, velocities and tempos remain the musical
source of truth; hints do not contain an original-song byte snapshot.
The first track, `snap_native_v1_preview_releases`, carries generated releases
before equal-tick events on other tracks. It is preview-only and consumes no
compact slot; keep it first so standard MIDI merging preserves release/pedal
ordering. Its bindings are validated against the editable musical tracks.

Generic MIDI players preview the visible events, not SDK loop execution. Export
normalizes the actual first pass, repeated body and finite-loop tail from the
physical jump target, including compressed repeat-entry state. Unreachable bytes
after an infinite loop remain in the lossless JSON but are not preview music.

Ordinary authored Type 0/1 MIDI does not need native hints. Names `cseq_track_NN`
select compact slots; other tracks are packed into at most 16 playable slots.
Slot names that reverse source-order simultaneous events on a shared channel
are rejected rather than silently changing playback.
Notes pair FIFO per channel and key across synchronous tracks; unpaired notes
are errors. Explicit MIDI releases remain explicit native releases, so
zero-length notes and same-tick sustain ordering are retained. A conductor is
merged into a non-looping slot, or tick-zero tempo is placed before a musical
loop; later conductor changes require a non-looping slot. Channel messages,
tempo and trailing EOT silence are retained. Informational text/time/key-signature
metadata is omitted. Unsupported system events, asynchronous Type 2, SMPTE timing
and unknown markers are rejected.

Markers `loop_start` and `loop_end:N[:C]` specify SDK loop control. N is the reset
count, C is the initial current count (default N), each 0–255; current 255 loops
forever, current 0 falls through. Bare `loop_end` means forever. Loops must be
paired and non-nested per track, and infinite loops must advance time. Physical
back-offsets are recomputed after FE escaping, not copied from an exported song.
With ordinary loop markers, explicit releases must balance their NoteOns in
the same active loop body. Cross-boundary or unreachable releases require
native duration hints; otherwise looping would leak voices or repeat a
one-shot release. Normalized export supports flat stable repeat paths; nested
control flow and observable shared mutable-counter dependencies are rejected.

To use an exported song as ordinary authored MIDI instead, remove all native
sequencer-specific hints and protected track-name prefixes deliberately; its
visible preview releases then become explicit releases, not native timers.
Remove loop markers or balance releases within the authored loop. DAWs that
discard hints cannot preserve native loop/release semantics.

This is native Pokémon Snap `ALCSeq` playback, not a new runtime or new song IDs.
The loader derives playback buffer sizes from table lengths, but the total audio
heap remains fixed at `0x4B000`; large authored songs require capacity review.
No original per-slot size cap is imposed and no unbounded runtime capacity is
promised. MIDI overrides do not change bank or wave-table content.
