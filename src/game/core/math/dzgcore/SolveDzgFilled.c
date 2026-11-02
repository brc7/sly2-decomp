typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; int nUnused; } DLI;

typedef struct DZ {
    int nKind;
    char pad4[0xC];
    float g10;
    char pad14[0xC];
    DLE dle;
} DZ;

typedef struct DZG {
    int cdzsMax;
    int cdzs;
    void *adzs;
    DL dlA;
    DL dlB;
    DL dlC;
    DL dlD;
    int n3C;
} DZG;

extern DLI *g_pdliFirst;
void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void EnforceDzgDz__FP3DZGP2DZ(DZG *pdzg, DZ *pdz);

static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->ibDle;
    pdli->pdliNext = g_pdliFirst;
    g_pdliFirst = pdli;
}

static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}

/** @sly2-annot addr=0x11ABF8
 * Purpose: Moves equality constraints into the active list, EnforceDzgDz, then repeatedly adds the
 *   most-violated constraint
 * Info: proposed name SolveDzgFilled (T1); TU dzgcore
 * Notes: COLLISION: SolveDzg already used (existing symbol at 0x158988); renamed to SolveDzg_11ABF8 |
 *   superseded: SolveDzg_11ABF8 (arbitration) | arbitration other name: CONFIRMED static: Sly1 SolveDzg loop
 *   (equality rows Uncat->Pos, EnforceDzgDz(0), add most-violated row) WITHOUT the lazy FillDzgDz; only
 *   caller SolveInequalitie
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; arbitration; sources:
 *   r1_116918+arbitration
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#dzgcore-0x11a550-0x11afe0
 * @end-sly2-annot */
void SolveDzgFilled(DZG *pdzg)
{
    DLI dli;
    DZ *pdz;

    InitDli(&dli, &pdzg->dlD);
    while ((pdz = (DZ *)PvNextDli(&dli)) != 0) {
        if (pdz->nKind == 1) {
            RemoveDlEntry__FP2DLPv(&pdzg->dlD, pdz);
            AppendDlEntry__FP2DLPv(&pdzg->dlA, pdz);
            pdzg->n3C++;
        }
    }
    EnforceDzgDz__FP3DZGP2DZ(pdzg, 0);
    g_pdliFirst = dli.pdliNext;

    for (;;) {
        DZ *pdzMin = 0;
        float gMin = -0.01f;
        DZ *pdzT;
        for (pdzT = (DZ *)pdzg->dlD.pvFirst; pdzT; pdzT = (DZ *)pdzT->dle.pvNext) {
            if (pdzT->g10 < gMin) {
                gMin = pdzT->g10;
                pdzMin = pdzT;
            }
        }
        if (!pdzMin) {
            break;
        }
        EnforceDzgDz__FP3DZGP2DZ(pdzg, pdzMin);
    }
}
