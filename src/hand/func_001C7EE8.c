typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x100 - 0x10]; int x100; };
struct S {
    char pad[0xC]; struct VTE *vt;
    char p0[0x150 - 0x10]; int x150;
    char p1[0x210 - 0x154]; struct LO *x210;
};
struct M { int n; struct S *p; int pad[14]; };
extern char D_003DD490[];
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
qword func_001A62E8(struct S *p);
void *EmitSwNoise(void *pg, int n, qword q, struct S *p);
struct LO *func_001CB658(struct S *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *p, int n);
void func_001C7C30(int n, int f);
void func_001618B0(void *pg);
void func_001C60B8(struct S *p, int n);
static inline int FTest(struct S *p) { struct LO *q = func_001CB658(p); return q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 7); }
void func_001C7EE8(struct S *p, int n)
{
    struct LO *q;
    char *pg;
    qword qv;
    if (n == p->x150) return;
    if (n == 1) {
        struct LO *plo = p->x210;
        if (plo) {
            struct M m;
            m.n = n;
            m.p = p;
            ((void (*)(void *, struct M *))plo->vt[51].pf)((char *)plo + plo->vt[51].d, &m);
        } else {
            func_001FB2F8(func_001FBB90(0x29), 0);
        }
        pg = D_003DD490;
        n = 2;
        qv = func_001A62E8(p);
        EmitSwNoise(pg, 0, qv, p);
        ((void (*)(void *, int, int, int, int, int))p->vt[3].pf)((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
        q = func_001CB658(p);
        if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 7)) {
            struct LO *plo = func_001CB658(p);
            func_001C7C30(plo->x100, 1);
            ((void (*)(void *))plo->vt[9].pf)((char *)plo + plo->vt[9].d);
            func_001618B0(pg);
        }
    }
    func_001C60B8(p, n);
}
