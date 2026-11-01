#include "mat.h"
struct LO { char pad[0x154]; int x154; };
struct S {
    int a[4]; float g10, g14, g18, g1c, g20, g24, g28, g2c; int n30; int pad34[3];
    struct MATRIX3 mat;
    float g70, g74; int pad78; struct LO *x7C; int x80;
};
struct J { char pad[0x103C]; float a, b; char p1[0x1070 - 0x1044]; float c, d; char p2[0x10B4 - 0x1078]; int x10B4; int p3; int x10BC; };
extern int D_002DFBC0;
extern struct J D_002DFC00;
extern struct J D_002E0D00;
extern char D_002DDFA0[];
void ResetPoctl(struct S *p);
void func_0011BC88(void *pv, float *pa, float *pb);
struct MATRIX3 LoadRotateMatrixPanTilt(float a, float b);
void func_001C4330(struct S *p, float g, float *px, float *py, float *pang, float *pr, float *pr2);
void UpdatePoctlPressed(struct S *p);
void UpdatePoctl(struct S *p)
{
    float a, b;
    struct J *pj;
    b = 0.0f;
    a = 0.0f;
    switch (p->x80) {
    case 0:
        ResetPoctl(p);
        break;
    case 1:
        pj = (D_002DFBC0 & 2) ? &D_002DFC00 : &D_002E0D00;
        p->g10 = pj->a;
        p->g14 = pj->b;
        p->g20 = pj->c;
        p->g24 = pj->d;
        p->n30 = (pj->x10B4 & ~pj->x10BC) | p->x7C->x154;
        func_0011BC88(D_002DDFA0, &a, &b);
        break;
    }
    p->mat = LoadRotateMatrixPanTilt(a, b);
    func_001C4330(p, a, &p->g10, &p->g14, &p->g18, &p->g1c, &p->g70);
    func_001C4330(p, a, &p->g20, &p->g24, &p->g28, &p->g2c, &p->g74);
    UpdatePoctlPressed(p);
}
