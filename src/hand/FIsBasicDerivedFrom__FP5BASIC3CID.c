struct VTE { short d; short pad; void *(*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct R { int f0; int f4; };
struct T { int f0; int f4; int f8; int fc; int f10; };
extern struct T D_002BAAE8[];
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int n) {
    struct R *pr = (struct R *)((void *(*)(void *))po->vt[6].pfn)((char *)po + po->vt[6].d);
    int x = pr->f4;
    int f = 0;
    if (x >= n) { struct T *pt = &D_002BAAE8[n]; f = x < pt->f8; }
    return f;
}
