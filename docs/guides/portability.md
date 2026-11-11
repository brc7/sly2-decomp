# Portability register

Every mechanism this repository has for compiling its sources somewhere other than the EE (the native 64-bit port
builds these files with clang), what it does in the matching build, what a native build does with it, and why the
matching form is not simply the portable one. The rule behind all of them: the PS2 build stays byte-identical, so a
hook expands on the EE to exactly the code that was there before, and the portable form lives beside it.

History: `git log --grep='port request'` (v1.10 onwards): g_sndmgr's full definition, CStreamBis in a header,
SW_ctor slot sizes, Init list-link offsets, pointer-array and record sizes, sizeof(IKREC), uptr/sptr on the load
path, heap bounds and SLY2_OFFSETOF as hooks, level-load records with their real layouts, then (v1.13) the option
table offsets, class-descriptor sizes, LO's tagged update word, the ik/pipe_jt/gait views, SLY2_EE_LAYOUT with the
colour-table and bitmap file records, and breakable's pointer-wide action records.

## 1. Guards and hooks

| hook | EE (matching build) | native build | where |
|---|---|---|---|
| `SLY2_EE_ASM` (sly2/port.h) | defined (`__R5900__`, unless `SLY2_NO_EE_ASM`): inline EE assembly allowed | undefined: the `#else` plain-C body is compiled | `grep -rn SLY2_EE_ASM src` (sw.cpp, path.cpp, gs.cpp) |
| `SLY2_EE_LAYOUT` (sly2/port.h) | defined (`__R5900__`, unless `SLY2_NO_EE_LAYOUT`): EE data layout (4-byte pointers, file records read straight into in-memory structs) and g++ 2.95 vtables | undefined: `#else` reads file records field by field, uses plain virtual calls | shd.cpp LoadClutFromBrx / LoadBmpFromBrx, gait.cpp StartLeap's slot-35 call |
| `uptr`, `sptr` (sly2/port.h) | `unsigned int`, `int` | `__UINTPTR_TYPE__`, `__INTPTR_TYPE__` (override: `SLY2_UPTR_TYPE`, `SLY2_SPTR_TYPE`) | `grep -rlwn 'uptr\|sptr' src include` |
| `SLY2_OFFSETOF(T, m)` (sly2/basic.h) | `((int)(char *)&((T *)0)->m)` | define first, e.g. `((int)__builtin_offsetof(T, m))` | layout asserts, list-link offsets, g_aeopid's `EOPID_OFF` |
| `SLY2_STORE_VPTR_AT`, `SLY2_SET_VPTR`, `SLY2_SET_VPTR_AT`, `SLY2_STORE_VPTR`, `SLY2_VTBL` (sly2/vtables.h) | word store of `_vt$<n><CLASS>` at +0xC (BASIC), +0x18 (ACT), +0x30 (CBinaryInputStream), +0xEC (RIP), 0 | override `SLY2_STORE_VPTR_AT` alone (vptr first in the Itanium ABI), or let the constructors become real ones | `grep -rln 'SLY2_SET_VPTR\|SLY2_STORE_VPTR' src` (24 files) |
| `SLY2_HW_*`, `SLY2_UNCACHED`, `SLY2_UNCACHED_ACCEL`, `SLY2_HW_OVERRIDE` (sly2/hw.h) | the PS2 addresses as integer constants (scratchpad 0x70000000, GS/DMA/VIF registers, uncached mirrors) | define the ones it emulates first, in the header named by `-DSLY2_HW_OVERRIDE='"port_hw.h"'` | gs.cpp, sprbuf.cpp, sw.cpp, hud.cpp, glob.cpp, mpeg.cpp |
| `SLY2_HEAP_BEGIN`, `SLY2_HEAP_END` (platform/memory/memory.cpp) | the end of the program image (`D_0047220F`) and the top of the 32 MB RAM (0x02000000) | a static buffer's bounds | memory.cpp StartupMemMgr |
| `CStreamBisStorage` (sly2/streambis.h) | room for a CStreamBis without g++'s implicit constructor; the callers call `CStreamBis_ctor` / `_$_10CStreamBis` themselves | provide those two C entry points (or override the two declarations); sizes follow `sizeof(CStreamBis)` | sw.cpp, hud.cpp (the class itself: brx_sw.cpp, memcard.cpp) |
| EE layout asserts (the `typedef char assert_...[cond ? 1 : -1]` lines) | compile-time checks of the EE offsets and sizes | not true on a 64-bit host: the port rewrites them into a non-failing sink (its own generator) | every sly2/ header, many TUs |
| `EOPID_PMF`, `EOPID_PMFI`, `EOPID_PMFT`, `EOPID_VPMF`, `EOPIDW` (app/class_registry_options.inc) | g++ 2.95 pointers to member functions: `{short delta; short index; pfn or delta2}` | redefine the four macros and `EOPIDW` in the native PMF ABI (Itanium: `{ptr, adj}`, ptr = address or 1 + vtable offset) | class_registry_options.inc (documented next to the macros) |

