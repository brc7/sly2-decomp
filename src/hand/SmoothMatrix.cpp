// SmoothMatrix (Sly 1 name). Matched by Claude agent (2026-10-23): g++ named-return MulM3 with a dead M3 copy t
// (stored before m, per row), the first mat in its own block so the MulM3 temp and the VU scalar u reuse its slot.
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef union { qword q; float g; } UF;
struct SMP;
extern "C" {
M3 MulMatrix3Matrix3(M3 *pa, M3 *pb);
void func_0011BCF8(M3 *pmat, float *prad, qword *pvec);
float GSmooth__FfffP3SMPPf(float g, float gGoal, float dt, struct SMP *psmp, float *pdv);
M3 func_0011B500(float rad, qword axis);
}
static inline M3 MulM3(const M3 &a, M3 *pb) return m
{
    M3 t;
    qword r0, r1, r2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r0) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[0]));
    t.r[0] = r0; m.r[0] = r0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r1) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[1]));
    t.r[1] = r1; m.r[1] = r1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r2) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[2]));
    t.r[2] = r2; m.r[2] = r2;
}
extern "C" void SmoothMatrix(M3 *pmatPrev, M3 *pmatGoal, struct SMP *psmp, float dt, M3 *pmatNext, qword *pw);
void SmoothMatrix(M3 *pmatPrev, M3 *pmatGoal, struct SMP *psmp, float dt, M3 *pmatNext, qword *pw)
{
    UF normal;
    float rad, dv, radNew;
    {
        M3 mat = MulMatrix3Matrix3(pmatPrev, pmatGoal);
        func_0011BCF8(&mat, &rad, &normal.q);
    }
    radNew = GSmooth__FfffP3SMPPf(0.0f, rad, dt, psmp, &dv);
    if (pmatNext) {
        if (radNew == rad) {
            *pmatNext = *pmatGoal;
        } else {
            *pmatNext = MulM3(func_0011B500(radNew, normal.q), pmatPrev);
        }
    }
    if (pw) {
        UF u;
        qword s, r;
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(*(int *)&dv));
        u.q = s;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(normal.q), "j"(s));
        *pw = r;
    }
}
