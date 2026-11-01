extern char D_003DD490[];
void *func_00210B98(void *p);
void func_0022B490(void *a, void *b, int c, int d, int e, int f);
/** @sly2-annot addr=0x14C190
 * Purpose: v = 0x210B98(iep) ?: 0x210B98(0), then 0x22B490(g_sw, v, 0x189, -1, 0, -1)
 * Info: proposed name CheatWarpEpisode (T2); TU cheat
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: semantic_tu_docs/cheat
 * See: docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void func_0014C190(void *p)
{
    void *v = func_00210B98(p);
    if (!v) v = func_00210B98(0);
    func_0022B490(D_003DD490, v, 0x189, -1, 0, -1);
}
