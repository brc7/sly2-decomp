struct CSR { unsigned int SIGNAL:1, FINISH:1, HSINT:1, VSINT:1, EDWINT:1, ZERO1:3, FLUSH:1, RESET:1, ZERO2:2, NFIELD:1, FIELD:1, FIFO:2, REV:8, ID:8; };
extern int D_002DEDBC;
extern int D_002DFB60;
extern int D_002DEDB4;
extern int D_002DEDB0;
extern char D_002DECD0[];
extern char D_002DEBE0[];
extern char D_002DECC0[];
extern char D_002DEBD0[];
extern "C" void sceGsSetHalfOffset(void *pdraw, short x, short y, short half);
extern "C" void func_00234CA0(int n);
extern "C" void func_002406C0(void *p);
static inline int FField() { return ((CSR *)0x12001000)->FIELD; }
/** @sly2-annot addr=0x16F6B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void RestoreGsBuffers()
{
    if (D_002DEDBC == 3)
        D_002DEDBC = 2;
    if (D_002DFB60) {
        D_002DFB60 = 0;
        D_002DEDB4 = FField() == 0;
        sceGsSetHalfOffset((D_002DEDB0 & 1) ? D_002DECD0 : D_002DEBE0, 0x800, 0x800, D_002DEDB4);
        func_00234CA0(0);
        func_002406C0((D_002DEDB0 & 1) ? D_002DECC0 : D_002DEBD0);
    }
}
