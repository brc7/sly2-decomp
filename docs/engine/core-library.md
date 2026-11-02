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

| Span | File (label) | Conf | Notes |
|---|---|---|---|
| 0x1147D8-0x114AE0 | tu_1147D8 | low |  |
| 0x114AE0-0x116918 | tu_114AE0 | low |  |
| 0x116918-0x116CA0 | heap | medium | tu_table said splotheap; PvAllocClear etc. |
| 0x116CA0-0x117030 | dynarray | low | r1 dynarray(?); tu_table said bis; name unknown, sorts before bez |
| 0x117030-0x1177A8 | bez | medium | tu_table said bis |
| 0x1177A8-0x118668 | bis | high |  |
| 0x118668-0x1195E0 | bsp | medium |  |
| 0x1195E0-0x119BC8 | crout | high |  |
| 0x119BC8-0x119DA0 | tu_119BC8 | low | small string class |
| 0x11A050-0x11A550 | dl | high |  |
| 0x11A550-0x11AFE0 | dzgcore | low | dzg-related (second dzg TU at 0x1585D8); arbitration d): util-run dzg LCP core; P2 TU 0x1585D8 keeps "dzg" (real filename unknown, HYPOTHESIS) |
| 0x11AFE0-0x11B1A8 | tu_11AFE0 | low | unknown hash helpers; case-insensitive string hash |
| 0x11B300-0x11CCA8 | mat | high |  |
| 0x11CCA8-0x11CF60 | slotheap | medium |  |
| 0x11CF60-0x11E198 | util | high |  |
| 0x11E198-0x11F440 | vec | medium | tu_table said util |
| 0x132570-0x132B00 | bas | medium |  |
| 0x13D558-0x13DA18 | bq | high |  |
| 0x14B660-0x14B7F8 | tu_14B660 | low | tiny file helpers (ca*) |
| 0x14B7F8-0x14BEA8 | cd | medium | tu_table label, not re-checked |
| 0x1585D8-0x159118 | dzg | high | Sly1 dzg.c order |
| 0x1848A8-0x184F50 | tu_1848A8 | low | bisstream? |
| 0x1AE150-0x1AE7D0 | memory | high | starts at 0x1AE150 (0x1AE130 is memcard $D); 0x1AE130 is memcard _GLOBAL_$D |
| 0x201620-0x201D78 | sprbuf | high |  |
| 0x20D5D0-0x20D9D0 | stream | high |  |
| 0x21D638-0x21EFD0 | text | high |  |
| 0x21EFD0-0x21F2E0 | thread | high |  |
| 0x254F00-0x259DE4 | game_linkonce | medium | game C++ after collect2 reg/dereg_frame: .gnu.linkonce inline/template code (sdk_names NOTES s3, HYPOTHESIS strong); inline/template helpers (smart pointers, VECTOR ops) |

## 7. Key functions

