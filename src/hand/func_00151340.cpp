// func_00151340 (Lilly's agent, T1649): joystick / random-target camera spring. MATCH.
// Keys: the sibling helper style of func_00151700 / func_0014EB70 (by-value VU_FLOAT temps -> the sqc2 0(sp) after each qmtc2,
// VU_VECTOR(const qword &) wrappers, VecCombo(f1, v1, f2, v2)); Sub taking VU_VECTORs makes both p14->pos reads plain MEMs,
// so cse2 keeps the target's two lqc2 0x20(v0); k is read through a VU_FLOAT(const float *) ctor whose word input has a
// never-chosen "V" alternative (the func_0014F110 trick: lw instead of lwc1+mfc1).
typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern "C" CLOCK D_002DDED0;
struct CM;
extern "C" CM D_002DDF40;
struct CLQ { float g0, g1, g2, pad; };
struct LM { float gMin, gMax; };
extern "C" CLQ D_002BDA10;
extern "C" LM D_002BDA20;
extern "C" CLQ D_002BDA30;
extern "C" LM D_002BDA40;
extern "C" LM D_002BDA48, D_002BDA50, D_002BDA58;
extern "C" float D_002BDA60, D_002BDA64;
struct JOY { char pad[0x1070]; float x; float y; float g; };
struct GS { char pad[0x4CC]; int n4CC; };
struct TGT { char pad[0x20]; qword pos; };
struct OBJ { char pad[0x40]; int m40; };
struct X {
    GS *pgs; int pad4[3]; OBJ *p10; TGT *p14; float g18; int pad1C;
    union { qword q; float f[4]; } pos; qword v;
};
extern "C" {
void func_0014FD40(X *p, void *pv, JOY *pjoy);
float GRandInRange__Fff(float gMin, float gMax);
void SetActPositionGoal(TGT *p, qword pos);
void func_0014E168(CM *p, int *pm);
void func_00151340(X *p, void *pv, JOY *pjoy);
}
static inline float GFromClq(const CLQ &c, float g) { return c.g0 + g * (c.g1 + g * c.g2); }
static inline float GClampR(float g, float lo, float hi) { if (g < lo) return lo; if (hi < g) return hi; return g; }
static inline float GClamp(float g, float lo, float hi) { if (g < lo) g = lo; else if (g > hi) g = hi; return g; }
static inline float GRand(const LM &lm) { return GRandInRange__Fff(lm.gMin, lm.gMax); }
static inline qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__ __volatile__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
static inline qword Zero() { qword q; __asm__ __volatile__("vsub.xyzw %0, $vf0, $vf0" : "=j"(q)); return q; }
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(*(const int *)pg)); } /* never-chosen "V" alternative: allocation-only */ };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword Sub(VU_VECTOR a, VU_VECTOR b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a.q), "j"(b.q)); return d; }
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}
static inline qword AddScaled2(VU_VECTOR a, VU_VECTOR b, VU_VECTOR c, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %4x\n\tvmaddx.xyzw %0, %3, %4x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(c.q), "j"(s.q));
    return r;
}

void func_00151340(X *p, void *pv, JOY *pjoy)
{
    qword pos, v;
    func_0014FD40(p, pv, pjoy);
    if (0.0f < pjoy->g) {
        float sx, sy;
        sx = 1.0f;
        if (0.0f < pjoy->x)
            sx = -1.0f;
        sy = -1.0f;
        if (0.0f < pjoy->y)
            sy = 1.0f;
        float ax = sx * GClampR(GFromClq(D_002BDA10, __builtin_fabsf(pjoy->x)), D_002BDA20.gMin, D_002BDA20.gMax);
        float ay = GClamp(GFromClq(D_002BDA30, __builtin_fabsf(pjoy->y)), D_002BDA40.gMin, D_002BDA40.gMax);
        float y = sy * ay;
        v = Zero();
        union { qword q; } u;
        u.q = Make(0.0f, y, ax);
        pos = u.q;
    } else if (p->pgs->n4CC != 9) {
        qword zero = Zero();
        pos = zero;
        v = pos;
    } else {
        if (p->g18 < D_002DDED0.t) {
            float y = GRand(D_002BDA48);
            float z = GRand(D_002BDA50);
            float *pf = p->pos.f;
            pf[0] = 0.0f;
            pf[1] = y;
            pf[2] = z;
            float t = D_002DDED0.t;
            p->g18 = t + GRand(D_002BDA58);
        }
        float dt = D_002DDED0.dt;
        qword accel = VecCombo(VU_FLOAT(&D_002BDA60), Sub(p->pos.q, p->p14->pos), VU_FLOAT(-D_002BDA64), p->v);
        v = AddScaled(p->v, accel, VU_FLOAT(dt));
        pos = AddScaled2(p->p14->pos, p->v, v, VU_FLOAT(dt * 0.5f));
    }
    SetActPositionGoal(p->p14, pos);
    p->v = v;
    func_0014E168(&D_002DDF40, &p->p10->m40);
}
