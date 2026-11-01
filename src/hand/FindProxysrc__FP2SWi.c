struct E { int pad[4]; int id; char pad2[0x58]; };
struct S { char pad[0x3F04]; int c; struct E *a; };
/** @sly2-annot addr=0x1CB770
 * Purpose: index of proxy source with id or -1
 * Info: proposed name FindProxysrc (T1); TU proxy
 * Notes: names_v2 C proposed SetAloThrobOutColor__FP3ALOP6VECTOR (0.195)
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
int FindProxysrc__FP2SWi(struct S *p, int id)
{
    int i;
    for (i = 0; i < p->c; i++)
        if (p->a[i].id == id) return i;
    return -1;
}
