struct O { char pad[0x14]; int n14; char pad2[0x30-0x18]; struct O *pChild; char pad3[0x40-0x34]; struct O *pNext; };
extern struct O *D_003E159C;
/** @sly2-annot addr=0x1EC918
 * Purpose: Find an SAA by id (+0x14) in list D_003E159C (stops at entries with a child)
 * Info: proposed name PsaaFind (T2); TU shd
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
struct O *func_001EC918(int n)
{
    int unused[4];
    struct O *p;
    if (n == -1) return 0;
    for (p = D_003E159C; p; p = p->pNext) {
        if (p->pChild) break;
        if (p->n14 == n) return p;
    }
    return 0;
}
