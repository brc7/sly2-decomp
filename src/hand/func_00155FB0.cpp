// func_00155FB0 (Claude agent, lane D20): matched from v8.
// g_normalZ: Sly 1 name of the (0,0,1) vector at D_002A2DC0.
// The clamp scale lives in a function-scope VUV sc (declared after lm) so its slot sits at 0x30 before frame.
// Sub2 clobbers vf4, which is dead there (the next write to vf4 is the qmtc2 of func_0011E208's result), so the pos reload takes vf2.
// The empty volatile asm in the loop stops jump threading of the two f tests.
// DRAFT (Claude agent): escape/flee target position search (logic complete, not close: frame 0x130 vs 0x1D0 - target returns func_256CE8/256D08 VECTOR4 results via hidden pointer slots 0xB0..0xE0 and spills more temps; unreachable `b; mov.s f22,f20` block at 0x1561EC not reproduced).
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct VECTOR { float x, y, z, w; };
struct LM { float gMin, gMax; };
class SO;
struct VT { char pad[0x108]; short d; short pad2; qword (*pfn)(SO *); };
class SO { public: char pad[0xC]; VT *pvt; };
struct VUV { qword q; VUV() {} VUV(const VUV &v) : q(v.q) {} };
struct FRAME { int a[4]; };
extern "C" {
extern char D_003DD490[];
extern qword g_normalZ;
extern float D_002BDC98;
extern VECTOR D_002BDC60;
extern VECTOR D_002BDC80;
extern float D_002BDC70[2];
extern float D_002BDC90[2];
extern float D_002BDCA0[11];
extern float D_002A513C;
SO *func_001C4840(void *p);
extern char D_002DE578[];
qword func_001A62E8(void *p);
M3 *func_001A6318(void *p);
float func_00154C90(void *p);
VUV func_00256CE8(float g);
VUV func_00256D08(VECTOR *pv);
qword func_00258480(qword a, qword b, qword c, qword d);
void func_00256D18(VECTOR *pv, qword v);
float func_002458F0(float y, float x);
qword func_0011E1A0(float a, float b, float c);
void func_0011EFB0(qword v, float *pa, float *pb, float *pc);
FRAME *func_001AE478(FRAME *pf);
void func_001AE4A0(FRAME *pf, int n);
void func_001B44D8(int *pc, void **pa);
float func_0011CF60(float g);
qword func_0011E208(float pan, float tilt, float s);
int func_0014CC58(int n, qword a, qword b, int c, void *pv, qword *phit);
qword func_00155FB0(void *p, qword pos);
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}
static inline qword Scale(VU_VECTOR v, VU_FLOAT s) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v.q), "j"(s.q), "j"(v.q)); /* extra operand: allocation only */ return r; }
static inline qword ScaleR(VU_VECTOR v, const VUV &s) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v.q), "j"(s.q), "j"(v.q)); return r; }
static inline qword Sub2(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b) : "vf4"); /* clobber of dead vf4 */ return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b), "j"(a)); /* extra operand: allocation only */ return d; }
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline float Len(qword a)
{
    qword d, t;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(t) : "j"(a));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
static inline float GClampR(float g, LM *plm) { if (g < plm->gMin) return plm->gMin; if (plm->gMax < g) return plm->gMax; return g; }

static inline int FFast(qword v) { return D_002BDC98 < Len(v); }

/** @sly2-annot addr=0x155FB0
 * Purpose: Escape target: projects the player's motion by a difficulty-scaled time and finds the clearest of
 *   11 directions
 * Info: proposed name PosChooseEscape (T2); TU tu_1553F0
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
qword func_00155FB0(void *p, qword pos)
{
    SO *pso = func_001C4840(D_003DD490);
    float dist = Len(Sub(func_001A62E8(pso), pos));
    float u = func_00154C90(D_002DE578);
    qword posTarget = AddScaled(func_001A62E8(pso), g_normalZ, VU_FLOAT(50.0f));
    VUV off;
    VECTOR clq;
    LM lm;
    VUV sc;
    qword vel = pso->pvt->pfn((SO *)((char *)pso + pso->pvt->d));
    FRAME frame;
    VUV hit;
    qword pad[5]; /* unused: frame layout */
    float pan, tilt, s, gBest, panBest;
    int c;
    void *av;
    unsigned int i;
    if (FFast(vel)) {
        float u1 = 1.0f - u;
        float g;
        VECTOR *pv0 = &D_002BDC60;
        VECTOR *pv1 = &D_002BDC80;
        qword cq = func_00258480(func_00256CE8(u1).q, func_00256D08(pv0).q, func_00256CE8(u).q, func_00256D08(pv1).q);
        func_00256D18(&clq, cq);
        g = clq.x + dist * (clq.y + dist * clq.z);
        lm.gMin = u1 * D_002BDC70[0] + u * D_002BDC90[0];
        lm.gMax = u1 * D_002BDC70[1] + u * D_002BDC90[1];
        {
            qword t;
            __asm__("qmtc2.ni %1, %0" : "=j"(t) : "r"(GClampR(g, &lm)));
            sc.q = t;
        }
        off.q = ScaleR(vel, sc);
    } else {
        M3 *pm = func_001A6318(pso);
        M3 *pm2 = func_001A6318(pso);
        off.q = func_0011E1A0(func_002458F0(((float *)pm)[1], ((float *)pm2)[0]), D_002BDC98, 0.0f);
    }
    func_0011EFB0(Sub(Add(posTarget, off.q), pos), &pan, &tilt, &s);
    func_001AE478(&frame);
    func_001B44D8(&c, &av);
    gBest = D_002A513C;
    for (i = 0; i < 11; i++) {
        qword test;
        float g;
        float panT = func_0011CF60(pan + D_002BDCA0[i]);
        test = Add(pos, func_0011E208(panT, tilt, s));
        int f = func_0014CC58(0, pos, test, c, av, &hit.q);
        if (f)
            test = hit.q;
        __asm__ __volatile__(""); /* emulates the original compiler not threading this jump */
        if (!f) {
            panBest = panT;
            break;
        }
        g = Len(Sub2(pos, test));
        if (gBest < g) {
            gBest = g;
            panBest = panT;
        }
    }
    {
        qword r = Add(pos, func_0011E208(panBest, tilt, s));
        func_001AE4A0(&frame, 2);
        return r;
    }
}
