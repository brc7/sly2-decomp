typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
#define va_arg(ap, T) (*(T *)((ap += 8) - 8))
#define va_end(ap)
typedef unsigned long ulong;
struct SBE { int cb; char *pv; };
struct SPRBUF { ulong *aqw; char *pv; SBE a[8]; int c; int state; };
struct DMAC { int chcr; int pad[31]; int sadr; };
extern "C" {
extern int D_002C3C5C;
extern SPRBUF D_003DD2C0[2];
extern DMAC *D_002DE59C;
extern DMAC *D_002DE5A0;
void sceDmaSync(DMAC *pch, int a, int b);
void sceDmaSend(DMAC *pch, int a);
}

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
        psb->a[c].pv = pv;
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
