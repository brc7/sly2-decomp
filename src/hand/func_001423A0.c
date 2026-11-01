typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct BUF { char pad[0x30]; qword pos; };
struct VTE { short d; short pad; void *(*pfn)(void *, struct BUF *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct X994 { char pad[0x1C]; float t; };
struct P {
    char pad[0x4C8]; int x4C8; char pad1[0x4D4 - 0x4CC]; int x4D4; char pad2[0x934 - 0x4D8]; struct O *po;
    char pad3[0x994 - 0x938]; struct X994 *p994; char pad4[0x9A4 - 0x998]; int x9A4;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void LoadIdentityMatrix4(void *a, int b, M4 *pm, int c, float g);
qword func_001A62E8(void *p);
static __inline__ qword Mul(M4 *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(o) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]), "j"(v));
    return o;
}
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
int func_001423A0(struct P *p)
{
    struct BUF buf;
    M4 m;
    qword v;
    if (p->x4C8 != 5 || !p->x4D4) return 1;
    LoadIdentityMatrix4(p->po->vt[32].pfn((char *)p->po + p->po->vt[32].d, &buf), 0, &m, 0, 0.0f);
    v = Mul(&m, buf.pos);
    if (5.0f < GDist(func_001A62E8(p->po), v)) return 0;
    if (p->x9A4 != 1) return 1;
    if (D_002DDED0.t < p->p994->t) return 0;
    return 1;
}
