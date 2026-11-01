struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct N { struct O *po; int f4; struct N *pnext; };
struct S { int f0; int f4; struct N *pfirst; struct VTE *vt; };
void SendLoMessage(struct S *p, int a, int b) {
    struct N *pn;
    ((void (*)(void *, int, int))p->vt[5].pfn)((char *)p + p->vt[5].d, a, b);
    for (pn = p->pfirst; pn; ) {
        struct O *po = pn->po;
        pn = pn->pnext;
        ((void (*)(void *, int, int))po->vt[5].pfn)((char *)po + po->vt[5].d, a, b);
    }
}
