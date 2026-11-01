struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct DLI { struct DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; };
extern struct DLI *D_002DD2FC;
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct E { char pad[0x4C]; float t; };
struct P { char pad[0x4118]; char pool[0x18]; struct DL dl; };
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
void func_0011CD98(void *, void *);
static inline void InitDli(struct DLI *pdli, struct DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->ibDle;
    pdli->pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(struct DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}
/** @sly2-annot addr=0x1F77C0
 * Purpose: Drop zap-history entries older than 0.25 s from g_sw list
 * Info: proposed name ExpireSwZapRecords (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#3-zaps-damage-delivery,
 *   docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void ExpireSwZapRecords(struct P *p)
{
    struct DLI dli;
    struct E *pe;
    InitDli(&dli, &p->dl);
    while ((pe = PvNextDli(&dli)) != 0) {
        if (D_002DDED0.t < pe->t + 0.25f) continue;
        RemoveDlEntry__FP2DLPv(&p->dl, pe);
        func_0011CD98(p->pool, pe);
    }
    D_002DD2FC = dli.pdliNext;
}
