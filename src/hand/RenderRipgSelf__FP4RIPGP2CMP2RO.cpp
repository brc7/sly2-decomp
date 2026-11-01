struct DL { void *head; void *tail; int ibDle; };
struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; DLI *m_pdliNext; };
extern DLI *D_002DD2FC;
struct CLOCK { float t, dt; };
extern CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pf)(); };
struct RIP { char pad[0x14]; float tStart; char p1[0xEC - 0x18]; VTE *vt; };
struct RIPG { char pad[0x15C]; DL dl; };
struct CM;
struct RO;
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->m_pdl = pdl;
    pdli->m_ppv = (void **)pdl;
    pdli->m_ibDle = pdl->ibDle;
    pdli->m_pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
/** @sly2-annot addr=0x1CE608
 * Purpose: RIPG vt slot 19 RenderSelf: render each rip via vtable
 * Info: proposed name RenderRipgSelf__FP4RIPGP2CMP2RO (EXISTING); class RIPG slot19; TU rip
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void RenderRipgSelf(RIPG *pripg, CM *pcm, RO *pro)
{
    DLI dli;
    RIP *prip;
    InitDli(&dli, &pripg->dl);
    while ((prip = (RIP *)PvNextDli(&dli)) != 0) {
        if (D_002DDED0.t <= prip->tStart) continue;
            ((void (*)(void *))prip->vt[6].pf)((char *)prip + prip->vt[6].d);
    }
    D_002DD2FC = dli.m_pdliNext;
}
