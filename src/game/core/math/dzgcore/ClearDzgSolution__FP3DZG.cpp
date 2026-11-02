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

/** @sly2-annot addr=0x11A6B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name ClearDzgSolution__FP3DZG (EXISTING); TU dzgcore
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/core-library.md#dzgcore-0x11a550-0x11afe0
 * @end-sly2-annot */
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
