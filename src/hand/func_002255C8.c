typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x80]; qword v; };
struct O { char pad[0xE8]; struct T *t; };
struct Q { int a; struct O *o; };
struct S { char pad[0x1B0]; void *x; };
extern qword D_002A2DD0;
extern char D_002C43D0[], D_002C43E0[];
void AddSoAcceleration(struct O *o, qword v);
qword func_001A62E8(void *p);
void AccelSoTowardPosSpring(struct O *o, qword v, void *a, qword b, void *c);
void func_002255C8(struct S *p, struct Q *q)
{
    struct O *o = q->o;
    union { qword q; float f[4]; } u;
    qword r, v;

    float m = -1.0f;
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(m));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(o->t->v), "j"(s));
    u.q = s;
    AddSoAcceleration(o, r);
    if (p->x) v = func_001A62E8(p->x); else v = func_001A62E8(p);
    AccelSoTowardPosSpring(o, v, D_002C43D0, D_002A2DD0, D_002C43E0);
}
