struct ENTRY;
struct S { char pad[0x2C0]; long mask; };
struct ENTRY *PanimbindBest(struct S *p, long mask);
/** @sly2-annot addr=0x209BC0
 * Purpose: PanimbindBest for the current flags
 * Info: proposed name PanimbindCur (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
struct ENTRY *func_00209BC0(struct S *p)
{
    return PanimbindBest(p, p->mask);
}
