typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
#define va_arg(ap, T) (*(T *)((ap += 8) - 8))
#define va_end(ap)
typedef unsigned long ulong;
struct SBE { int cb; ulong *pv; };
struct SPRBUF { ulong *aqw; char *pv; SBE a[8]; int c; int state; };
struct DMAC { int chcr; int pad[31]; int sadr; };
extern "C" {
extern int D_002C3C5C;
extern SPRBUF D_003DD2C0[2];
extern DMAC *D_002DE59C;
void sceDmaSync(DMAC *pch, int a, int b);
void sceDmaRecv(DMAC *pch);
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
