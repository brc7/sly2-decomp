typedef struct MSG { int cb; int nKind; } MSG;
typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLE { void *pvNext; void *pvPrev; } DLE;
typedef struct FRAME { int an[4]; } FRAME;

typedef struct DZ {
    int nKind;
    int n4;
    float g8;
    float gC;
    float g10;
    int n14;
    float *ag;
    int n1C;
    DLE dle;
} DZ;

typedef struct DZG {
    int cdzsMax;
    int cdzs;
    DZ *adzs;
    DL dlA;
    DL dlB;
    DL dlC;
    DL dlD;
    int n3C;
    void *pv40;
    void *pv44;
    void *pv48;
    void *pv4C;
} DZG;

void STACKSCOPE_ctor(FRAME *pframe);
void STACKSCOPE_ctor_1AE4A0(FRAME *pframe, int n);
void *func_001AE318(MSG *pmsg);
void CopyAb__FPvT0Ui(void *pvDst, void *pvSrc, unsigned int cb);
DZG *InitDzg(DZG *pdzg, int cdzsMax);
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv);
void SolveDzgFilled(DZG *pdzg);

static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE318(pmsg); }
/** @sly2-annot addr=0x11AD30
 * Purpose: Solves A x >= b style system of c constraints with a DZG on a stack frame; writes agSoln
 * Info: proposed name SolveInequalities (T1); TU dzgcore
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/core-library.md#dzgcore-0x11a550-0x11afe0
 * @end-sly2-annot */
void SolveInequalities(int c, float *agA, float *agB, float *agX, float g)
{
    FRAME frame;
    DZG dzg;
    MSG msg;
    int i;

    STACKSCOPE_ctor(&frame);
    InitDzg(&dzg, c);
    for (i = 0; i < c; i++) {
        DZ *pdz = &dzg.adzs[i];
        pdz->g8 = g;
        pdz->ag = (float *)PvAllocMsg(&msg, c * sizeof(float), 0x10);
        CopyAb__FPvT0Ui(pdz->ag, agA + i * c, c * sizeof(float));
        pdz->g10 = agB[i];
        AppendDlEntry__FP2DLPv(&dzg.dlD, pdz);
    }
    dzg.cdzs = c;
    SolveDzgFilled(&dzg);
    for (i = 0; i < c; i++) {
        agX[i] = dzg.adzs[i].gC;
    }
    STACKSCOPE_ctor_1AE4A0(&frame, 2);
}
