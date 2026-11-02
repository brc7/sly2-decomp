extern int D_004437A0;
extern int D_004437A8[];
/** @sly2-annot addr=0x22D670
 * Purpose: push onto stack D_004437A8[D_004437A0++]
 * Info: proposed name PushXfContext (T2); TU xf
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
void func_0022D670(int v)
{
    D_004437A8[D_004437A0++] = v;
}
