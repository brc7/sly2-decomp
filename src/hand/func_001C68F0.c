typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct MAT { qword a[3]; };
struct LO { char pad[0xC]; struct VTE *vt; };
struct S {
    char pad[0xC]; struct VTE *vt;
    char p0[0x150 - 0x10]; int x150;
    char p1[0x210 - 0x154]; struct LO *x210;
    char p2[0x230 - 0x214]; float x230; float x234; float x238; int x23C; int x240;
};
struct X {
    char pad[0x1C]; struct S *x1C; char p1[0x40 - 0x20]; struct MAT mat;
    char p2[0xEC - 0x70]; struct VTE *vt; void *xF0; struct S *xF4; int xF8;
};
struct M { int n; struct S *p; int pad[14]; };
extern char D_003DD490[];
extern char D_002C0E10[];
extern char D_002DEB30[];
float GRandInRange__Fff(float a, float b);
struct X *func_001CE888(void *pg, int a, int b);
struct LO *func_001C4840(void *pg);
qword func_001A62E8(struct S *p);
struct MAT *func_001A6318(struct S *p);
void func_001C6AB0(struct S *p, int n);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void func_00161868(void *pg);
void func_001619A0(void *p, int n, struct S *ps);
void func_001C60B8(struct S *p, int n);
void func_001C68F0(struct S *p, int n)
{
    if (p->x150 == n) return;
    switch (n) {
    case 0:
        p->x238 = GRandInRange__Fff(p->x230, p->x234);
        break;
    case 1: {
        struct LO *q = func_001C4840(D_003DD490);
        struct X *px;
        struct LO *plo;
        if (!((int (*)(void *))q->vt[80].pf)((char *)q + q->vt[80].d)) {
            n = p->x150;
            break;
        }
        px = func_001CE888(D_003DD490, 9, 0);
        if (px) {
            struct VTE *e;
            px->xF4 = p;
            px->xF0 = D_002C0E10;
            px->xF8 = 0;
            e = &px->vt[1];
            ((void (*)(void *, qword, int, float))e->pf)((char *)px + e->d, func_001A62E8(p), 0, 1.0f);
            px->x1C = p;
            px->mat = *func_001A6318(p);
        } else {
            func_001C6AB0(p, 0);
        }
        ((void (*)(void *, int, int))q->vt[81].pf)((char *)q + q->vt[81].d, p->x23C, p->x240);
        plo = p->x210;
        if (plo) {
            struct M m;
            m.n = 1;
            m.p = p;
            ((void (*)(void *, struct M *))plo->vt[51].pf)((char *)plo + plo->vt[51].d, &m);
        }
        n = 2;
        ((void (*)(void *, int, int, int, int, int))p->vt[3].pf)((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
        break;
    }
    }
    func_001C60B8(p, n);
}
