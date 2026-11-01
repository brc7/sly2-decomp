struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct DLE { void *pvNext; void *pvPrev; };
struct DZS {
    char pad0[0xC];
    float gC;
    float g10;
    char pad14[0xC];
    DLE dle;
};
struct DZG {
    int cdzsMax;
    int cdzs;
    DZS *adzs;
    DL dlA;
    DL dlB;
    DL dlC;
    DL dlD;
    int n3C;
};

extern "C" {
void ClearDl__FP2DL(DL *pdl);
void ClearDle__FP3DLE(DLE *pdle);
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
}

void ClearDzgSolution(DZG *pdzg)
{
    int i;

    pdzg->n3C = 0;
    ClearDl__FP2DL(&pdzg->dlA);
    ClearDl__FP2DL(&pdzg->dlB);
    ClearDl__FP2DL(&pdzg->dlC);
    ClearDl__FP2DL(&pdzg->dlD);

    for (i = 0; i < pdzg->cdzs; i++) {
        DZS *pdzs = &pdzg->adzs[i];
        pdzs->gC = pdzs->g10 = 0.0f;
        ClearDle__FP3DLE(&pdzs->dle);
        AppendDlEntry__FP2DLPv(&pdzg->dlD, pdzs);
    }
}
