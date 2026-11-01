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

| Span | File (label) | Notes |
|---|---|---|
| 0x1147D8-0x114AE0 | ? |  |
| 0x114AE0-0x116918 | ? |  |
| 0x116918-0x116CA0 | heap |  |
| 0x116CA0-0x117030 | dynarray? | name unknown, sorts before bez |
| 0x117030-0x1177A8 | bez |  |
| 0x1177A8-0x118668 | bis |  |
| 0x118668-0x1195E0 | bsp |  |
| 0x1195E0-0x119BC8 | crout |  |
| 0x119BC8-0x119DA0 | tu_119BC8 | small string class |
| 0x11A050-0x11A550 | dl |  |
| 0x11A550-0x11AFE0 | dzg (part 1) |  |
| 0x11AFE0-0x11B1A8 | tu_11AFE0 | case-insensitive string hash |
| 0x11B300-0x11CCA8 | mat |  |
| 0x11CCA8-0x11CF60 | slotheap |  |
| 0x11CF60-0x11E198 | util |  |
| 0x11E198-0x11F440 | vec |  |
| 0x132570-0x132B00 | bas |  |
| 0x13D558-0x13DA18 | bq |  |
| 0x14B660-0x14B7F8 | tu_14B660 (file helpers) |  |
| 0x14B7F8-0x14BEA8 | cd |  |
| 0x1585D8-0x159118 | dzg (part 2) |  |
| 0x1848A8-0x184F50 | tu_1848A8 (stream BIS) |  |
| 0x1AE130-0x1AE7D0 | memory | 0x1AE130 is memcard _GLOBAL_$D |
| 0x201620-0x201D78 | sprbuf |  |
| 0x20D5D0-0x20D9D0 | stream |  |
| 0x21D638-0x21EFD0 | text |  |
| 0x21EFD0-0x21F2E0 | thread |  |

## 7. Key functions

