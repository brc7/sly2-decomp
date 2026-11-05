// sprbuf.cpp -- double-buffered scratchpad transfers (LoadSprBuf/SaveSprBuf) and the two SPRBUF descriptors.
// TU 0x201620..0x201D78 (5 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=func_002019F0
// SLY2_ALIAS: _GLOBAL_$I$*=func_00201D58
#include "sly2/slotheap.h"

typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
#define va_arg(ap, T) (*(T *)((ap += 8) - 8))
#define va_end(ap)
typedef unsigned long ulong;

/* One transfer of a SPRBUF: its size and where its data starts (a DMA tag qword precedes it). */
struct SBE { int cb; ulong *pv; };
typedef char assert_SBE_size[sizeof(SBE) == 8 ? 1 : -1];

extern "C" void *func_002333B4(void *pv, int b, unsigned int cb) __asm__("memset");   /* memset */

/* A scratchpad buffer: its DMA tag list, its scratchpad address, up to eight transfers and a state
   (1 loading, 2 current, 3 saving). */
struct SPRBUF
{
    ulong *aqw;     /* +0x00 DMA tag list (uncached address) */
    char *pv;       /* +0x04 scratchpad address */
    SBE a[8];       /* +0x08 */
    int c;          /* +0x48 transfers */
    int state;      /* +0x4C */
    SPRBUF() { func_002333B4(this, 0, sizeof(*this)); }
};
typedef char assert_SPRBUF_size[sizeof(SPRBUF) == 0x50 ? 1 : -1];

struct DMAC { int chcr; int pad[31]; int sadr; };
extern "C" {
int D_002C3C5C = 0;          /* index of the current buffer */
extern DMAC *D_002DE59C;        /* fromSPR channel */
extern DMAC *D_002DE5A0;        /* toSPR channel */
void sceDmaSync(DMAC *pch, int a, int b);
void sceDmaSend(DMAC *pch, int a);
void sceDmaRecv(DMAC *pch);
}
extern ulong D_003DD380[16], D_003DD400[16];   /* the two tag lists, defined after D_003DD2C0 (address order) */

static inline SPRBUF MakeFirst() { SPRBUF b; b.pv = (char *)0x70000000; b.aqw = (ulong *)((int)D_003DD380 | 0x20000000); return b; }
static inline SPRBUF MakeSecond() { SPRBUF b; b.aqw = (ulong *)((int)D_003DD400 | 0x20000000); b.pv = (char *)0x70002000; return b; }

// 0x2019F0 func_002019F0 (StaticInit_sprbuf, 218 instructions) and its _GLOBAL_$I$ stub func_00201D58: generated
// by g++ for the SPRBUF pair below, then the six engine slot pools.
SPRBUF D_003DD2C0[2] = { MakeFirst(), MakeSecond() };
/* the two tag lists (0x80 bytes each, up to stepguard's .bss), read uncached: 64-aligned (a cache line), which is
 * what puts the first at 0x3DD380 after D_003DD2C0 ends at 0x3DD360 */
ulong D_003DD380[16] __attribute__((aligned(64))), D_003DD400[16] __attribute__((aligned(64)));

SLY2_INSTANTIATE_SLOT_POOLS()

/** @sly2-annot addr=0x201620
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#6-source-files, docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/engine/splice-scripting.md#6-source-files
 * @end-sly2-annot */
void LoadSprBuf(int cb, ...)
{
    SPRBUF *psb = &D_003DD2C0[1 - D_002C3C5C];
    int c = 0;
    char *pv;
    char *pvSrc;
    va_list ap;
    va_start(ap, cb);
    pv = psb->pv;
    if (psb->state == 3)
        sceDmaSync(D_002DE59C, 0, 0);
    psb->state = 1;
    pvSrc = va_arg(ap, char *);
    while (cb) {
        int cbNext;
        char *pvNext;
        cb = (cb + 15) & ~15;
        cbNext = va_arg(ap, int);
        pv += 16;
        psb->a[c].pv = (ulong *)pv;
        psb->a[c].cb = cb;
        pv += cb;
        pvNext = va_arg(ap, char *);
        if (pvSrc) {
            psb->aqw[c * 2] = (ulong)(cb >> 4) | ((ulong)(pvNext ? 3 : 0) << 28) | ((ulong)(unsigned int)pvSrc << 32);
        }
        pvSrc = pvNext;
        cb = cbNext;
        c++;
    }
    psb->c = c;
    D_002DE5A0->chcr |= 0x40;
    D_002DE5A0->sadr = ((int)psb->pv & 0x0FFFFFFF) | 0x80000000;
    sceDmaSend(D_002DE5A0, (int)psb->aqw & 0x0FFFFFFF);
    va_end(ap);
}

/** @sly2-annot addr=0x2017A0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#sprbuf-0x201620-0x201d78
 * @end-sly2-annot */
void SaveSprBuf(void *pv, ...)
{
    SPRBUF *psb = &D_003DD2C0[D_002C3C5C];
    void *apv[8];
    int c = 0;
    int j;
    DMAC *pch;
    va_list ap;
    psb->state = 3;
    sceDmaSync(D_002DE59C, 0, 0);
    va_start(ap, pv);
    while (pv) {
        apv[c++] = pv;
        pv = va_arg(ap, void *);
    }
    j = 0;
    for (c = psb->c - c; c < psb->c - 1; c++, j++)
        psb->a[c].pv[-2] = (ulong)(psb->a[c].cb >> 4) | ((ulong)1 << 28) | ((ulong)(int)apv[j] << 32);
    pch = D_002DE59C;
    psb->a[c].pv[-2] = (ulong)(psb->a[c].cb >> 4) | ((ulong)7 << 28) | ((ulong)(int)apv[j] << 32);
    {
        pch->chcr &= ~0x40;
        pch->sadr = ((int)psb->pv & 0x0FFFFFFF) | 0x80000000;
        sceDmaRecv(pch);
    }
    psb = &D_003DD2C0[1 - D_002C3C5C];
    if (psb->state == 3)
        psb->state = 0;
    va_end(ap);
}

/** @sly2-annot addr=0x201998
 * Purpose: sceDmaSync on the transfer channel, flip double-buffer index D_002C3C5C, mark new buffer state 2
 * Info: proposed name SwapSprBuf (T2); TU sprbuf
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/save-memcard.md#3-scratchpad-buffers-sprbufcpp,
 *   docs/reference/functions/core-library.md#sprbuf-0x201620-0x201d78
 * @end-sly2-annot */
extern "C" void func_00201998(void)
{
    sceDmaSync(D_002DE5A0, 0, 0);
    D_002C3C5C = 1 - D_002C3C5C;
    D_003DD2C0[D_002C3C5C].state = 2;
}
