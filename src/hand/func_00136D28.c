typedef int ti128 __attribute__((mode(TI)));
struct E { char pad[0x18]; int f18; };
struct S { int f0; int f4; int pad[6]; ti128 v20; float f30; int f34; struct E ae[6]; };
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(int, int, int, struct E *, int, int, int, int *);
void func_00136D28(struct S *p, int a, ti128 v, float g0, float g1) {
    unsigned int i;
    p->f0 = a;
    p->v20 = v;
    p->f30 = g1;
    p->f4 = -1;
    for (i = 0; i < 6; i++) EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(p->f0, 0xc, 2, &p->ae[i], 0, 0, 0, &p->ae[i].f18);
}