| Addr | Name | Purpose | Conf | Tier | Evidence |
|---|---|---|---|---|---|
| [0x116A58](../../src/auto/CSwHeap_PvRealloc.c) | `CSwHeap_PvRealloc` | vtable slot 2: ignores old pointer, re-dispatches to slot-1 PvAlloc with alignment = this->+4 unless it is -1 | 0.85 | T1 | verified |
| [0x117120](../../src/hand/EvaluateBezierPos.c) | `EvaluateBezierPos` | Cubic Hermite position/velocity/acceleration along a curve segment | 0.90 | T1 | verified |
| [0x117988](../../src/P2/bis.cpp) | `Align__18CBinaryInputStreami` |  | 0.45 | existing | parse |
| [0x117FD0](../../src/hand/ReadGeom__18CBinaryInputStreamP4GEOM.cpp) | `CBinaryInputStream_ReadGeom` | Reads a GEOM: positions, faces (planes), edges with twin links | 0.95 | T1 | parse, verified |
| [0x118220](../../src/hand/CBinaryInputStream_ReadBspc.c) | `CBinaryInputStream_ReadBspc` | Reads a BSP collision tree: U32 node count, allocate (0x1AE2A0), per node U16 plane/children indices | 0.80 | T1 | parse |
| [0x118380](../../src/hand/CBinaryInputStream_ReadStringSw.c) | `CBinaryInputStream_ReadStringSw` | Reads a U16-length-prefixed string into a buffer allocated from a heap/alloc request (Sly2 adds allocator + kind args) | 0.75 | T1 | parse |
| [0x118420](../../src/auto/CBinaryInputStream_SkipStringSw.c) | `CBinaryInputStream_SkipStringSw` | Skips a U16-length-prefixed string (Read(U16Read(),NULL)) | 0.60 | T1 | parse |
| [0x118458](../../src/auto/CBinaryInputStream_ReadCb64.c) | `CBinaryInputStream_ReadCb64` | Read(0x40 bytes) into buffer | 0.75 | T1 | parse |
| [0x118668](../../src/hand/func_00118668.cpp) | `ClsgClipEdgeToBsp` | Clips an edge against a BSP volume; returns inside segments (LSG list) | 0.90 | T1 | verified |
| [0x118A88](../../src/hand/func_00118A88.cpp) | `PruneBsp` | Recursive prune of a BSP against a sphere (pos, radius): emits a reduced tree | 0.88 | T1 | verified |
| [0x119308](../../src/hand/InitPlaneFromPositions.c) | `InitPlaneFromPositions` | Plane through 3 points: normal = normalize((b-a)x(c-a)), d = n.a | 0.85 | T1 | verified |
| [0x1193C0](../../src/auto/SetPlane.c) | `SetPlane` | Stores normal qword and d at +0xC | 0.85 | T1 | verified |
| [0x11A060](../../src/hand/ClearSl.c) | `ClearSl` | pvFirst = pvLast = NULL | 0.85 | T1 | verified |
| [0x11A070](../../src/hand/func_0011A070.c) | `AppendSlEntry` | Append to singly linked list (tail pointer) | 0.85 | T1 | verified |
| [0x11A1B8](../../src/hand/FFindSlEntry.c) | `FFindSlEntry` | True if entry is linked (next != NULL or it is the tail) | 0.85 | T1 | verified |
| [0x11A200](../../src/hand/ClearDl__FP2DL.c) | `ClearDl` | pvFirst = pvLast = NULL | 0.85 | T1 | verified |
| [0x11A210](../../src/hand/ClearDle__FP3DLE.c) | `ClearDle` | pvNext = pvPrev = NULL | 0.85 | T1 | verified |
| [0x11A250](../../src/hand/PrependDlEntry__FP2DLPv.c) | `PrependDlEntry` | Prepend | 0.90 | T1 | verified |
| [0x11A550](../../src/hand/InitDzg.c) | `InitDzg` | Allocates DZ array and matrices for c constraints, inits the constraint DLs | 0.85 | T1 | verified, arbitration |
| [0x11ABF8](../../src/hand/SolveDzgFilled.c) | `SolveDzgFilled` | Moves equality constraints into the active list, EnforceDzgDz, then repeatedly adds the most-violated constraint | 0.60 | T1 | arbitration |
| [0x11AFE0](../../src/hand/HashPchzCaseless.c) | `HashPchzCaseless` | Case-insensitive string hash h = h*33 + toupper(c) (NULL -> default string D_002A4150) | 0.85 | T1 | verified |
| [0x11BA08](../../src/hand/BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3.cpp) | `BuildRotateVectorsMatrix` | Rotation taking direction a onto b: identity if either is shorter than 1e-4, axis = norm(a x b) with perpendicular fallback, clamped cos | 0.75 | T1 | verified, arbitration |
| [0x11C168](../../src/hand/__ml__FRC7MATRIX4T0.c) | `MultiplyMatrix4Matrix4` | 4x4 matrix product | 0.85 | T1 | verified |
| [0x11C950](../../src/hand/SmoothMatrix.cpp) | `SmoothMatrix` |  |  | existing | arbitration |
| [0x11CF60](../../src/hand/RadNormalize__Ff.c) | `RadNormalize` | Wraps an angle into [-pi, pi) using GModPositive | 0.85 | T1 | verified |
| [0x11D018](../../src/hand/GSmooth__FfffP3SMPPf.c) | `GSmooth` | Critically-damped style smoothing of a scalar toward a target with SMP limits | 0.90 | T1 | verified |
| [0x11D448](../../src/hand/GRandInRange__Fff.c) | `GRandInRange` | Uniform float in [gMin, gMax] | 0.88 | T1 | verified |
| [0x11D508](../../src/hand/CSolveQuadratic__FfffPf.c) | `CSolveQuadratic` | Real roots of a x^2 + b x + c (count returned, roots written) | 0.85 | T1 | verified |
| [0x11D608](../../src/hand/CalculateSinCos__FfPfT1.c) | `CalculateSinCos` | sin and cos of an angle (polynomial, clamped) | 0.85 | T1 | verified |
| [0x11D7F0](../../src/hand/GTrunc__Ff.c) | `GTrunc` | Truncate a float toward zero via exponent bit manipulation | 0.85 | T1 | verified |
| [0x11D898](../../src/hand/GModPositive__Fff.c) | `GModPositive` | fmod returning a non-negative result | 0.85 | T1 | verified |
| [0x11D978](../../src/hand/RadLimitLm.c) | `RadLimitLm` | Clamps an angle into the angular range plm (around its midpoint, wrap-aware) | 0.85 | T1 | verified |
| [0x11D9D8](../../src/auto/Force__FPv.c) | `Force` | Empty function taking a pointer (keeps a value alive / forces evaluation) | 0.60 | T1 | verified |
| [0x11DD30](../../src/hand/ConvertVectorToRgba.c) | `ConvertVectorToRgba` | floats*255+0.5 -> RGBA bytes | 0.85 | T1 | verified |
| [0x11E2D8](../../src/hand/GetNormalVectors.c) | `GetNormalVectors` | Two unit vectors perpendicular to a normal (basis rows) | 0.65 | T1 | verified |
| [0x11E630](../../src/hand/FindClosestPointBetweenLines.c) | `FindClosestPointBetweenLines` | Closest points between two infinite lines | 0.85 | T1 | verified |
| [0x11E958](../../src/hand/CalculateVectorPanTilt.c) | `CalculateVectorPanTilt` | pan = atan2(y,x), tilt = atan2(z, \|xy\|), each RadNormalized, optional outputs | 0.80 | T1 | verified |
| [0x11E9F8](../../src/hand/LimitVectorLength.c) | `LimitVectorLength` | Scales v down to length gMax if longer | 0.85 | T1 | verified |
| [0x11F088](../../src/hand/PosSmooth.cpp) | `PosSmooth` | Smooth a position toward a target (existing name) | 0.85 | existing | arbitration |
| [0x13D558](../../src/auto/Init__10CByteQueueiPv.c) | `CByteQueue_Init` | Store buffer size (+4) and buffer (+0) then Reset | 0.92 | T1 | verified |
| [0x13D850](../../src/auto/CbRead__17CQueueInputMemoryiPv.c) | `CQueueInputMemory_CbRead` | Copy min(cb, remaining) bytes from memory source and advance | 0.90 | T1 | verified |
| [0x13D8B8](../../src/auto/CbRead__15CQueueInputFileiPv.c) | `CQueueInputFile_CbRead` | Stub returning 0 (file input unused in retail) | 0.75 | T1 | verified |
| [0x14B9C0](../../src/auto/func_0014B9C0.c) | `FValidCdMmode` | sceCdGetDiskType cached into global, returns ==0x14 (PS2DVD); identical to Sly1 decomp source | 1.00 | T1 | verified |
| [0x14BC78](../../src/hand/func_0014BC78.c) | `ReadCdDirect` | Loop sceCdRead until 1, else sceCdSync(0)+error, as Sly1 ReadCdDirect; Sly2 adds a 4th int arg choosing the read mode | 0.95 | T1 | verified |
| [0x1585D8](../../src/hand/InitDzgXp.c) | `InitDzgXp` | Initialises a DZG constraint group: base init (0x11A550) with 2*cdz rows, stores four params, allocates 0x60 bytes per row | 0.50 | T1 | arbitration |
| [0x158728](../../src/hand/FillDzgDz.cpp) | `FillDzgDz` | Fills one constraint row: lets each of the two bodies add itself (virtual), computes per-SO relative vectors and the dot products against existing row | 0.70 | T1 | verified |
| [0x158988](../../src/hand/SolveDzg__FP3DZGiiPP2XPPiPPP2SO.cpp) | `SolveDzg__FP3DZGi` |  | 0.80 | existing | arbitration; replaces existing SolveDzg__FP3DZGiiPP2XPPiPPP2SO |
| [0x158B20](../../src/hand/SolveDzgFric.cpp) | `SolveDzgFric` | Friction pass: repeatedly SolveDzg, then adds a friction row for the free contact with the largest tangential push, clamped by the material's friction | 0.80 | T1 | verified |
| [0x1AE1C8](../../src/auto/CheckForOutOfMemory__Fv.c) | `CheckForOutOfMemory__Fv` | Empty out-of-memory hook (release build) | 0.90 | existing | verified |
| [0x1AE2A0](../../src/hand/func_001AE2A0.c) | `PvAllocSwClearImpl` | PvAllocSwImpl then memset 0 | 0.93 | T1 |  |
| [0x1AE318](../../src/auto/func_001AE318.c) | `PvAllocStackImpl` | Stack (scratch) heap allocator: D_002F6388 = (D_002F6388 - align16(cb)) & -cbAlign; grows down | 0.90 | existing | verified, arbitration; replaces existing PvAllocSlot__FP8ALLOCREQ |
| [0x1AE4F8](../../src/hand/func_001AE4F8.c) | `__builtin_new` | operator new: PvAllocSwClearImpl({cb 16}) | 0.95 | T1 |  |
| [0x21E0D8](../../src/hand/func_0021E0D8.c) | `ChDigitFromG` | digit character of g at decimal position a (precision b) | 0.85 | T1 | verified |
| [0x21EDF8](../../src/hand/func_0021EDF8.c) | `strchr` | libc-style strchr game copy | 0.85 | T1 | verified |
| [0x21F0C0](../../src/auto/func_0021F0C0.c) | `LeaveCritSect` | decrement enter count; at 0 clear owner and SignalSema | 0.90 | T1 | verified |
| [0x21F100](../../src/hand/func_0021F100.c) | `StartupThread` | creates the worker thread (stack 0x1000, prio 2, entry 0x160B70) after raising main prio to 5 | 0.90 | T1 | verified |
| [0x2584A8](../../src/hand/func_002584A8.c) | `LoadEmitblipColorsFromBrx` | read count + RGBA list into EMITB [game linkonce (inline/template)] | 0.75 | T1 | parse |
| [0x258598](../../src/lib/func_00258598.c) | `LoadEmitterFromBrx` | EMITTER vt14: alloc+InitEmitb, LoadXfmFromBrx, options, blip colours [game linkonce (inline/template)] | 0.75 | T1 | parse |
| [0x258F40](../../src/hand/func_00258F40.c) | `LoadExploBlipColorsFromBrx` | EXPLO twin of 0x2584A8 [game linkonce (inline/template)] | 0.75 | T1 | parse |
| [0x259030](../../src/lib/func_00259030.c) | `LoadExploFromBrx` | EXPLO/EXPLS vt14 (template twin of LoadEmitterFromBrx) [game linkonce (inline/template)] | 0.75 | T1 | parse |

Selected 60 of 460 named functions (T1 with confidence >= 0.85, or runtime/verified/parse/arbitration evidence, or cited in the text). Full list: [reference/functions/core-library.md](../reference/functions/core-library.md).

## Open questions
- Names of the small unnamed TUs 0x116CA0, 0x119BC8, 0x11AFE0 (string hash h\*33+toupper also used by the save schema).
- `CheckForOutOfMemory`: really `__Fv` when callers pass a size?
- Whether the PS2 LZSS ring's uninitialised scratchpad start ever matters (tools zero-fill).
