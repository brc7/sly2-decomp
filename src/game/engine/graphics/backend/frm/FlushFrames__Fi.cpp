union QW;
struct FR {
    int cqwGifs; QW *aqwGifs; QW *aqwGifsBuf; int cqwGifsBuf;
    int cqwVifs; QW *aqwVifs; QW *aqwVifsBuf; int cqwVifsBuf;
    int cqw20; QW *aqw24; int n28; int n2C; int n30; int adl[6]; int n4C;
};
extern FR *D_002DEB14;
extern FR D_002DEA50[2];
extern volatile int D_002DEB20;
extern int D_002DEB1C;
extern int D_002DEDBC;
extern "C" int func_00234A60(int sid);
extern "C" int func_00234A40(int sid);
extern "C" void RenderFrame__FP3FRMi(FR *pfr, int f);
extern "C" void BlendPrevFrame__Fv();
extern "C" void ClearPendingFrame__FP3FRM(FR *pfr);
extern "C" void ClearPhase(int phase);
void SwapGsBuffers();
/** @sly2-annot addr=0x160E68
 * Purpose: Drains the render thread (wait/signal sema twice), optionally re-renders the last frame 3 times to
 *   both buffers and clears pending frames
 * Info: proposed name FlushFrames (T1); TU frm
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/rendering.md#13-key-functions, docs/guides/port-plan.md#61-interception-point-k7-k8-k16,
 *   docs/reference/functions/rendering.md#frm-0x1607a0-0x1613a8
 * @end-sly2-annot */
extern "C" void FlushFrames__Fi(int fReset)
{
    unsigned int i;
    for (i = 0; i < 2; i++)
        func_00234A60(D_002DEB1C);
    for (i = 0; i < 2; i++)
        func_00234A40(D_002DEB1C);
    while (!D_002DEB20)
        ;
    if (fReset && D_002DEB14) {
        for (int j = 0; j < 3; j++) {
            RenderFrame__FP3FRMi(D_002DEB14, 1);
            if (D_002DEB14->n30 & 1)
                BlendPrevFrame__Fv();
            SwapGsBuffers();
        }
        D_002DEDBC = 3;
    }
    if (fReset) {
        FR *pfr = D_002DEA50;
        do {
            ClearPendingFrame__FP3FRM(pfr);
            pfr++;
        } while (pfr < D_002DEA50 + 2);
    }
    ClearPhase(3);
    D_002DEB14 = 0;
}
