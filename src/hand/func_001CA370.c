struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct Z { struct VTE *vt; };
struct R { int n; struct LO **ap; int x8; int c; int x10; int x14; void **ap18; char pad[0x6C - 0x1C]; };
struct G { char pad[0x3F08]; struct R *ar; };
void *IdProxy__Fii(int i, int c);
void func_001CA370(struct G *pg, int i, int a, int b, void *x, int y, struct Z *pz)
{
    struct R *pr = &pg->ar[i];
    void *v = IdProxy__Fii(i, pr->c);
    int j;
    pr->ap18[pr->c] = x;
    for (j = 0; j < pr->n; j++) {
        struct LO *plo = pr->ap[pr->c * pr->n + j];
        ((void (*)(void *, void *, int))plo->vt[25].pf)((char *)plo + plo->vt[25].d, v, 1);
        ((void (*)(void *))plo->vt[8].pf)((char *)plo + plo->vt[8].d);
        ((void (*)(void *, int, int, int))plo->vt[10].pf)((char *)plo + plo->vt[10].d, a, b, 0);
        ((void (*)(void *, int))plo->vt[37].pf)((char *)plo + plo->vt[37].d, y);
    }
    if (pz)
        ((void (*)(void *, int, struct LO **))pz->vt[1].pf)((char *)pz + pz->vt[1].d, pr->n, &pr->ap[pr->c * pr->n]);
    pr->c++;
}
