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
void func_0011A770(DZG *pdzg, DZ *pdz);

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
    func_0011A770(pdzg, 0);
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
        func_0011A770(pdzg, pdzMin);
    }
}