Why the matching form cannot be the portable one:

- **Inline assembly** (`SLY2_EE_ASM`): VU0 macro-mode instructions (`vmulax`, `vsqrt`, `lqc2`, `qmfc2`...), the
  128-bit MMI moves and the register pins that fix g++'s allocation cannot be produced from C by ee-gcc 2.95; the
  asm is the only spelling that matches. The C equivalent is semantics only.
- **File records** (`SLY2_EE_LAYOUT`): the level file stores colour tables and bitmaps as 0x10 / 0x18-byte records
  with 32-bit offsets, and the game reads them with `Read(sizeof(CLUTHDR), pclut)`: `sizeof(CLUTHDR)` equals the
  on-disk size only with 4-byte pointers. shd.h's `CLUTFILE` / `BMPFILE` are the file records (asserted on every
  target); the `#else` path reads one and widens it. Writing the EE path through the file struct would add copies
  and change the code.
- **Hand-written vtable calls** (`SLY2_EE_LAYOUT`): a few functions load the vtable entry (`VTE`: delta, pfn) before
  the arguments are built, which a plain virtual call does not (the schedule differs). That spelling depends on
  g++ 2.95's vtable format and the vptr at +0xC. ik.cpp and pipe_jt.cpp's calls matched as real virtual calls and
  are plain C++ now; gait.cpp's slot 35 (SO::SetVelocity) did not and is guarded.
- **Pointer-sized integers** (`uptr`, `sptr`): the matched code casts pointers to `int` / `unsigned`; the signedness
  picks the EE instructions (an unsigned `& ~0x3F` needs a 64-bit mask), so `int` becomes `sptr` and `unsigned`
  becomes `uptr`, never the other way round. breakable.cpp's action record `I {int a; sptr b}` is one: action kind
  0xC keeps a pointer in `b`.
- **Offsets and sizes** (`SLY2_OFFSETOF`, `sizeof`): literals were the decomp's first spelling; the EE value is the
  same, so these are free. g_aeopid's member offsets are `EOPID_OFF(class, member)` and the class descriptors' `cb` is
  `sizeof(class)` (CloneSaa copies `instanceSize` bytes).
- **vptr stores**: constructors written as C functions (for asm labels, views or call order) store the vtable
  themselves; g++ 2.95 puts the vptr after the first base's data, the Itanium ABI first.
- **LO's tagged update word** (LO+0x40, `unknown40` / `ploupd`): either an LOUPD pointer or the flags with bit 0 set.
  Readers take it as `uptr` through `LO::ploupd` and write the flags through `LOUPD::grf` or `LO::unknown40`; LO's own
  `int unknown40` stays (making it unsigned would change sign extensions elsewhere). On a little-endian host the low
  word holds the tag.

## 2. Known remaining hazards

From the v1.12 list (scratch REMAINING.md), minus what v1.13 fixed:

