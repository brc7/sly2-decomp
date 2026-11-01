struct VTE { short d; short pad; void (*pfn)(void *, void *, int); };
struct O { int a[3]; struct VTE *vt; char pad[0x14]; struct O *pNext; char pad2[0x3C]; struct O *pFirst; };
void func_001CA118(struct O *p, void *a);
/** @sly2-annot addr=0x1CA308
 * Purpose: SO vt25: LO::SetIdProxy then recurse into child objects
 * Info: proposed name SetSoIdProxy (T1); class SO slot25; TU proxy
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void func_001CA308(struct O *p, void *a)
{
    struct O *o;
    int unused[4];
    func_001CA118(p, a);
    for (o = p->pFirst; o; o = o->pNext) { struct VTE *e = &o->vt[25]; e->pfn((char *)o + e->d, a, 0); }
}
