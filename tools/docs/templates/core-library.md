# Core library: memory, containers, math, streams, CD, threads, text

The utility layer linked first (0x116918-0x11F440) plus memory, CD, stream, thread and text files. Mostly direct Sly 1 counterparts.
Related: [formats](../formats/disc-records.md) (what the CD and stream code read).

## 1. Memory

- **Two bump heaps, no free lists** (memory.cpp 0x1AE130) `[H r5 memory.md]` (Sly 1 memory.c):
  - *World heap* grows up from align16(`_end`): D_002F6380 min, D_002F6384 top; `FreeSw` resets it at level unload; all `new` goes here (zeroed, 16-aligned).
  - *Stack heap* grows down from 0x2000000 (D_002F6388), scoped by InitStackImpl/FreeStackImpl and an RAII helper (team name STACKSCOPE, 40 users).
  - `malloc` returns NULL, `free`/`delete` are no-ops. Allocation takes an `{cb, cbAlign}` request. `CbFreeMem` 0x1AE150 (gap between the heaps) gates movie playback.
  - Fixes: 0x1AE318 `PvAllocSlot` -> `PvAllocStackImpl`; 0x1572E0 `AllocSw__4DMASi` -> `AllocStack__4DMASi` `[C:verified]`.
- **HEAP interface** (heap.cpp 0x116918; vptr at +0: slots PvAlloc, PvRealloc, Free): `CLinearHeap` (Free is a no-op) and a heap for D_002DD590 that
  bump-allocates from the world heap; `PvAllocClear`, `PvAllocCopy`, `PchzAllocCopy` `[H r1 core_lib.md]`.
- **Slot heaps** (slotheap.cpp 0x11CCA8): fixed-size pools with lazy block allocation (`InitSlotheap`, `PvAllocSlotheapClear`, `FreeSlotheapPv`),
  used by every TU's static init (Splice pools) and by CRef boxes `[H r1]`. Many engine pools (LOROOTs, callbacks, sounds, stimuli, JSGs) are slot heaps.
- **CDynArray** (0x116CA0, file "dynarray?"): Free/Reserve/Append + memcmp comparators by size `[H r1]`.

## 2. Containers

dl.cpp (0x11A050): Sly 1's doubly linked list API (`DL {pvFirst, pvLast, ibDle}`, `DLE {next, prev}`, iterator fix-up via `g_pdliFirst`; `InitDl`..`FIsDlEmpty`, `CPvDl`)
plus a **new singly linked list SL** `{pvFirst, pvLast, ibNext}` used by the SW class lists, oid hash and scheduled callbacks `[H r1]`, layouts `[C:runtime]`.
`AppendDlEntry` = 0x11A220, `RemoveDlEntry` = 0x11A2E8 `[H r8, r4]`. CByteQueue (bq.cpp 0x13D558) for streaming `[H r2b bq.md]`.

## 3. Math

mat.cpp (rotation builders, look-at, decompositions, 4x4 ops; `operator*` order as Sly 1), util.cpp (RadNormalize..MinimizeRange, smoothing,
limits, RGB/HSV), vec.cpp (Sly 1 vec.c order plus ray/triangle helpers; PosSmooth moved here), bez.cpp (Bezier float/pos/mat evaluation),
crout.cpp (Crout LU solve), bsp.cpp (BSP clipping and point tests), dzg.cpp (LCP solver) `[H r1 core_lib.md]` (several verified: EvaluateBezierPos,
PruneBsp, ClsgClipEdgeToBsp). Sly 2 passes VU vectors **by value** where Sly 1 passed `VECTOR*`; Sly 1 mangled signatures must not be applied blindly `[C:verified]` (verify A1).
Fix: 0x11C168 is `operator*(MATRIX4, MATRIX4)`, not MultiplyMatrix4Vector `[C:verified]`.

## 4. Binary input streams and CD

- `CBinaryInputStream` (`src/P2/bis.cpp`, 0x1177A8): base `CBinaryStreamBase` with vptr at +0x30 (`Fill()`, `Close()`); readers U8/U16/U32/S8/S16/S32/F32,
  vectors/matrices, `Align`, `Skip`, `Read`, string16; LZSS decompression into a window (`SetDecompress` 0x1179D8, `Decompress` 0x1179F0) `[C:static]`.
  API names carried over from Sly 1, layout not `[C:static layouts.md]`.
- A CD-backed stream subclass (TU 0x1848A8, vtable D_002D2918; class name coined CStreamBis) is used for all world/file loads; `FOpenSector` 0x184D98 `[H r4b bisstream.md]`.
- `CBinaryAsyncStream` (bas.cpp 0x132570) spools FMV data `[C:static]`.
- cd.cpp (0x14B7F8): `StartupCd`, `UpdateCd` 0x14BA38 (per frame; disc check CRC of one sector), `CdPath`, `FCdCompleted`; the only sector reader is
  `sceCdRead` through the retry wrapper 0x14BC78 `[C:static disc_toc.md]`. File helpers (0x14B660): `PchzFidName` 0x14B690 returns "fid:#unknown" (no names on disc).

## 5. Threads, text, streams

thread.cpp (semaphores, critical sections, `StartupThread`), text.cpp (`CchOstrmPrintf`, `printf`, `sprintf`), stream.cpp (OSTRM output streams) are existing-symbol TUs `[C:static]`.

## 6. Source files

{{TUS}}

## 7. Key functions

{{KEYFUNCS:max=60 minc=0.85}}

## Open questions
- Names of the small unnamed TUs 0x116CA0, 0x119BC8, 0x11AFE0 (string hash h\*33+toupper also used by the save schema).
- `CheckForOutOfMemory`: really `__Fv` when callers pass a size?
- Whether the PS2 LZSS ring's uninitialised scratchpad start ever matters (tools zero-fill).
