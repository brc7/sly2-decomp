typedef int qword __attribute__((mode(TI)));
struct K { int a, b; };
extern qword D_002BE7D0;
extern struct K D_002BE7E0;
struct S { char pad[0x40]; int fl40; char pad2[0x13C]; int n180; char pad3[0x1C]; qword q1A0; struct K k1B0; };
void InitSo(struct S *p);
void func_0017F090(struct S *p) {
    int v;
    InitSo(p);
    p->n180 = 10;
    p->q1A0 = D_002BE7D0;
    p->k1B0 = D_002BE7E0;
    v = p->fl40;
    if ((v ^ 1) & 1) *(int *)v |= 2; else p->fl40 = v | 2;
}
