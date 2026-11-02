typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct HIT { qword pos; char pad[0x18]; float x28; char pad2[0x14]; float x40; char pad3[0x1C]; };
struct LO2 { char pad[0x218]; qword *x218; };
struct O { char pad[0xEC]; void *xec; };
struct S { O *x0; };
struct Q { int x0, x4, x8; };
typedef struct { float g, gFirst, gSecond; } CLQ;
typedef struct { float gMin, gMax; } LM;
extern "C" {
void STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n);
qword func_0019F3F8(O *p, Q *q, float g);
void IntersectSwBoundingSphere(void *g, qword pos, void *pfn, void *x, int *pi, int *pj, float r);
int func_0014CC58(int z, qword a, qword b, int i, int j, HIT *phit);
void FFilterSoPair(void);
extern char D_003DD490[];
extern float D_002BF944;
extern qword D_002BF950[8];
extern CLQ D_002BF9D0;
extern LM D_002BF9E0;
}
class FRAME { int a[4]; public: FRAME() { STACKSCOPE_ctor(this); } ~FRAME() { STACKSCOPE_ctor_1AE4A0(this, 2); } };
static inline float GMax(float a, float b) { return a > b ? a : b; }
static inline float GFromClq(CLQ *pclq, float g) { return pclq->g + g * (pclq->gFirst + g * pclq->gSecond); }
static inline float GClamp(float g, float lo, float hi) { if (g < lo) g = lo; else if (g > hi) g = hi; return g; }
extern "C" float func_0019F7D0(S *p, Q *q, int n, LO2 **apo);
/** @sly2-annot addr=0x19F7D0
 * Purpose: Score a place: distance from STEPs, line-of-sight/bounding-sphere test against objects
 * Info: proposed name ScoreJtPlace (T2); TU places
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
float func_0019F7D0(S *p, Q *q, int n, LO2 **apo)
{
    float g = 0.0f;
    qword pos = func_0019F3F8(p->x0, q, g);
    if (q->x8) {
        FRAME frame;
        U a, b;
        HIT hit;
        int i0, i1;
        IntersectSwBoundingSphere(D_003DD490, pos, (void *)FFilterSoPair, p->x0->xec, &i0, &i1, D_002BF944);
        a.q = pos;
        b.q = pos;
        a.f[2] += 125.0f;
        b.f[2] -= 175.0f;
        if (!func_0014CC58(0, a.q, b.q, i0, i1, &hit) || hit.x40 == 0.0f || hit.x28 < 0.7f)
            g = GMax(g, 2000.0f);
        else
            pos = hit.pos;
        b.q = pos;
        a.q = pos;
        a.f[2] += 180.0f;
        b.f[2] += 5.0f;
        if (func_0014CC58(0, a.q, b.q, i0, i1, &hit))
            g = GMax(g, 2000.0f);
        for (unsigned k = 0; k < 8; k++) {
            qword v;
            __asm__("vadd.xyzw %0, %1, %2" : "=j"(v) : "j"(pos), "j"(D_002BF950[k]));
            b.q = v;
            a.q = v;
            a.f[2] += 50.0f;
            b.f[2] -= 30.0f;
            if (!func_0014CC58(0, a.q, b.q, i0, i1, &hit)) {
                g = GMax(g, 1500.0f);
                break;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        LO2 *lo = apo[i];
        if (lo == (LO2 *)p->x0) continue;
        qword d, t;
        float dist;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(*lo->x218), "j"(*(qword *)(void *)&pos));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=&j"(t));
        __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
        g = GMax(g, GClamp(GFromClq(&D_002BF9D0, dist), D_002BF9E0.gMin, D_002BF9E0.gMax));
    }
    return g;
}
