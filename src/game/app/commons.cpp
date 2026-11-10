// commons.cpp -- the game's common symbols, defined once, in the order the original link allocated them.
// Not an original translation unit: a data-only file (no functions) that tools/link.py links like the TUs
// (docs/guides/full-link.md, "Absolute symbols"; docs/guides/tu-merging.md, "Phase 9: the commons file").
//
// g++ 2.95 emits an uninitialized class or template static data member as COMMON in every file that instantiates
// it (and the sentry words of the template statics' guarded initialization likewise), and GNU ld allocated the
// commons after every object's .bss: those of 8 bytes or less in .sbss (its default -G 8), the rest at the end of
// .bss, in its own order (symbol hash-table order, not file order). No placement of objects reproduces that order,
// so this file defines each one as an ordinary (strong) object, in address order, and the commons the other files
// still emit resolve to these definitions:
//   .sbss 0x2DD280..0x2DD31C (the whole section): words of 4 bytes, no gaps;
//   .bss  0x452098..0x452200 (the end of .bss, after collect2's): the block tables, slot pools and splice heaps.
// `__attribute__((section(...)))` puts each object in its block and keeps g++ from emitting it as COMMON (a
// definition with a section name is never common); with -G0 nothing would go to .sbss. Every definition has a zero
// initializer: g++ emits initialized definitions in source order (explicit instantiations of template statics it
// would emit after everything else), and zeros in a NOBITS section stay NOBITS.
//
// Kinds of definitions:
// - CGcObj<T>'s list words and block tables (sly2/gc.h): template static data member definitions (explicit
//   specializations for the six collected kinds).
// - The slot pools and splice heaps (sly2/slotheap.h): constructor-less storage (a SLOTHEAP) under the template
//   static's mangled name. A C++ definition of CSlotPool<>::s_pool would make g++ generate its constructor call here,
//   code the ELF does not have: every TU's StaticInit constructs the pools through the sentries.
// - The sentries (`__sn` + the template static's mangled name): g++'s guard words; an int under that name.
// - The other small commons (class statics whose class is not known): storage under the address name.
// The 15 commons of the SDK sources (989snd's and libkernel's buffers, 0x443800..0x450000) are not here: they lie in
// the SDK libraries' .bss, between other SDK objects' data, not in the commons area (full-link.md).
#include "sly2/gc.h"

#define SBSS __attribute__((section(".sbss")))
#define BSS __attribute__((section(".bss")))

struct SLI;
struct DLI;

// ---- .sbss (0x2DD280..0x2DD31C) ----
// The six kinds' list words: all nodes newest first, blocks in use, the sweep's recycle cursor, the root list.
template <> CPair *CGcObj<CPair>::s_ptFirst SBSS = 0;                       /* 0x2DD280 */
template <> unsigned int CGcObj<CPair>::s_cpage SBSS = 0;                   /* 0x2DD284 */
template <> CPair *CGcObj<CPair>::s_ptRecycle SBSS = 0;                     /* 0x2DD288 */
template <> CPair *CGcObj<CPair>::s_ptRoot SBSS = 0;                        /* 0x2DD28C */
template <> CVarb *CGcObj<CVarb>::s_ptFirst SBSS = 0;                       /* 0x2DD290 */
template <> unsigned int CGcObj<CVarb>::s_cpage SBSS = 0;                   /* 0x2DD294 */
template <> CVarb *CGcObj<CVarb>::s_ptRecycle SBSS = 0;                     /* 0x2DD298 */
template <> CVarb *CGcObj<CVarb>::s_ptRoot SBSS = 0;                        /* 0x2DD29C */
template <> CFrame *CGcObj<CFrame>::s_ptFirst SBSS = 0;                     /* 0x2DD2A0 */
template <> unsigned int CGcObj<CFrame>::s_cpage SBSS = 0;                  /* 0x2DD2A4 */
template <> CFrame *CGcObj<CFrame>::s_ptRecycle SBSS = 0;                   /* 0x2DD2A8 */
template <> CFrame *CGcObj<CFrame>::s_ptRoot SBSS = 0;                      /* 0x2DD2AC */
template <> CProc *CGcObj<CProc>::s_ptFirst SBSS = 0;                       /* 0x2DD2B0 */
template <> unsigned int CGcObj<CProc>::s_cpage SBSS = 0;                   /* 0x2DD2B4 */
template <> CProc *CGcObj<CProc>::s_ptRecycle SBSS = 0;                     /* 0x2DD2B8 */
template <> CProc *CGcObj<CProc>::s_ptRoot SBSS = 0;                        /* 0x2DD2BC */
template <> CSidebag *CGcObj<CSidebag>::s_ptFirst SBSS = 0;                 /* 0x2DD2C0 */
template <> unsigned int CGcObj<CSidebag>::s_cpage SBSS = 0;                /* 0x2DD2C4 */
template <> CSidebag *CGcObj<CSidebag>::s_ptRecycle SBSS = 0;               /* 0x2DD2C8 */
template <> CSidebag *CGcObj<CSidebag>::s_ptRoot SBSS = 0;                  /* 0x2DD2CC */
template <> CMethod *CGcObj<CMethod>::s_ptFirst SBSS = 0;                   /* 0x2DD2D0 */
template <> unsigned int CGcObj<CMethod>::s_cpage SBSS = 0;                 /* 0x2DD2D4 */
template <> CMethod *CGcObj<CMethod>::s_ptRecycle SBSS = 0;                 /* 0x2DD2D8 */
template <> CMethod *CGcObj<CMethod>::s_ptRoot SBSS = 0;                    /* 0x2DD2DC */
// The six slot pools' sentries (`if (++sentry == 1) InitSlotheap(...)` in every TU's StaticInit).
int sn_pool_14_2000 __asm__("__sn_t9CSlotPool2i20i8192$s_pool") SBSS = 0;   /* 0x2DD2E0 */
int sn_pool_18_800 __asm__("__sn_t9CSlotPool2i24i2048$s_pool") SBSS = 0;    /* 0x2DD2E4 */
int sn_pool_28_200 __asm__("__sn_t9CSlotPool2i40i512$s_pool") SBSS = 0;     /* 0x2DD2E8 */
int sn_pool_1C_800 __asm__("__sn_t9CSlotPool2i28i2048$s_pool") SBSS = 0;    /* 0x2DD2EC */
int sn_pool_C_C8 __asm__("__sn_t9CSlotPool2i12i200$s_pool") SBSS = 0;       /* 0x2DD2F0 */
int sn_pool_14_100 __asm__("__sn_t9CSlotPool2i20i256$s_pool") SBSS = 0;     /* 0x2DD2F4 */
// dl's iterator stacks (core/containers/dl.cpp): g_psliFirst, g_pdliFirst (sly2/dli.h). In .sbss, so commons too.
SLI *g_psliFirst __asm__("g_psliFirst") SBSS = 0;                            /* 0x2DD2F8 */
DLI *g_pdliFirst __asm__("g_pdliFirst") SBSS = 0;                            /* 0x2DD2FC */
// The five splice heaps' sentries (splice/ref.cpp, spliceutils.cpp).
int sn_heap_20_100 __asm__("__sn_t11CSpliceHeap2i32i256$s_heap") SBSS = 0;  /* 0x2DD300 */
int sn_heap_50_100 __asm__("__sn_t11CSpliceHeap2i80i256$s_heap") SBSS = 0;  /* 0x2DD304 */
int sn_heap_20_40 __asm__("__sn_t11CSpliceHeap2i32i64$s_heap") SBSS = 0;    /* 0x2DD308 */
int sn_heap_C_40 __asm__("__sn_t11CSpliceHeap2i12i64$s_heap") SBSS = 0;     /* 0x2DD30C */
int sn_heap_10_20 __asm__("__sn_t11CSpliceHeap2i16i32$s_heap") SBSS = 0;    /* 0x2DD310 */
// prog's loading-screen start time (the caller's record, published by BeginLoadingScreen) and hud's nesting count
// of the clock pause (hud.cpp, screen.cpp).
float *D_002DD314 SBSS = 0;                                                 /* 0x2DD314 */
int D_002DD318 SBSS = 0;                                                    /* 0x2DD318 */

