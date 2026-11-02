void func_001DAF38(void *p, int n);
void func_0019F0A8(void *p);
extern char D_003DD490[];
/** @sly2-annot addr=0x14C2B0
 * Purpose: 0x1DAF38(nParam, 1); 0x19F0A8(g_sw) (refresh)
 * Info: proposed name CheatGiveGadget (T1); TU cheat
 * Notes: names_v2 v1 proposed FIsPvGarbage__FPv (0.015)
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; static naming pass; sources:
 *   semantic_tu_docs/cheat
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void CheatGiveGadget(void *p)
{
    func_001DAF38(p, 1);
    func_0019F0A8(D_003DD490);
}
