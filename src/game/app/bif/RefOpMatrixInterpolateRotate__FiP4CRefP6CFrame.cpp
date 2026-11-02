// Script matrix-slerp builtin. Matched by Claude agent (2026-10-17): MulM3 uses the g++ named return value (`return m`),
// stores each row right after its asm, and also writes a second (dead) M3 copy t, which is the target's 0xB0 slot.
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef union { qword q; float g; } UF;
struct MATRIX4 { float m[4][4]; };
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
};
extern "C" {
M3 MulMatrix3Matrix3(M3 *pa, M3 *pb);
void func_0011BCF8(M3 *pmat, float *prad, qword *pvec);
M3 func_0011B500(float rad, qword axis);
void *func_002333B4(void *pv, int n, int cb);
}
static inline M3 MulM3(const M3 &a, M3 *pb) return m
{
    M3 t;
    qword r0, r1, r2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r0) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[0]));
    m.r[0] = r0; t.r[0] = r0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r1) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[1]));
    m.r[1] = r1; t.r[1] = r1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r2) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(pb->r[2]));
    m.r[2] = r2; t.r[2] = r2;
}
extern "C" CRef RefOpMatrixInterpolateRotate__FiP4CRefP6CFrame(void *pv, CRef *aref);
/** @sly2-annot addr=0x122F00
 * Purpose: Splice builtin "matrix-interpolate-rotate": decompose+rebuild rotation (0x11BC00/0x11B500)
 * Info: proposed name RefOpMatrixInterpolateRotate (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpMatrixInterpolateRotate__FiP4CRefP6CFrame(void *pv, CRef *aref)
{
    M3 mat = MulMatrix3Matrix3((M3 *)aref[0].u.pv, (M3 *)aref[1].u.pv);
    UF normal;
    float rad;
    func_0011BCF8(&mat, &rad, &normal.q);
    CRef ref(0x1B);
    MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
    func_002333B4(pm, 0, 0x40);
    pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = pm->m[3][3] = 1.0f;
    *(M3 *)ref.u.pv = MulM3(func_0011B500(rad * aref[2].u.g, normal.q), (M3 *)aref[0].u.pv);
    return ref;
}
