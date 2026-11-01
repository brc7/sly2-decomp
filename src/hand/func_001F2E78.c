typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, void *, void *); };
struct O { char pad[0xC]; struct VTE *pvt; struct O *p10; char pad2[0x24 - 0x14]; struct O *pNext; ulong fl; char pad3[0x64 - 0x30]; struct O *pFirst; char pad4[0xE0 - 0x68]; int nE0; };
void func_001A5B80(struct O *, struct O *, struct O *);
void func_001F2BA8(struct O *);
void func_001F2B20(struct O *);
static inline struct O *Find(struct O *x)
{
    while (x && !(x->fl & (1L << 56))) x = x->p10;
    return x;
}
/** @sly2-annot addr=0x1F2E78
 * Purpose: SO vt13: UpdateLoroot, recurse to children; if the SO has parts and its root changed, re-add parts
 *   to the new tree
 * Info: proposed name UpdateSoLoroot (T1); class SO slot13; TU so
 * Notes: names_v2 C proposed ApplySoConstraintWorld__FP2SOP6CONSTRP6VECTORN22 (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F2E78(struct O *p, struct O *pNew, struct O *pOld)
{
    struct O *po;
    int unused[4];
    func_001A5B80(p, pNew, pOld);
    for (po = p->pFirst; po; po = po->pNext)
        po->pvt[13].pfn((char *)po + po->pvt[13].d, pNew, pOld);
    if (!p->nE0) return;
    for (po = p; ; po = po->p10) {
        if (po->fl & (1L << 56)) return;
        if (po == pNew) break;
    }
    {
    struct O *a = Find(pOld);
    struct O *b = Find(p->p10);
    if (!a && !b) {
        func_001F2BA8(p);
        func_001F2B20(p);
    }
    }
}
