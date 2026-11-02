extern int D_002DFB54;
extern int D_002DFB50;
extern "C" void func_00234A50(int sid);
/** @sly2-annot addr=0x16F868
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
int ResetGs()
{
    if (D_002DFB54 && (*(volatile long *)0x12001000 & 2)) {
        D_002DFB54 = 0;
        func_00234A50(D_002DFB50);
    }
    asm volatile("sync\n\tei");
    return -1;
}
