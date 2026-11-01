typedef int qword __attribute__((mode(TI)));
struct L { char pad[0x28]; unsigned long fl28; };
struct Y { char pad[0x50]; void *p50; char pad2[0x5C - 0x54]; void *p5C; };
struct X { char pad[0x10DC]; void *p10DC; int pad2; int n10E4; char pad3[0x10F0 - 0x10E8]; int n10F0; };
struct S {
    char pad0[0x2C0]; unsigned long fl2C0; char pad1[0x364 - 0x2C8]; int n364; char a368[8]; char a370[8]; float f378; int n37C;
    char a380[8]; float f388; void *p38C; char pad2[0x3F4 - 0x390]; struct L *p3F4; struct L *p3F8;
};
extern float D_002C3D90;
extern float D_002C3D94;
void HeroInteractDistance(struct X *x, struct S *p, void *pv, float *pd, float *pa);
unsigned long func_00207A20(int n);
int func_00209A68(struct S *p, int n);
struct Y *func_00205458(struct S *p, int n);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *po, int n);
qword func_001A62E8(void *p);
int FCheckVolPoint(struct L *pl, qword q);
int func_0011D910(void *p, float f);

int func_00208E70(struct S *p, struct X *x, float *pout)
{
    float d, ang;
    void *o;
    struct L *pl;
    void *pv;
    HeroInteractDistance(x, p, &x->p10DC, &d, &ang);
    if (pout)
        *pout = d;
    if (!(p->fl2C0 & func_00207A20(1))) {
        if (p->n364 && x->n10F0) {
            if (func_00209A68(p, 0x1D))
                o = p->p38C;
            else if (func_00209A68(p, 5))
                o = func_00205458(p, 0xA1)->p50;
            else if (func_00209A68(p, 6))
                o = func_00205458(p, 0xA2)->p50;
            else if (func_00209A68(p, 0x17)) {
                o = func_00205458(p, 0xB1)->p5C;
                if (o == x)
                    *pout += D_002C3D90;
            } else
                o = 0;
            if (o) {
                if (o == x)
                    return 4;
                if (FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x34))
                    return 4;
            }
        }
        if (p == x->p10DC && x->n10E4 == 3)
            pl = p->p3F8;
        else
            pl = p->p3F4;
        if (pl && !!((pl->fl28 >> 60) & 1) && FCheckVolPoint(pl, func_001A62E8(x)))
            return 3;
    }
    pv = func_00209A68(p, 0x17) ? p->a368 : p->a370;
    if (d <= p->f378 * D_002C3D94 && func_0011D910(pv, ang) && p->n364)
        return 2;
    if (d <= p->f388 && func_0011D910(p->a380, ang) && p->n37C)
        return 1;
    return 0;
}
