// func_00137248 (Claude agent laneB24, 2026-10-23): matched. len as an mtc1 asm from the qword (gcc emits the qmfc2), cz via a pextuw asm with a float "=r" output (gcc emits the mtc1).
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
union UQ { qword q; };
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
struct VU_FLOATM {
    qword q;
    VU_FLOATM(const float &f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "rV"(*(int *)&f)); }
};
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Dot(qword a, qword b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
        : "=j"(r), "=&j"(one) : "j"(a), "j"(b));
    return r;
}
static inline qword Length(qword a)
{
    qword r, t, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
        : "=&j"(t), "=&j"(one) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(t));
    return r;
}
static inline qword Scale(qword v, qword f)
{
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f), "j"(v));
    return r;
}
static inline qword DivQ(qword v, qword f)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(f));
    return r;
}
static inline qword DivF(qword v, VU_FLOATM f)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(f.q));
    return r;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword AddScaled(qword a, qword v, VU_FLOAT f)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(v), "j"(f.q));
    return r;
}
static inline qword MulM3(const M3 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(v), "j"(m.r[0])); /* extra operand: allocation only */
    return r;
}
static inline float GetX(qword q)
{
    float f;
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(q));
    return f;
}
static inline float GetXM(qword q)
{
    float f;
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q));
    return f;
}
static inline float GetZ(qword q)
{
    float f;
    __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q));
    return f;
}

static inline void SafeDiv(UQ &v, qword len, const qword &vDef)
{
    if (GetX(len) < 0.0001f)
        v.q = vDef;
    else
        v.q = DivQ(v.q, len);
}

#define V4(a) virtual void a##0(); virtual void a##1(); virtual void a##2(); virtual void a##3();
struct BASE { int x0, x4, x8; };
struct P : BASE {
    V4(a) V4(b) V4(c) V4(d) V4(e) V4(f) V4(g) V4(h)
    virtual qword GetVel();            /* 33 */
};
struct PX { char pad[0x10DC]; P *py; };
struct S { P *p; };
struct CLOCK { float t; float dt; };
struct SMP { float a, b, c; };
extern "C" {
extern CLOCK D_002DDED0;
extern float D_002BC998, D_002BC99C, D_002BC9A0;
extern SMP D_002BC9A8;
extern qword D_002A2DC0, D_002A2DA0;
qword func_001A62E8(P *p);
float GSmooth__FfffP3SMPPf(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);
float func_0011CFE0(float g, float gMax);
M3 func_0011B500(qword axis, float g);
qword LimitVectorLength(qword v, float g);
void AddSoAcceleration(P *p, qword v);
}

/** @sly2-annot addr=0x137248
 * Purpose: Relative velocity/position correction against a ridden object
 * Info: proposed name JT_VUnk69 (T2); class JT; TU bentley
 * Notes: agent qualifier: helper
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
extern "C" void func_00137248(S *s)
{
    P *p = s->p;
    P *py = ((PX *)p)->py;
    if (py == 0)
        return;
    float gMax = D_002BC99C * D_002BC99C / D_002BC998 + D_002BC9A0;
    qword dv = Sub(p->GetVel(), py->GetVel());
    UQ n;
    qword d0 = Sub(func_001A62E8(s->p), func_001A62E8(py));
    n.q = Sub(d0, Scale(D_002A2DC0, Dot(d0, D_002A2DC0)));
    qword qlen = Length(n.q);
    SafeDiv(n, qlen, D_002A2DA0);
    qword c = Cross(n.q, dv);
    float len = GetXM(qlen);
    float cz = GetZ(c);
    float dot = GetX(Dot(dv, n.q));
    float w = cz / len;
    float dgNext;
    GSmooth__FfffP3SMPPf(len, D_002BC998, D_002DDED0.dt, &D_002BC9A8, &dgNext);
    float g = func_0011CFE0(dgNext - dot, D_002BC9A0 * D_002DDED0.dt);
    qword t = AddScaled(dv, n.q, VU_FLOAT(g));
    M3 mc;
    M3 m = func_0011B500(D_002A2DC0, w * D_002DDED0.dt);
    mc = m;
    qword r = DivF(Sub(MulM3(mc, t), dv), VU_FLOATM(D_002DDED0.dt));
    AddSoAcceleration(s->p, LimitVectorLength(r, gMax));
}
