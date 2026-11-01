struct O { int oid; char pad[0x14]; struct O *pNext; };
extern struct O *D_003E1590;
/** @sly2-annot addr=0x1EC880
 * Purpose: Find an entry by oid in linked list D_003E1590 (next +0x18)
 * Info: proposed name PfontFind (T2); TU shd
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
struct O *func_001EC880(int oid)
{
    int unused[4];
    struct O *p;
    if (oid == -1) return 0;
    for (p = D_003E1590; p; p = p->pNext) {
        if (p->oid == oid) return p;
    }
    return 0;
}
