struct DISPENV { int a[10]; };
extern int D_002DFB58;
extern int D_002DFB4C;
extern int D_002DEDBC;
extern int D_002DFB5C;
extern int D_002DFB60;
extern int D_002DEDB0;
extern int D_002BE590;
extern int D_002BE594;
extern DISPENV D_002DEB80[2];
extern "C" void func_00234A50(int sid);
extern "C" void sceGsPutDispEnv(DISPENV *pdisp);
void BlendDisplayOnBufferMismatch();
/** @sly2-annot addr=0x16F468
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
int VBlankS_Interrupt(int ch)
{
    if (D_002DFB58) {
        D_002DFB58 = 0;
        func_00234A50(D_002DFB4C);
    }
    switch (D_002DEDBC) {
    case 1:
        if (!D_002DFB5C) {
            D_002BE594 = D_002BE590;
            BlendDisplayOnBufferMismatch();
        }
        break;
    case 2:
        if (!D_002DFB5C)
            BlendDisplayOnBufferMismatch();
        break;
    case 3:
        D_002DEDB0++;
        D_002DFB60 = 1;
        sceGsPutDispEnv(&D_002DEB80[D_002DEDB0 & 1]);
        break;
    }
    asm volatile("sync\n\tei");
    return 0;
}
