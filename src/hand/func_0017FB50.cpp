#include "mat.h"
typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1C0]; int n1C0; char pad2[0x54]; float g218; int pad3; qword q220; };
extern "C" {
extern qword D_002A2DC0, D_002A2DD0;
extern char D_002BE800[], D_002BE810[];
MATRIX3 func_0011B500(qword q, float g);
void func_001F59A0(S *p, MATRIX3 *pm, void *pv);
void AccelSoTowardPosSpring(S *p, qword a, int b, qword c, void *pv);
void func_0017FB50(S *p);
}
void func_0017FB50(S *p) {
    if (p->n1C0 == 3) return;
    MATRIX3 m; m = func_0011B500(D_002A2DC0, p->g218);
    func_001F59A0(p, &m, D_002BE800);
    AccelSoTowardPosSpring(p, D_002A2DD0, 0, p->q220, D_002BE810);
}