// ---- .bss (0x452098..0x452200) ----
// Each kind's block table, followed by its slot pool (CGcPool<T>::POOL); then the five splice heaps.
template <> CPair *CGcObj<CPair>::s_aptPage[4] BSS = {0};                   /* 0x452098 */
SLOTHEAP pool_14_2000 __asm__("_t9CSlotPool2i20i8192$s_pool") BSS = {0};    /* 0x4520A8 */
template <> CVarb *CGcObj<CVarb>::s_aptPage[4] BSS = {0};                   /* 0x4520C0 */
SLOTHEAP pool_18_800 __asm__("_t9CSlotPool2i24i2048$s_pool") BSS = {0};     /* 0x4520D0 */
template <> CFrame *CGcObj<CFrame>::s_aptPage[4] BSS = {0};                 /* 0x4520E8 */
SLOTHEAP pool_28_200 __asm__("_t9CSlotPool2i40i512$s_pool") BSS = {0};      /* 0x4520F8 */
template <> CProc *CGcObj<CProc>::s_aptPage[4] BSS = {0};                   /* 0x452110 */
SLOTHEAP pool_1C_800 __asm__("_t9CSlotPool2i28i2048$s_pool") BSS = {0};     /* 0x452120 */
template <> CSidebag *CGcObj<CSidebag>::s_aptPage[4] BSS = {0};             /* 0x452138 */
SLOTHEAP pool_C_C8 __asm__("_t9CSlotPool2i12i200$s_pool") BSS = {0};        /* 0x452148 */
template <> CMethod *CGcObj<CMethod>::s_aptPage[4] BSS = {0};               /* 0x452160 */
SLOTHEAP pool_14_100 __asm__("_t9CSlotPool2i20i256$s_pool") BSS = {0};      /* 0x452170 */
SLOTHEAP heap_20_100 __asm__("_t11CSpliceHeap2i32i256$s_heap") BSS = {0};   /* 0x452188 */
SLOTHEAP heap_50_100 __asm__("_t11CSpliceHeap2i80i256$s_heap") BSS = {0};   /* 0x4521A0 */
SLOTHEAP heap_20_40 __asm__("_t11CSpliceHeap2i32i64$s_heap") BSS = {0};     /* 0x4521B8 */
SLOTHEAP heap_C_40 __asm__("_t11CSpliceHeap2i12i64$s_heap") BSS = {0};      /* 0x4521D0 */
SLOTHEAP heap_10_20 __asm__("_t11CSpliceHeap2i16i32$s_heap") BSS = {0};     /* 0x4521E8 */
