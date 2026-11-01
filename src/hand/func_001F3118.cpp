typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *, float); };
struct VT { VTE a[43]; };
struct L { char pad[0xC]; VT *pvt; struct L *parent; int n14; char pad1[0x24 - 0x18]; struct L *next; ulong lo : 60; ulong b60 : 1; ulong hi : 3; float g30, g34, g38; char pad2[0x64 - 0x3C]; struct L *first; char pad3[0x140 - 0x68]; int n140; };
struct G;
extern G D_003DD490;
extern "C" void func_001D6310(L *);
extern "C" void func_001D6378(L *);
extern "C" void func_00147E88(G *, int);
/** @sly2-annot addr=0x1F3118
 * Purpose: SO vt42: store busy distance +0x38, refresh +0x140 registration (0x1D6310/0x1D6378), re-sort root
 *   in g_sw (0x147E88), push smaller value to children
 * Info: proposed name SetSoSBusy (T2); class SO slot42; TU so
 * Notes: names_v2 C proposed ApplyConstr__FP6CONSTRP6VECTORN21 (0.438)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F3118(L *p, float t)
{
    L *q;
    int unused[4];
    p->g38 = t;
    if (p->n140 != -1) {
        func_001D6310(p);
        func_001D6378(p);
    }
    if (!p->parent) {
        bool f = p->b60;
        if (f) func_00147E88(&D_003DD490, p->n14);
    }
    for (q = p->first; q; q = q->next) {
        if (t < q->g38) q->pvt->a[42].pfn((char *)q + q->pvt->a[42].d, t);
    }
}