| Addr | Name | Purpose | Conf | Status |
|---|---|---|---|---|
| [0x116A58](../../src/auto/func_00116A58.c) | `CSwHeap::PvRealloc` | vtable slot 2: ignores old pointer, re-dispatches to slot-1 PvAlloc with alignment = this->+4 unless it is -1 | 0.55 | VERIFIED |
| [0x117120](../../src/hand/func_00117120.c) | `EvaluateBezierPos` | Cubic Hermite position/velocity/acceleration along a curve segment | 0.90 | VERIFIED |
| [0x117FD0](../../src/hand/func_00117FD0.cpp) | `CBinaryInputStream::ReadGeom` | Reads a GEOM: positions, faces (planes), edges with twin links | 0.90 | VERIFIED |
| [0x118668](../../src/hand/func_00118668.cpp) | `ClsgClipEdgeToBsp` | Clips an edge against a BSP volume; returns inside segments (LSG list) | 0.90 | VERIFIED |
| [0x118A88](../../src/hand/func_00118A88.cpp) | `PruneBsp` | Recursive prune of a BSP against a sphere (pos, radius): emits a reduced tree | 0.88 | VERIFIED |
| [0x119308](../../src/hand/func_00119308.c) | `InitPlaneFromPositions` | Plane through 3 points: normal = normalize((b-a)x(c-a)), d = n.a | 0.50 | VERIFIED |
| [0x1193C0](../../src/auto/func_001193C0.c) | `SetPlane` | Stores normal qword and d at +0xC | 0.45 | VERIFIED |
| [0x11A060](../../src/hand/func_0011A060.c) | `ClearSl` | pvFirst = pvLast = NULL | 0.70 | VERIFIED |
| [0x11A070](../../src/hand/func_0011A070.c) | `AppendSlEntry` | Append to singly linked list (tail pointer) | 0.75 | VERIFIED |
| [0x11A1B8](../../src/hand/func_0011A1B8.c) | `FFindSlEntry` | True if entry is linked (next != NULL or it is the tail) | 0.65 | VERIFIED |
| [0x11A1F8](../../src/auto/func_0011A1F8.c) | `InitDl` | DL init: ibDle at +8 | 0.85 | HYP |
| [0x11A200](../../src/hand/func_0011A200.c) | `ClearDl` | pvFirst = pvLast = NULL | 0.80 | VERIFIED |
| [0x11A210](../../src/hand/func_0011A210.c) | `ClearDle` | pvNext = pvPrev = NULL | 0.80 | VERIFIED |
| [0x11A220](../../src/hand/func_0011A220.c) | `AppendDlEntry` | Append to doubly linked list | 0.92 | HYP |
| [0x11A250](../../src/hand/func_0011A250.c) | `PrependDlEntry` | Prepend | 0.90 | VERIFIED |
| [0x11A280](../../src/hand/func_0011A280.c) | `InsertDlEntryBefore` | Insert before pvNext (NULL -> append, first -> prepend) | 0.88 | HYP |
| [0x11A2E8](../../src/hand/func_0011A2E8.c) | `RemoveDlEntry` | Unlink and fix up iterators on g_pdliFirst chain | 0.92 | HYP |
| [0x11A380](../../src/hand/func_0011A380.c) | `FFindDlEntry` | True if pv is linked (next or tail) | 0.85 | HYP |
| [0x11A3B0](../../src/auto/func_0011A3B0.c) | `FIsDlEmpty` | pvFirst == NULL | 0.85 | HYP |
| [0x11A550](../../src/hand/func_0011A550.c) | `InitDzg` | Allocates DZ array and matrices for c constraints, inits the constraint DLs | 0.85 | VERIFIED |
| [0x11AFE0](../../src/hand/func_0011AFE0.c) | `HashPchzCaseless` | Case-insensitive string hash h = h*33 + toupper(c) (NULL -> default string D_002A4150) | 0.40 | VERIFIED |
| [0x11CF60](../../src/hand/func_0011CF60.c) | `RadNormalize` | Wraps an angle into [-pi, pi) using GModPositive | 0.85 | VERIFIED |
| [0x11D018](../../src/hand/func_0011D018.c) | `GSmooth` | Critically-damped style smoothing of a scalar toward a target with SMP limits | 0.90 | VERIFIED |
| [0x11D448](../../src/hand/func_0011D448.c) | `GRandInRange` | Uniform float in [gMin, gMax] | 0.88 | VERIFIED |
| [0x11D508](../../src/hand/func_0011D508.c) | `CSolveQuadratic` | Real roots of a x^2 + b x + c (count returned, roots written) | 0.60 | VERIFIED |
| [0x11D608](../../src/hand/func_0011D608.c) | `CalculateSinCos` | sin and cos of an angle (polynomial, clamped) | 0.80 | VERIFIED |
| [0x11D7F0](../../src/hand/func_0011D7F0.c) | `GTrunc(float)` | Truncate a float toward zero via exponent bit manipulation | 0.75 | VERIFIED |
| [0x11D898](../../src/hand/func_0011D898.c) | `GModPositive` | fmod returning a non-negative result | 0.80 | VERIFIED |
| [0x11D978](../../src/hand/func_0011D978.c) | `RadLimitLm` | Clamps an angle into the angular range plm (around its midpoint, wrap-aware) | 0.50 | VERIFIED |
| [0x11D9D8](../../src/auto/func_0011D9D8.c) | `Force` | Empty function taking a pointer (keeps a value alive / forces evaluation) | 0.60 | VERIFIED |
| [0x11DD30](../../src/hand/func_0011DD30.c) | `ConvertVectorToRgba` | floats*255+0.5 -> RGBA bytes | 0.45 | VERIFIED |
| [0x11E2D8](../../src/hand/func_0011E2D8.c) | `GetNormalVectors` | Two unit vectors perpendicular to a normal (basis rows) | 0.65 | VERIFIED |
| [0x11E630](../../src/hand/func_0011E630.c) | `FindClosestPointBetweenLines` | Closest points between two infinite lines | 0.60 | VERIFIED |
| [0x11E958](../../src/hand/func_0011E958.c) | `CalculateVectorPanTilt` | pan = atan2(y,x), tilt = atan2(z, \|xy\|), each RadNormalized, optional outputs | 0.80 | VERIFIED |
| [0x11E9F8](../../src/hand/func_0011E9F8.c) | `LimitVectorLength` | Scales v down to length gMax if longer | 0.75 | VERIFIED |
| [0x13D558](../../src/auto/func_0013D558.c) | `CByteQueue::Init` | Store buffer size (+4) and buffer (+0) then Reset | 0.92 | VERIFIED |
| [0x13D850](../../src/auto/func_0013D850.c) | `CQueueInputMemory::CbRead` | Copy min(cb, remaining) bytes from memory source and advance | 0.90 | VERIFIED |
| [0x13D8B8](../../src/auto/func_0013D8B8.c) | `CQueueInputFile::CbRead` | Stub returning 0 (file input unused in retail) | 0.75 | VERIFIED |
| [0x158728](../../src/hand/func_00158728.cpp) | `FillDzgDz` | Fills one constraint row: lets each of the two bodies add itself (virtual), computes per-SO relative vectors and the dot products against existing row | 0.70 | VERIFIED |
| [0x158B20](../../src/hand/func_00158B20.cpp) | `SolveDzgFric` | Friction pass: repeatedly SolveDzg, then adds a friction row for the free contact with the largest tangential push, clamped by the material's friction | 0.80 | VERIFIED |
| [0x1AE168](../../src/hand/StartupMemMgr__Fv.c) | `StartupMemMgr` | Init heaps: world min/mac = align16(_end), stack min/mac = 0x2000000, stack index -1, InitCritSect | 0.98 | HYP |
| [0x1AE1C8](../../src/auto/CheckForOutOfMemory__Fv.c) | `CheckForOutOfMemory` | Empty out-of-memory hook (release build) | 0.90 | VERIFIED |
| [0x1AE1D0](../../src/hand/func_001AE1D0.c) | `PvAllocSwImpl` | World (level) heap bump allocator: align up D_002F6384 to req.cbAlign, advance by align16(cb) | 0.92 | HYP |
| [0x1AE238](../../src/hand/func_001AE238.c) | `PvAllocSwCopyImpl` | PvAllocSwImpl then CopyAb(dst src cb) | 0.92 | HYP |
| [0x1AE2A0](../../src/hand/func_001AE2A0.c) | `PvAllocSwClearImpl` | PvAllocSwImpl then memset 0 | 0.93 | HYP |
| [0x1AE300](../../src/auto/FreeSw__Fv.c) | `FreeSw` | Reset world heap top to world heap min (level unload) | 0.97 | HYP |
| [0x1AE318](../../src/auto/func_001AE318.c) | `PvAllocStackImpl` | Stack (scratch) heap allocator: D_002F6388 = (D_002F6388 - align16(cb)) & -cbAlign; grows down | 0.93 | VERIFIED |
| [0x1AE378](../../src/hand/func_001AE378.c) | `PvAllocStackClearImpl` | PvAllocStackImpl then memset 0 | 0.92 | HYP |
| [0x1AE3D8](../../src/hand/InitStackImpl__Fv.cpp) | `InitStackImpl` | EnterCritSect and push stack-heap mark | 0.97 | HYP |
| [0x1AE428](../../src/hand/FreeStackImpl__Fv.cpp) | `FreeStackImpl` | Pop stack-heap mark and LeaveCritSect | 0.97 | HYP |
| [0x1AE4E8](../../src/auto/malloc.c) | `malloc` | Returns NULL (libc malloc disabled) | 0.97 | HYP |
| [0x1AE4F0](../../src/auto/free.c) | `free` | No-op | 0.97 | HYP |
| [0x1AE4F8](../../src/hand/func_001AE4F8.c) | `__builtin_new` | operator new: PvAllocSwClearImpl({cb 16}) | 0.95 | HYP |
| [0x1AE520](../../src/auto/__builtin_delete.c) | `__builtin_delete` | operator delete no-op | 0.97 | HYP |
| [0x1AE528](../../src/hand/CopyAqw__FPvT0i.cpp) | `CopyAqw` | Quadword copy (remainder then 4x unrolled) | 0.95 | HYP |
| [0x1AE5A0](../../src/hand/CopyAb__FPvT0Ui.cpp) | `CopyAb` | Byte/word memcpy | 0.95 | HYP |
| [0x21E0D8](../../src/hand/func_0021E0D8.c) | `ChDigitFromG` | digit character of g at decimal position a (precision b) | 0.85 | VERIFIED |
| [0x21EDF8](../../src/hand/func_0021EDF8.c) | `strchr` | libc-style strchr game copy | 0.70 | VERIFIED |
| [0x21F0C0](../../src/auto/func_0021F0C0.c) | `LeaveCritSect` | decrement enter count; at 0 clear owner and SignalSema | 0.90 | VERIFIED |
| [0x21F100](../../src/hand/func_0021F100.c) | `StartupThread` | creates the worker thread (stack 0x1000, prio 2, entry 0x160B70) after raising main prio to 5 | 0.90 | VERIFIED |

Selected 60 of 270 named functions (confidence >= 0.85, verified, or flagged). Full list: [reference/functions/core-library.md](../reference/functions/core-library.md).

## Open questions
- Names of the small unnamed TUs 0x116CA0, 0x119BC8, 0x11AFE0 (string hash h\*33+toupper also used by the save schema).
- `CheckForOutOfMemory`: really `__Fv` when callers pass a size?
- Whether the PS2 LZSS ring's uninitialised scratchpad start ever matters (tools zero-fill).
