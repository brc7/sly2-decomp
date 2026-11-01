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
void func_0011A200(DL *pdl);
void func_0011A210(DLE *pdle);
void func_0011A220(DL *pdl, void *pv);
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
    func_0011A200(&pdzg->dlA);
    func_0011A200(&pdzg->dlB);
    func_0011A200(&pdzg->dlC);
    func_0011A200(&pdzg->dlD);

    for (i = 0; i < pdzg->cdzs; i++) {
        DZS *pdzs = &pdzg->adzs[i];
        pdzs->gC = pdzs->g10 = 0.0f;
        func_0011A210(&pdzs->dle);
        func_0011A220(&pdzg->dlD, pdzs);
    }
}
