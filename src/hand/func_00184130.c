struct VTE { short d; short pad; void (*pfn)(); };
struct S { char pad[0xC]; struct VTE *vt; char p1[0xFC]; int x10c; char p2[4]; void *x114; int x118; int x11c; char p3[0x3C]; int x15c; int x160; };
extern char D_003DD490[], D_002DEB30[];
void TriggerRsmg(void *g, int a, int *pb, struct S *p, int n);
void ApplyTaskStates(void *g, int a, int *pb, struct S *p);
void func_001DFD58(void *p, int n);
void func_00184130(struct S *p, int n)
{
    struct VTE *e;
    p->x10c = n;
    e = &p->vt[4];
    ((void (*)(void *, int, struct S *))e->pfn)((char *)p + e->d, 6, p);
    TriggerRsmg(D_003DD490, p->x118, &p->x11c, p, 1);
    ApplyTaskStates(D_002DEB30, p->x15c, &p->x160, p);
    e = &p->vt[3];
    ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)p + e->d, 5, 0, 0, 0, 0);
    if (p->x114) func_001DFD58(p->x114, n);
}
