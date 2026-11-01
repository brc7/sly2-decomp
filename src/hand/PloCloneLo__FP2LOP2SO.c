struct VTE { short d; short pad; void *(*pfn)(); };
struct O { int pad[3]; struct VTE *vt; int pad2[2]; int f18; };
struct R { int f0; int f4; };
struct O *PloNew__F3CIDP2SO3OIDi(int, int, int, int);
struct O *PloCloneLo__FP2LOP2SO(struct O *po, int a) {
    struct R *pr;
    struct O *pn;
    if (!po) return 0;
    pr = (struct R *)((void *(*)(void *))po->vt[6].pfn)((char *)po + po->vt[6].d);
    pn = PloNew__F3CIDP2SO3OIDi(pr->f4, a, po->f18, -1);
    ((void (*)(void *, struct O *))pn->vt[40].pfn)((char *)pn + pn->vt[40].d, po);
    return pn;
}
