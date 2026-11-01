struct VTE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { int n0; int c; struct O *ap[1]; };
void UpdateBhvstk(struct S *p)
{
    int i;
    for (i = p->c; --i >= 0;) {
        struct O *po = p->ap[i];
        po->vt[10].pf((char *)po + po->vt[10].d);
    }
}
