typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct VT { short d; short pad; void *pfn; };
struct X { char pad[0xC]; struct VT *vt; char pad2[0xF4 - 0x10]; qword *apos; };
int IsegFindShapeS(struct X *p, float g, int a, int b);
int func_001BC530(struct X *p, int i);
void EvaluateShapeS(struct X *p, qword *pv, float g, int n);
int FClipPathzone(void *pv, qword a, qword b, int k);
static inline qword MulPos(M4 *pm, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]), "j"(v));
    return r;
}
int func_001BCAC0(struct X *p, void *pv, int fRev, float g0, float g1)
{
    int i0, r, pass, k, i1;
    i0 = IsegFindShapeS(p, g0, 0, 0);
    i1 = IsegFindShapeS(p, g1, 0, 0);
    for (pass = 0; pass < 2; pass++) {
        int i, iEnd, a;
        M4 m;
        qword v0, v1, q, v;
        switch (pass) {
        case 0: r = 0; k = 0; break;
        case 1: r = 1; k = 1; break;
        }
        if (fRev) {
            a = func_001BC530(p, i0 + 1);
            iEnd = func_001BC530(p, i1 + 1);
        } else {
            a = i0;
            iEnd = i1;
        }
        i = a;
        m = *((M4 *(*)(void *))p->vt[31].pfn)((char *)p + p->vt[31].d);
        EvaluateShapeS(p, &v0, g0, 0);
        q = v0;
        while (i != iEnd) {
            v = MulPos(&m, p->apos[i]);
            if (FClipPathzone(pv, q, v, k))
                goto next;
            if (fRev)
                i = func_001BC530(p, i + 1);
            else
                i = func_001BC530(p, i - 1);
            q = v;
        }
        EvaluateShapeS(p, &v1, g1, 0);
        if (!FClipPathzone(pv, q, v1, k))
            return r;
    next:;
    }
    return 2;
}
