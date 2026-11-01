typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { qword r[4]; };
struct MATRIX3 { qword r[3]; };
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("" : "=r"(q) : "0"(*(int *)&f)); } };
struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0;
extern MATRIX4 D_002A2D30;
struct X { float x, y, z, w; qword axis; float gRot; float gScale; };
void func_0011B500(MATRIX3 *pm, float g, qword axis);
MATRIX4 operator*(const MATRIX4 &a, const MATRIX4 &b);
MATRIX4 operator*(const MATRIX4 &m, VU_FLOAT s);
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline MATRIX4 operator+(const MATRIX4 &a, const MATRIX4 &b) return r
{
    for (int i = 0; i < 4; i++)
        r.r[i] = Add(a.r[i], b.r[i]);
}
extern "C" MATRIX4 CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(X *p);
/** @sly2-annot addr=0x15BFD8
 * Purpose: Computes the drift/curl matrix of an EMITDV for time t
 * Info: proposed name CalculateEmitdvMatrix (T1); TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
MATRIX4 CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(X *p)
{
    MATRIX4 mScale = D_002A2D30;
    float *pf = (float *)&mScale;
    pf[0] = p->gScale;
    pf[5] = p->gScale;
    pf[10] = p->gScale;
    pf[12] = p->x;
    pf[13] = p->y;
    pf[14] = p->z;
    MATRIX4 mRot = D_002A2D30;
    MATRIX4 *pmRot = &mRot;
    if (p->gRot != 0.0f) {
        MATRIX3 m3;
        func_0011B500(&m3, p->gRot * D_002DDED0.dt, p->axis);
        *(MATRIX3 *)pmRot = m3;
    }
    MATRIX4 mD;
    mD = *pmRot * mScale * VU_FLOAT(D_002DDED0.dt);
    return *pmRot + mD;
}
