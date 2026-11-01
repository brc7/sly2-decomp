struct DL { void *head; void *tail; int ibDle; };
struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; DLI *m_pdliNext; };
extern DLI *D_002DD2FC;
struct DZ { int n0; int pad4[3]; float g10; int pad14; int f18; int pad1C; DZ *pdzNext; };
struct DZG { int pad0[3]; DL dlC; int pad18[6]; DL dl30; int c3C; };
struct XP; struct SO;
extern "C" {
int func_00158958(DZG *, DZ *);
void func_00158728(DZG *, int, DZ *, int);
void func_0011A2E8(DL *, void *);
void func_0011A220(DL *, void *);
void func_0011A770(DZG *, DZ *);
}
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
/** @sly2-annot addr=0x158988
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name SolveDzg__FP3DZGi (EXISTING); TU dzg
 * Notes: RENAME proposed -> SolveDzg__FP3DZGi (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.80]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#dzg-0x1585d8-0x159118
 * @end-sly2-annot */
void SolveDzg(DZG *pdzg, int a, int b, XP **ppxp, int *pn, SO ***pppso)
{
    DLI dli;
    DLI dliUnused;
    DZ *pdz;
    InitDli(&dli, &pdzg->dl30);
    while ((pdz = (DZ *)PvNextDli(&dli)) != 0) {
        if (pdz->n0 == 1) {
            if (!pdz->f18)
                func_00158728(pdzg, a, pdz, func_00158958(pdzg, pdz));
            func_0011A2E8(&pdzg->dl30, pdz);
            func_0011A220(&pdzg->dlC, pdz);
            pdzg->c3C++;
        }
    }
    func_0011A770(pdzg, 0);
    D_002DD2FC = dli.m_pdliNext;
    for (;;) {
        DZ *pdzBest = 0;
        float gMin = -0.01f;
        for (DZ *pdzT = (DZ *)pdzg->dl30.head; pdzT; pdzT = pdzT->pdzNext) {
            if (pdzT->g10 < gMin) {
                gMin = pdzT->g10;
                pdzBest = pdzT;
            }
        }
        if (!pdzBest)
            break;
        if (!pdzBest->f18)
            func_00158728(pdzg, a, pdzBest, func_00158958(pdzg, pdzBest));
        func_0011A770(pdzg, pdzBest);
    }
}
