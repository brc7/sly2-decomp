typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, void *, void *); };
struct O { char pad[0xC]; struct VTE *pvt; struct O *p10; char pad2[0x24 - 0x14]; struct O *pNext; ulong fl; char pad3[0x64 - 0x30]; struct O *pFirst; char pad4[0xE0 - 0x68]; int nE0; };
void func_001A5B80(struct O *, struct O *, struct O *);
void RemoveSoParts(struct O *);
void AddSoParts(struct O *);
static inline struct O *Find(struct O *x)
{
    while (x && !(x->fl & (1L << 56))) x = x->p10;
    return x;
}
void UpdateSoLoroot(struct O *p, struct O *pNew, struct O *pOld)
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
        RemoveSoParts(p);
        AddSoParts(p);
    }
    }
}
