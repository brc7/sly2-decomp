// (Claude agent, lane A9, 2026-10-23) from the old draft (21): the dot is ONE asm tied to pos ("0"(pos)), and the float
// readback is an mtc1 asm taking the qword (gcc emits the qmfc2 itself). Together they fix every VF number.
typedef int qword __attribute__((mode(TI)));
struct VECTOR;
extern "C" VECTOR *func_002564C0(VECTOR *);
extern "C" qword *func_002564A8(qword *p, qword v);
extern "C" void SetPlane(qword *p, qword n, float d);
struct VECTOR { qword q; VECTOR() { (void)func_002564C0(this); } };
struct MAT { qword r[4]; };

extern "C" qword func_001193D8(MAT *pm, qword plane)
{
    union { qword q; float f[4]; } u;
    qword n, t;
    VECTOR v;
    v.q = plane;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(n) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(v.q));
    t = pm->r[3];
    func_002564A8(&u.q, plane);
    float d = u.f[3];
    qword pos, one, p2;
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(d));
    u.q = s;
    __asm__("vadda.xyzw $ACC, %2, $vf0\n\tvmaddx.xyzw %0, %1, %3x" : "=j"(pos) : "j"(n), "j"(t), "j"(s));
    __asm__("vmul.xyz %0, %0, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(p2), "=j"(one) : "0"(pos), "j"(n));
    float g;
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(p2));
    SetPlane(&u.q, n, g);
    return u.q;
}
