typedef int qword __attribute__((mode(TI)));
struct P { void *p0; char pad[0xC]; qword pos; int d20; float t24; int f28; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern float D_002BD2D8;
extern float D_002BD2D0;
qword func_001A62E8(void *p);
static __inline__ float GDist(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static __inline__ int IDist(qword a, qword b)
{
    qword d, t;
    int g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
void SetJtgoal(struct P *p, qword pos)
{
    if (p->f28 == 0) {
        qword r;
        int fNear = GDist(p->pos, pos) < D_002BD2D8;
        if (fNear) return;
        p->pos = pos;
        r = func_001A62E8(p->p0);
        p->d20 = IDist(r, p->pos);
        p->t24 = D_002DDED0.t + D_002BD2D0;
    }
}