- **g_aeopid**: 222 member offsets (unique class/offset) stay `EOPID_INT`: they fall inside class pads (the `char pad...[]`
  blocks in sly2/classes.h: JSG, BOMB, TURRET, CID96, CID46, CID121, CID112, CID105, MISSILE, LASEN, BOOTY, CID37...) or point
  into TU-private types (CPD in tn.cpp, BOOTYE in booty.cpp, AMBSND in amb.cpp, SFX/SFXHOLDER and EMITTERV in
  emitter.cpp) or at the interior pointers emitter.h's accessors return. Object-reference options (otyp 0x2xxx) whose
  header member is not a pointer: JOB::stateRequester, TASK::stateRequester, NEELA::giveUpBinding.oid,
  STEPGUARD::noiseType, STEPPLAYER::jtair.unknown160, GRABSPOT::nGroup (PostLoadOidField stores a pointer there).
  The OwnFields accessors (GetPipeOwnFields and the like) return `p + sizeof(Base)`; natively they should be `static_cast<GRABSPOT *>(p)`.
- **Literal sizes**: SW_ctor heaps not yet sizeof (Event/Callback 0x14, Refnode 0xC, Xplist/Xp 0xC0, Act 0x80, Ssge, Jsg,
  Xfh, Leap, Diract, Blip 0x10C0, the blip splines) and about 15 SW_ctor list offsets, slotheap.h
  engine/Splice pools, the post-load pool, hud sprite records, PATHZONE dlRegent, DIALOGEVENT, BOOTYE 0x2C.
- **Dangerous views**: water.cpp SOW (a flat SO/WATER view with hand vtable calls through `VE`; the 64-bit word at
  +0x148 has no single member) and rip.cpp `M_ProjectRipTransform::S` (a RIP view: different pointer slots before
  +0x40, local M3/PHYS types over RIPM3 and the motion record, asm with literal offsets). Other flat views with
  void * pads (BLIPGV, BOOTYV, FADEV, TRAIL, SOV, LASENV) only read LO's tagged word through LO now; their other
  fields are still at EE offsets.
- **glob.cpp**: DMA chains keep 32-bit EE addresses: CopyRelocateDmaChain adds an `int` delta to REF tags' address
  words, and the shader packet tags pack `(ulong)(int)pshd->pbPacket << 32`. A native build needs an address model
  for chains (offsets from a heap base, or an emulated 4 GB EE address space) before its DMA/VIF interpreter can
  follow them; not a decomp-side change.
- **Inline asm without a C fallback**: VU0 math (mat.cpp, vec.cpp, xf.cpp's XF value builders), shd.cpp
  ColorCorrectVector / BuildShaderGifPackets / AddBitmapToHash / AddClutToHash, booty SetBootyState, rip LaunchMatch,
  gs.cpp sync/ei, the cane/murray/sticky/ik loaders; register pins (clang rejects about 400).
- **int to pointer**: about 23 (mpeg `pm->n8`, cane.cpp `(char *)0x002e0000`); breakable.cpp
  AddBreakableTriggerClass returns its record pointer as `int`.

## 3. Future clean port

Guards stay in the simplest form: `#if defined(SLY2_EE_LAYOUT)` / `#ifdef SLY2_EE_ASM`, `#else`, `#endif`, one macro per condition,
never nested inside another portability guard and never compound (`&&`, `||`). That keeps them mechanical: a port
can produce a flag-free native source tree with unifdef,

```sh
unifdef -USLY2_EE_ASM -USLY2_EE_LAYOUT -m $(git ls-files 'src/*.cpp' 'src/*.c' 'include/*.h')
```

and then simplify freely (drop the EE paths, the asm labels and the layout asserts). The definitions in sly2/port.h
are the only compound conditions (`__R5900__` and not the opt-out macro), and they are what `-U` replaces. A new
guard follows the same pattern: one macro in sly2/port.h, defined on the R5900 compiler, with an opt-out.
