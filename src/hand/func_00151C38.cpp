// func_00151C38 (Claude agent, lane D12): matched from the v7 draft. Normalize's sqrt and divide are two asms (sqrt into a
// separate length temp), which lengthens the VU chain so the scheduler orders the block like the target; Cross lists `a`
// twice (allocation-only operand) so the VF numbering matches.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct M3 { qword r[3]; };
union M4 { struct { qword r[4]; } q; M3 m3; float af[4][4]; };
struct CM;
extern "C" CM D_002DDF40;
struct CLQ { float g0, g1, g2, pad; };
struct LM { float gMin, gMax; int pad[2]; };
extern "C" float D_002BDACC;
extern "C" CLQ D_002BDAD0, D_002BDAF0, D_002BDB10, D_002BDB30;
extern "C" LM D_002BDAE0, D_002BDB00, D_002BDB20, D_002BDB40;
extern "C" qword D_002A2DC0;
struct ALO;
struct X { char pad[0x10]; ALO *p10; ALO *p14; };
extern "C" {
void func_0014FD40(X *p);
qword func_001A62E8(ALO *p);
M3 *func_0011B7E0(M3 *pm, qword v);
qword *func_00256D18(qword *p, qword v);
void func_0014E168(CM *p, M4 *m);
void func_0014F5A0(CM *p, float g);
void func_00151C38(X *p);
}
static inline float GFromClq(const CLQ &c, float g) { return c.g0 + g * (c.g1 + g * c.g2); }
static inline float GClampR(float g, float lo, float hi) { if (g < lo) return lo; if (hi < g) return hi; return g; }
static inline float GClamp(float g, float lo, float hi) { if (g < lo) g = lo; else if (g > hi) g = hi; return g; }
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); } VU_FLOAT(const float &f, int) { __asm__("lw %0, %1" : "=r"(q) : "m"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q), "j"(f1.q), "j"(f2.q)); /* extra unused operands: allocation only */
    return r;
}
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
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
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b), "j"(a)); /* extra operand: allocation only */
    return r;
}
static inline qword Normalize(qword a)
{
    qword d, one, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=&j"(one) : "j"(a));
    qword l;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(a), "j"(l));
    return r;
}
struct VU_FLOATJ { qword q; VU_FLOATJ(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
static inline qword Scale(qword a, VU_FLOATJ s) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(a), "j"(s.q)); return r; }

static inline void Cm1(M4 *pm) { func_0014E168(&D_002DDF40, pm); }
static inline void Cm2(float g) { func_0014F5A0(&D_002DDF40, g); }
/** @sly2-annot addr=0x151C38
 * Purpose: Frame two objects: focus = weighted mix at z=150, eye offset sideways (cross with up) and up by
 *   distance curves, look-at matrix, fov from distance curve
 * Info: proposed name UpdateCptwoshot (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_00151C38(X *p)
{
    U d;
    func_0014FD40(p);
    d.q = Sub(func_001A62E8(p->p10), func_001A62E8(p->p14));
    d.f[2] = 0.0f;
    float dist = Length(d.q);
    U focus;
    focus.q = VecCombo(VU_FLOAT(D_002BDACC, 0), VU_VECTOR(func_001A62E8(p->p10)), VU_FLOAT(1.0f - D_002BDACC), VU_VECTOR(func_001A62E8(p->p14)));
    focus.f[2] = 150.0f;
    U pos;
    pos.q = focus.q;
    pos.f[2] += GClampR(GFromClq(D_002BDAD0, dist), D_002BDAE0.gMin, D_002BDAE0.gMax);
    qword n = Normalize(Cross(d.q, D_002A2DC0));
    float g = GClamp(GFromClq(D_002BDAF0, dist), D_002BDB00.gMin, D_002BDB00.gMax);
    U pos2;
    pos2.q = Add(focus.q, Scale(n, VU_FLOATJ(g)));
    pos2.f[2] += GClamp(GFromClq(D_002BDB10, dist), D_002BDB20.gMin, D_002BDB20.gMax);
    {
        M4 m4;
        M3 m3;
        func_0011B7E0(&m3, Sub(pos.q, pos2.q));
        M4 *pm4 = &m4;
        func_00256D18(&pm4->q.r[3], pos2.q);
        pm4->q.r[0] = m3.r[0];
        pm4->q.r[1] = m3.r[1];
        pm4->q.r[2] = m3.r[2];
        m4.af[2][3] = 0.0f;
        m4.af[1][3] = 0.0f;
        m4.af[0][3] = 0.0f;
        pm4->af[3][3] = 1.0f;
        Cm1( pm4);
    }
    Cm2( GClamp(GFromClq(D_002BDB30, dist), D_002BDB40.gMin, D_002BDB40.gMax));
}
