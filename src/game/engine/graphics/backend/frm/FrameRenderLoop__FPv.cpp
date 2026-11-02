/* register-pinned locals (n in $v1, pfr in $a0) reproduce the madd-into-a0 form */
union QW;
struct sceDmaChan;
struct FR {
    int cqwGifs; QW *aqwGifs; QW *aqwGifsBuf; int cqwGifsBuf;
    int cqwVifs; QW *aqwVifs; QW *aqwVifsBuf; int cqwVifsBuf;
    int cqw20; QW *aqw24; int n28; int n2C; int n30; int adl[6]; int n4C;
};
extern FR *D_002DEB14;
extern FR D_002DEA50[2];
extern unsigned int D_002DEB0C;
extern int D_002DEB20;
extern int D_002DEB18;
extern int D_002DEB1C;
extern int D_002DEDBC;
extern sceDmaChan *D_002DE594;
extern sceDmaChan *D_002DE598;
extern "C" void func_00234A60(int sid);
extern "C" void func_00234A40(int sid);
extern "C" void PropagateSais(FR *pfr);
extern "C" void RebuildFrameGifs__Fv();
extern "C" void BlendPrevFrame__Fv();
extern "C" void func_002417F0(sceDmaChan *pch, int a, int b);
void RestoreGsBuffers();
void SwapGsBuffers();
void SendDmaSyncGsFinish(sceDmaChan *pch, QW *pqw);
/** @sly2-annot addr=0x160B70
 * Purpose: Render-thread body: wait for a closed frame, PrepareGsForFrameRender, rebuild frame packets, DMA
 *   VIF1 then GIF chains, optional BlendPrevFrame, signal, SwapGsBuffers; forever
 * Info: proposed name FrameRenderLoop (T1); TU frm
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r3_1539A8+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions, docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/rendering.md#1-frame-structure
 * @end-sly2-annot */
extern "C" void FrameRenderLoop__FPv()
{
    for (;;) {
        D_002DEB20 = 1;
        func_00234A60(D_002DEB18);
        D_002DEB20 = 0;
        register unsigned int n __asm__("$3") = D_002DEB0C;
        register FR *pfr __asm__("$4") = &D_002DEA50[n];
        D_002DEB0C = n + 1;
        D_002DEB14 = pfr;
        if (D_002DEB0C >= 2) D_002DEB0C = 0;
        PropagateSais(pfr);
        RestoreGsBuffers();
        RebuildFrameGifs__Fv();
        SendDmaSyncGsFinish(D_002DE594, D_002DEB14->aqw24);
        SendDmaSyncGsFinish(D_002DE598, D_002DEB14->aqwGifs);
        if (D_002DEB14->n30 & 1)
            BlendPrevFrame__Fv();
        func_002417F0(D_002DE594, 0, 0);
        func_00234A40(D_002DEB1C);
        SwapGsBuffers();
        D_002DEDBC = 1;
    }
}
