struct S { char pad[0x150]; int a; int b; int c; };
/** @sly2-annot addr=0x1C2DB8
 * Purpose: Clear held loot pointer(s)
 * Info: proposed name ClearCid99Loot (T2); class CID99; TU tu_1C2D20
 * Notes: names_v2 v1 proposed FUN_00177828 (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1c2d20-0x1c2d20-0x1c3390
 * @end-sly2-annot */
void func_001C2DB8(struct S *p) { p->c = 0; p->b = 0; p->a = 0; }
