extern char D_003DD490[];
void func_0020FEC0(void *a, void *b, int n, void *c, int d, int e, int f, int g);
/** @sly2-annot addr=0x148B38
 * Purpose: FAddRsmg(g_sw, arsmg +0xB4, 8, &crsmg, fOnTrigger, oidRoot, oidSM, oidGoal)
 * Info: proposed name SetBtnRsmg (T1); TU button
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void SetBtnRsmg(char *p, int d, int e, int f, int g)
{
    func_0020FEC0(D_003DD490, p + 0xB4, 8, p + 0xB0, d, e, f, g);
}
