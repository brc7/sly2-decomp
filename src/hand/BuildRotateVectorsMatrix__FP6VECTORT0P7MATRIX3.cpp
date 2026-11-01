// BuildRotateVectorsMatrix (Claude agent, lane A25, 2026-10-23): MATCH, fresh rewrite (old best fresh2 = 47).
// Rotation matrix taking direction a onto b (Sly 1 LoadRotateVectorMatrix-like): identity if either length < 1e-4,
// else axis = normalized a x b (GetNormalVector(a) fallback), cos = clamp(dot / (|a||b|)), sin = sqrt(1 - cos^2).
// Matching notes:
//  - la/lb are VU pseudos that get no hard register (spill slots 0x40/0x50). For that their reads must not pull
//    the regclass toward GPRs: H() lists a second, never-chosen "j" alternative (allocation-only; reload always
//    picks the first "r" alternative, so the instruction is the same mtc1).
//  - d.G(): the one register pin (copy to $v0 before the mtc1), so the dot readback stays after la*lb.
//  - The sqrt asm is volatile: emulates the original schedule (the B480 argument setup stays after it).
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
struct VU_FLOAT {
    qword q;
    VU_FLOAT() {}
    VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); }
    float F() const { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
    /* "r,j": the j alternative is allocation-only (never selected), see header */
    float H() const { float g; __asm__("mtc1 %1, %0" : "=f,f"(g) : "r,j"(q)); return g; }
    /* one register pin: $v0 copy, see header */
    float G() const { register qword t __asm__("$2") = q; float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(t)); return g; }
};
extern "C" {
void *func_002333B4(void *pv, int n, int cb);
void *func_0023320C(void *pvDst, const void *pvSrc, int cb);
qword GetNormalVector(qword v);
float func_0011CFE0(float g, float gMax);
M3 LoadRotateMatrixSinCos(VU_FLOAT s, VU_FLOAT c, qword axis);
}
static inline qword Len(qword a)
{
    qword l;
    qword d, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    return l;
}
static inline qword Dot(qword a, qword b)
{
    qword d;
    qword one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
struct MAT3 { float m[3][4]; };
extern "C" M3 *BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(M3 *pm, qword a, qword b);
M3 *BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(M3 *pm, qword a, qword b)
{
    VU_FLOAT la, lb;
    la.q = Len(a);
    lb.q = Len(b);
    if (la.H() < 1e-4f || lb.H() < 1e-4f) {
        MAT3 *p = (MAT3 *)pm;
        func_002333B4(pm, 0, 0x30);
        p->m[0][0] = p->m[1][1] = p->m[2][2] = 1.0f;
    } else {
        qword axis;
        __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(axis) : "j"(a), "j"(b));
        VU_FLOAT l;
        l.q = Len(axis);
        if (l.F() > 1e-8f) {
            __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(axis) : "j"(axis), "j"(l.q));
        } else {
            axis = GetNormalVector(a);
        }
        VU_FLOAT d;
        d.q = Dot(a, b);
        float ll = la.H() * lb.H();
        VU_FLOAT c = func_0011CFE0(VU_FLOAT(d.G() / ll).F(), 1.0f);
        float cc = c.F();
        float sn;
        /* volatile: emulates the original schedule, see header */
        __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(sn) : "f"(1.0f - cc * cc));
        M3 m = LoadRotateMatrixSinCos(VU_FLOAT(sn), c, axis);
        func_0023320C(pm, &m, sizeof(M3));
    }
    return pm;
}
