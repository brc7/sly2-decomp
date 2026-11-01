// EnforceDzgDz: Sly 1's dzg solver step (A/B/C/D constraint lists, Crout decomposition).
struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct DLE { void *pvNext; void *pvPrev; };
struct DZ {
    int nKind;
    float gMin;
    float gMax;
    float gC;
    float g10;
    float g14;
    float *ag;
    int n1C;
    DLE dle;
};
struct DZG {
    int cdzMax;
    int cdz;
    DZ *adz;
    DL dlA;
    DL dlB;
    DL dlC;
    DL dlD;
    int cdzA;
    float *agMat;
    float *agLu;
    float *agB;
    float *agX;
};

extern "C" {
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
extern const float D_002A4120;
extern const float D_002A4124;
}
void CroutDecomp(int n, float *a, float *lu);
void CroutSolve(int n, float *lu, float *b, float *x);

void EnforceDzgDz(DZG *pdzg, DZ *pdz)
{
    int iter;
    if (pdz) {
        RemoveDlEntry__FP2DLPv(&pdzg->dlD, pdz);
        AppendDlEntry__FP2DLPv(&pdzg->dlA, pdz);
        pdzg->cdzA++;
    }
    for (iter = 0; iter < 10; iter++) {
        DZ *pdzI, *pdzJ, *pdzK, *pdzBest;
        float *pg;
        int i, k;
        float gMin;

        float *pgMat = pdzg->agMat;
        for (pdzI = (DZ *)pdzg->dlA.pvFirst; pdzI; pdzI = (DZ *)pdzI->dle.pvNext)
            for (pdzJ = (DZ *)pdzg->dlA.pvFirst; pdzJ; pdzJ = (DZ *)pdzJ->dle.pvNext)
                *pgMat++ = pdzI->ag[pdzJ - pdzg->adz];
        CroutDecomp(pdzg->cdzA, pdzg->agMat, pdzg->agLu);
        float *pgB = pdzg->agB;
        for (pdzI = (DZ *)pdzg->dlA.pvFirst; pdzI; pdzI = (DZ *)pdzI->dle.pvNext)
            *pgB++ = -pdzI->g10;
        CroutSolve(pdzg->cdzA, pdzg->agLu, pdzg->agB, pdzg->agX);
        for (i = 0; i < pdzg->cdz; i++) {
            float g = 0.0f;
            float *pgX = pdzg->agX;
            for (pdzI = (DZ *)pdzg->dlA.pvFirst; pdzI; pdzI = (DZ *)pdzI->dle.pvNext)
                g += *pgX++ * pdzI->ag[i];
            pdzg->adz[i].g14 = g;
        }

        pdzBest = 0;
        gMin = 1.0f;
        k = 0;
        pg = pdzg->agX;
        for (pdzI = (DZ *)pdzg->dlA.pvFirst; pdzI; pdzI = (DZ *)pdzI->dle.pvNext) {
            float x = *pg++;
            if (x > 1e-4f) {
                // Empty asm, allocation-only operands (emits nothing): two extra uses of the +-1e-4 constants
                // raise their global-alloc priority above the hoisted D_002A4120/D_002A4124 loads, whose
                // REG_EQUIV-less pseudos otherwise win (local-alloc doubles the constants' live length), so
                // the constants get f5/f4 and the globals f7/f6 as in the target.
                __asm__("" : : "f"(1e-4f), "f"(-1e-4f));
                if (pdzI->gMax != D_002A4120) {
                    float t = (pdzI->gMax - pdzI->gC) / x;
                    if (t < gMin) {
                        gMin = t;
                        pdzBest = pdzI;
                        k = 4;
                    }
                }
            } else if (x < -1e-4f) {
                if (pdzI->gMin != D_002A4124) {
                    float t = (pdzI->gMin - pdzI->gC) / x;
                    if (t < gMin) {
                        gMin = t;
                        pdzBest = pdzI;
                        k = 2;
                    }
                }
            }
        }
        for (pdzK = (DZ *)pdzg->dlB.pvFirst; pdzK; pdzK = (DZ *)pdzK->dle.pvNext) {
            if (pdzK->g14 > -1e-4f) {} else {
                float t = -pdzK->g10 / pdzK->g14;
                if (t < gMin) {
                    gMin = t;
                    pdzBest = pdzK;
                    k = 1;
                }
            }
        }
        for (pdzK = (DZ *)pdzg->dlC.pvFirst; pdzK; pdzK = (DZ *)pdzK->dle.pvNext) {
            if (pdzK->g14 < 1e-4f) {} else {
                float t = -pdzK->g10 / pdzK->g14;
                if (t < gMin) {
                    gMin = t;
                    pdzBest = pdzK;
                    k = 3;
                }
            }
        }
        for (i = 0; i < pdzg->cdz; i++) {
            DZ *pdzT = &pdzg->adz[i];
            pdzT->g10 += gMin * pdzT->g14;
        }
        pg = pdzg->agX;
        for (pdzI = (DZ *)pdzg->dlA.pvFirst; pdzI; pdzI = (DZ *)pdzI->dle.pvNext)
            pdzI->gC += gMin * *pg++;

        switch (k) {
        case 0:
            return;
        case 2:
            pdzg->cdzA--;
            RemoveDlEntry__FP2DLPv(&pdzg->dlA, pdzBest);
            AppendDlEntry__FP2DLPv(&pdzg->dlB, pdzBest);
            if (pdzBest == pdz)
                return;
            break;
        case 1:
            pdzg->cdzA++;
            RemoveDlEntry__FP2DLPv(&pdzg->dlB, pdzBest);
            AppendDlEntry__FP2DLPv(&pdzg->dlA, pdzBest);
            break;
        case 4:
            pdzg->cdzA--;
            RemoveDlEntry__FP2DLPv(&pdzg->dlA, pdzBest);
            AppendDlEntry__FP2DLPv(&pdzg->dlC, pdzBest);
            if (pdzBest == pdz)
                return;
            break;
        case 3:
            pdzg->cdzA++;
            RemoveDlEntry__FP2DLPv(&pdzg->dlC, pdzBest);
            AppendDlEntry__FP2DLPv(&pdzg->dlA, pdzBest);
            break;
        }
    }
}
