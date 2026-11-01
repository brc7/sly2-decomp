
struct VTE { short d; short pad; void (*pf)(void *); };
struct OW { char pad[0x26C]; int x; };
struct O { char pad[0xC]; struct VTE *vt; struct OW *pow; };
struct S { struct O *po; };
void DecrementBhvstk(int *p);
void PopBhv(struct S *p)
{
    struct O *po = p->po;
    if (po) {
        int *px = &po->pow->x;
        po->vt[9].pf((char *)po + po->vt[9].d);
        DecrementBhvstk(px);
        p->po = 0;
    }
}
