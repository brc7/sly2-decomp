typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
union M4 { struct { qword r[4]; } q; M3 m3; float af[4][4]; };
struct CLOCK { float t; float dt; };
extern "C" CLOCK D_002DDED0;
struct CM {
    char pad0[0x20]; float g20; float g24; char pad1[0x60 - 0x28];
    M3 mat; qword pos; char pad2[0x300 - 0xA0]; int n300;
};
extern "C" CM D_002DDF40;
extern "C" char D_002BDAB0[], D_002BDAC0[];
struct LO;
struct LOV { char pad[0xF8]; short d31; short p31; M4 *(*pfn31)(LO *); };
struct LO { int a[3]; LOV *pvt; };
struct Y { char pad[0x1BC]; float g1BC; char pad1[0x1D0 - 0x1C0]; LO *plo; int n1D4; };
struct X { Y *py; };
extern "C" {
void UpdateCplcy(X *p);
M3 MulMatrix3Matrix3(M4 *pm, M3 *pm2);
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(M3 *pm, float *pg, qword *pv);
float GSmooth__FfffP3SMPPf(float gCur, float gTarget, float dt, void *psmp, float *pdgNext); /* GSmooth */
M3 func_0011B500(qword v, float g);
qword *VECTOR_opAssign(qword *p, qword v);
void SetCmMat4(CM *p, M4 *m);
void SetCmFov__FP2CMf(CM *p, float g);
void UpdateCpturret(X *p);
}
static inline M3 Mul(const M3 &a, const M3 &b) return r
{
    qword a0 = a.r[0], a1 = a.r[1], a2 = a.r[2];
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[0]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[0]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[1]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[1]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[2]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[2]));
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline float Length(qword a)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=&j"(one) : "j"(a), "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static inline qword AddScaled(qword a, qword b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(b), "j"(s.q));
    return r;
}

/** @sly2-annot addr=0x151950
 * Purpose: Camera follows turret object's world matrix (plo->vt31 PmatWorld), smoothing rotation/position
 *   unless cut; fov from turret +0x1BC
 * Info: proposed name UpdateCpturret (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/minigames.md#4-mounted-turret-turret-0x45-turret-tu-0x2259d8-0x226550-conf-high-ccode,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void UpdateCpturret(X *p)
{
    UpdateCplcy(p);
    Y *py = p->py;
    LO *plo = py->plo;
    M4 m = *plo->pvt->pfn31((LO *)((char *)plo + plo->pvt->d31));
    if (D_002DDF40.n300 == 0 && py->n1D4 != 0) {
        M3 mRel;
        M3 mNew;
        union { qword q; } axis;
        float rad;
        mRel = MulMatrix3Matrix3(&m, &D_002DDF40.mat);
        DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&mRel, &rad, &axis.q);
        if (rad < 0.0001f) {
            mNew = m.m3;
        } else {
            float g = GSmooth__FfffP3SMPPf(rad, 0.0f, D_002DDED0.dt, D_002BDAB0, 0);
            mNew = Mul(func_0011B500(axis.q, g), m.m3);
        }
        qword d = Sub(D_002DDF40.pos, m.q.r[3]);
        float dist = Length(d);
        qword pos;
        if (dist < 0.0001f)
            pos = m.q.r[3];
        else
            pos = AddScaled(m.q.r[3], d, VU_FLOAT(GSmooth__FfffP3SMPPf(dist, 0.0f, D_002DDED0.dt, D_002BDAC0, 0) / dist));
        VECTOR_opAssign(&m.q.r[3], pos);
        m.m3 = mNew;
        m.af[3][3] = 1.0f;
        m.af[2][3] = 0.0f;
        m.af[1][3] = 0.0f;
        m.af[0][3] = 0.0f;
    }
    SetCmMat4(&D_002DDF40, &m);
    if (D_002DDF40.g24 != D_002DDF40.g20)
        D_002DDF40.g24 = py->g1BC;
    else
        SetCmFov__FP2CMf(&D_002DDF40, py->g1BC);
}
