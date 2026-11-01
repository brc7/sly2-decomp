typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x70]; qword pos; };
struct O { char pad[0x19A4]; void *p19A4; char pad2[0x19C0 - 0x19A8]; struct T *p19C0; };
extern qword func_001A62E8(void *p);
extern qword ConvertAloPos(int n, struct O *p, qword v);
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
int func_0018ED98(struct O *p)
{
    union { qword q; float f[4]; } u;
    qword a, b, s, d;
    float t;
    int f;
    a = ConvertAloPos(0, p, func_001A62E8(p->p19A4));
    b = ConvertAloPos(0, p, p->p19C0->pos);
    t = 0.5f;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(t));
    u.q = s;
    __asm__("vmulax.xyzw ACC, %1, %2x\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(d) : "j"(a), "j"(s), "j"(b));
    f = 1;
    if (GetY(d) > 0.0f) {} else f = 0;
    return f;
}
