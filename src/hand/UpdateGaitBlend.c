typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct T { char pad[0x190]; qword v; };
union FLOATWORD { float value; int bits; };
struct R { char pad[0x40]; union FLOATWORD scale; };
struct ANIMATION;
struct S { struct T *pt; struct ANIMATION *animation; int pad[2]; float f10; char pad2[0x50 - 0x14]; float f50; int n54; };
void func_001384C0(struct ANIMATION *);
struct R *func_0020BB48(struct S *p);
qword func_0020BAD8(struct S *p);
static inline void InitScalar(U *value,int bits) {
    register int word __asm__("$2")=bits;
    register qword r __asm__("vf2");
    __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(word));
    value->q=r;
}
static inline qword Negate(qword v,U *value) {
    register qword scalar __asm__("vf1");
    register qword result __asm__("vf3");
    __asm__("qmtc2.ni %1,%0" : "=j"(scalar) : "r"(-1.0f));
    value->q=scalar;
    __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(result) : "j"(v),"j"(value->q));
    return result;
}
void UpdateGaitBlend(struct S *p, float g)
{
    struct T *pt;
    U b, a;
    qword q, r1, r2;
    if (p->n54 == 0) {
        p->f50 = 0.0f;
        p->f10 = 1.0f;
    } else {
        p->f50 = g;
        p->f10 = 1.0f - g;
    }
    func_001384C0(p->animation);
    pt = p->pt;
    InitScalar(&a,func_0020BB48(p)->scale.bits);
    b = a;
    q = func_0020BAD8(p);
    r1=Negate(q,&b);
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r2) : "j"(a.q), "j"(r1));
    pt->v = r2;
}
