extern int D_002DFB4C;
extern int D_002DFB58;
extern "C" void func_00234A60(int sid);
/** @sly2-annot addr=0x16F560
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void SyncVBlank()
{
    D_002DFB58 = 1;
    func_00234A60(D_002DFB4C);
}
