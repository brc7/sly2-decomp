extern int D_003DD00C, D_003DD010, D_003DD014, D_003DD018;
/** @sly2-annot addr=0x1ED448
 * Purpose: Null both hash tables and counts
 * Info: proposed name ClearTexHashTables (T2); TU shd
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001ED448(void)
{
    D_003DD010 = 0;
    D_003DD018 = 0;
    D_003DD00C = D_003DD014 = 0;
}
