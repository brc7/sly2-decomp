// wr.cpp -- WR, a space warp (an LO): up to eight warp entries (circle waves and noisy bends) over up to four warp
// matrices, applied to the vertices of its warp groups (WRBG); the per-vertex state vectors and weights
// (UpdateWrStateVectors, a Lilly port), the per-frame warp matrices (UpdateWrMatrixes), the 1D noise (UNoise, ONZ) and
// the option setters. VU0 asm; UpdateWrStateVectors and UpdateWrMatrixes are hand-tuned.
// Entry points: WR::Init, UpdateWrMatrixes (per frame), AddWrWarpGroup (glob loading), the Add*/Set* option setters.
// TU 0x22B720..0x22C818 (20 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_wr
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_wr
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/scrap.h"
#include "sly2/classes.h"
#include "sly2/wr.h"
#include "sly2/basic.h"
#include "sly2/port.h"   /* SLY2_FDIV */

// 0x22C6C0 StaticInit_wr and GlobalConstructors_wr: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Types from UpdateWrStateVectors' per-function file (Lilly's wr.h/glob.h/alo.h layouts), with WRE and WR unified
   with UpdateWrMatrixes' and the option setters' views. */
typedef int qwordV __attribute__((mode(TI)));
typedef unsigned int uint;

struct MATRIX4V
{
    float mat[4][4];
} __attribute__((aligned(16)));

struct VU_FLOAT
{
    qwordV q;
};

struct VU_VECTOR
{
    qwordV q;
    // Uninitialised vector.
    VU_VECTOR() {}
    // Wrap a qword.
    VU_VECTOR(qwordV qw) { q = qw; }
    // Wrap a VECTOR's 16 bytes.
    VU_VECTOR(const VECTOR4 &vec) { q = *(const qwordV *)&vec; }
};



struct SO;

struct GLOBV
{
    int oid;
    XF *pxf;
    MATRIX4V *pmatUnk08;
};


struct WARPVTX
{
    float ag[4];
};

/* A float4 as a quadword or as floats, and a 4x4 matrix as quadword rows (UpdateWrMatrixes): basic.h's forms. */
typedef QWROW V;
typedef MATRIX4Q M4;
struct M3 { float f[3][4]; } __attribute__((aligned(16)));
/* ONZ/ONZE, the warp entries (WRE) and the space warp (WR) are sly2/wr.h's. */


extern VECTOR4 D_002A2DA0; // (1, 0, 0, 0)
extern VECTOR4 D_002A2DB0; // (0, 1, 0, 0)
extern VECTOR4 g_normalZ; // (0, 0, 1, 0)
extern VECTOR4 D_002A2DD0; // (0, 0, 0, 0)
extern "C"
{
void *func_002333B4(void *pv, int b, uint cb) __asm__("memset");                       // memset
void CopyAb__FPvT0Ui(void *pvDst, const void *pvSrc, uint cb);         // CopyAb
qwordV ConvertPosBetweenLos(SO *psoFrom, SO *psoTo, qwordV pos);              // ConvertLoPos
float NormalizeAngle__Ff(float rad);                                      // NormalizeAngle
void CalculateSinCos__FfPfT1(float rad, float *pgSin, float *pgCos);           // CalculateSinCos
MATRIX4V InvertMatrix4Rigid(const MATRIX4V *pmat);                          // the inverse of a 4x4 matrix
MATRIX4V __ml__FRC7MATRIX4T0(const MATRIX4V &matL, const MATRIX4V &matR);     // MATRIX4 operator*
void InitLo(WR *warp);
void InitSl(void *pdl, int ibDle);
void AppendSlEntry(void *pdl, void *pv);
void SetLoBusyKind(WR *warp, int busyk);
void UpdateLo(WR *warp);
M3 BuildAxisAngleRotationMatrix(qwordV,float);
}
void UpdateWrStateVectors(WR *warp) __asm__("UpdateWrStateVectors__FP2WR");
float UNoise(int n);
float UNoise(float frequency, float phase, float randomness);
float ApplyBiasCurve(float u, float bias);
float EvaluateNoiseChannel(ONZ *noise);
WRE *GetCurrentWarpEntry(WR *warp, ENSK ensk, WREK kind) __asm__("GetCurrentWarpEntry__FP2WR4ENSK4WREK");
WRE *GetCurrentCircleWarpEntry(WR *warp, ENSK ensk) __asm__("GetCurrentCircleWarpEntry__FP2WR4ENSK");
WRE *GetCurrentBendWarpEntry(WR *warp, ENSK ensk) __asm__("GetCurrentBendWarpEntry__FP2WR4ENSK");
void AddNoiseOctave(ONZ *noise, float amplitude, float frequency, float phase, float randomness);

// Initializes a space warp: base LO init, no free warp matrix and no current entry yet, forces a state-vector update on
// the first frame (tUpdate = -1), empties the warp-group list and marks the object busy (it needs per-frame updates).
// Also known as: InitWr (upstream readable / Sly 1 name; InitWr__FP2WR).
void WR::Init()
{
    WR *p = this;
    sptr f;
    InitLo(p);
    p->freeMatrixIndex = -1;
    p->currentEntryIndex = -1;
    p->lastUpdateTime = -1.0f;
    InitSl(&p->warpGroups, SLY2_OFFSETOF(WRBG, pwrbgNextWr));
    SetLoBusyKind(p, 1);
    f = (sptr)p->ploupd;
    if (IsPointer(f)) ((LOUPD *)f)->grf |= 2;
    else p->unknown40 = f | 2;
}

// Attaches a warp group (WRBG: a set of mesh vertices) to a space warp: appends it to the warp's group list and points
// the group back at the warp. Called while loading glob meshes (glob.cpp). (inferred)
extern "C" void AddWrWarpGroup(WR *warp, WRBG *group)
{
    AppendSlEntry(&warp->warpGroups, group);
    group->pwr = warp;
}

/* UpdateWrStateVectors (comment from its per-function file): */
// UpdateWrStateVectors = Sly 1's UpdateWrStateVectors (Sly 1 0x1F2840): for every warp group (WRBG) of a space warp (WR), the
// weight of each vertex (the WEKI envelope: a linear CLQ of the distance along the warp axis, limited to an LM) and the state
// vector every warp entry (WRE) keeps per vertex (circle: cos/sin of the wave phase at the vertex; bend: the vertex offset
// from the bend centre, w = 1). Struct names/layouts follow Lilly's Sly 2 headers (wr.h, glob.h, alo.h); our names otherwise.
// How it matched (Lilly agent, T1601): the vertex loops use the game's by-value VU_VECTOR / VU_FLOAT classes. Their
// constructor copies are what make the circle loop big enough (>= 97 RTL insns) for loop.c to leave its constants
// (2 pi, 1.0, 1) inside the loop like the game; the dot product's float comes out through an asm mtc1.
// A float in x of a VU0 register
// A vector in a VU0 register (the game passes and returns it by value)
// Quadratic curve g0 + x * (g1 + x * g2)
// Limits
// Singly linked list
// The warp envelope: weight uInner at distance sInner, uOuter at sOuter (linear in between), measured in dmat's space
// The state vector of one vertex for one warp matrix
// A batch of the glob: its vertex positions and, per warp matrix, where the state vectors go
// pos (w = 1) transformed by mat
static inline qwordV TransformPoint(const MATRIX4V &mat, qwordV qwPos)
{
    qwordV qw;
    qw = VuMulMatrix4(*(qwordV *)mat.mat[0], *(qwordV *)mat.mat[1], *(qwordV *)mat.mat[2], *(qwordV *)mat.mat[3], qwPos);
    return qw;
}

// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT vu;
    __asm__("qmtc2.ni %1, %0" : "=j"(vu.q) : "r"(g));
    return vu;
}

// Return qw scaled by g.
static inline qwordV VuScale(qwordV qw, VU_FLOAT g)
{
    qwordV qwResult;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qwResult) : "j"(qw), "j"(g.q));
    return qwResult;
}


// |v| (x, y, z) in x
static inline qwordV VuLength(qwordV v)
{
    qwordV qw;
    qw = VuDot(v, v);
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}




// Set the limit range from g0 and g1 in either order.
static inline void SetLm(LM *plm, const float &g0, const float &g1)
{
    plm->minValue = Min(g0, g1);
    plm->maxValue = Max(g0, g1);
}

// The line through (s0, u0) and (s1, u1)
static inline void FitLinearClq(CLQ *pclq, const float &s0, const float &u0, const float &s1, const float &u1)
{
    pclq->g1 = SLY2_FDIV(u0 - u1, s0 - s1);   /* s0 == s1 for a degenerate segment: the EE div.s result */
    pclq->g0 = u0 - pclq->g1 * s0;
    pclq->g2 = 0.0f;
}

// pos (w = 1) transformed by mat
static inline VU_VECTOR operator*(const MATRIX4V &mat, VU_VECTOR pos)
{
    qwordV qw;
    qw = VuMulMatrix4(VU_VECTOR(*(VECTOR4 *)mat.mat[0]).q, VU_VECTOR(*(VECTOR4 *)mat.mat[1]).q, VU_VECTOR(*(VECTOR4 *)mat.mat[2]).q, VU_VECTOR(*(VECTOR4 *)mat.mat[3]).q, pos.q);
    return VU_VECTOR(qw);
}

// Return vec0 - vec1.
static inline VU_VECTOR operator-(VU_VECTOR vec0, VU_VECTOR vec1)
{
    qwordV qw;
    qw = VuSub(vec0.q, vec1.q);
    return VU_VECTOR(qw);
}

// vec0 . vec1 (x, y, z) in x
static inline VU_FLOAT VuDot(VU_VECTOR vec0, VU_VECTOR vec1)
{
    VU_FLOAT g;
    qwordV qw;
    qw = VuDot(vec0.q, vec1.q);
    g.q = qw;
    return g;
}

// Unit vector along vec.
static inline VU_VECTOR Normalize(VU_VECTOR vec)
{
    qwordV qwLength = VuDot(vec, vec).q, qwNormal;
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwLength) : "0"(qwLength));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwNormal) : "j"(qwLength), "j"(vec.q));
    return VU_VECTOR(qwNormal);
}

// x of a VU_FLOAT as a float, through an asm mtc1 (the compiler's mtc1 schedules the circle case differently)
static inline float FloatFromVu(VU_FLOAT g)
{
    register int n asm("$3"); // the one pin: local-alloc gives this block-local scratch $v0, the game has $v1
    float gResult;
    __asm__("qmfc2.ni %0, %1" : "=r"(n) : "j"(g.q));
    __asm__("mtc1 %1, %0" : "=f"(gResult) : "r"(n));
    return gResult;
}

// Recomputes, for every warp group, each vertex's weight (the warp envelope: linear in the distance along the warp axis,
// clamped) and the per-vertex state vector of every warp entry (circle: cos/sin of the wave phase at the vertex; bend:
// the vertex offset from the bend centre). Runs when the warp's parameters changed. Port of Sly 1's function.
void UpdateWrStateVectors(WR *warp)
{
    MATRIX4V dmat;
    CLQ clq;
    LM lm;
    qwordV posBend;
    qwordV posWorldBend;

    for (WRBG *group = (WRBG *)warp->warpGroups.head; group; group = group->pwrbgNextWr)
    {
        // the envelope's space and the bend centre in it
        if ((int)group->weki.wek == -1)
        {
            func_002333B4(&dmat, 0, sizeof(MATRIX4V));
            dmat.mat[0][0] = dmat.mat[1][1] = dmat.mat[2][2] = dmat.mat[3][3] = 1.0f;
            posBend = *(qwordV *)&D_002A2DD0;
        }
        else
        {
            dmat = *(MATRIX4V *)&group->weki.dmat;
            SetLm(&lm, group->weki.uInner, group->weki.uOuter);
            FitLinearClq(&clq, group->weki.sInner, group->weki.uInner, group->weki.sOuter, group->weki.uOuter);
            switch (group->weki.wek)
            {
            case 4:
                posBend = VuScale(*(qwordV *)&D_002A2DA0, VuFloat((group->weki.sInner + group->weki.sOuter) * 0.5f));
                break;
            case 5:
                posBend = VuScale(*(qwordV *)&D_002A2DB0, VuFloat((group->weki.sInner + group->weki.sOuter) * 0.5f));
                break;
            case 6:
                posBend = VuScale(*(qwordV *)&g_normalZ, VuFloat((group->weki.sInner + group->weki.sOuter) * 0.5f));
                break;
            default:
                posBend = *(qwordV *)&D_002A2DD0;
                break;
            }
            MATRIX4V matInverse;
            matInverse = InvertMatrix4Rigid(&dmat);
            posBend = TransformPoint(matInverse, posBend);
            if (((GLOBV *)group->pglob)->pmatUnk08)
                dmat = __ml__FRC7MATRIX4T0(*(MATRIX4V *)&group->weki.dmat, *((GLOBV *)group->pglob)->pmatUnk08);
        }

        for (WRBGBATCH *pwrbgbatch = (WRBGBATCH *)group->slBatch.head; pwrbgbatch; pwrbgbatch = pwrbgbatch->pwrbgbatchNext)
        {
            VECTOR4 positions[256];
            float au[256];
            CopyAb__FPvT0Ui(positions, (VECTOR4 *)pwrbgbatch->pvPos, pwrbgbatch->cvtx * sizeof(VECTOR4));
            VECTOR4 *pposEnd = positions + pwrbgbatch->cvtx;
            VECTOR4 *ppos = positions;
            float *pu = au;

            // the weight of every vertex
            for (; ppos < pposEnd; ppos++, pu++)
            {
                VECTOR4 pos;
                float s;
                *(qwordV *)&pos = TransformPoint(dmat, *(qwordV *)ppos);
                switch (group->weki.wek)
                {
                case -1:
                    *pu = 1.0f;
                    continue;
                case 0:
                    s = VuToFloat(VuLength(*(qwordV *)&pos));
                    break;
                case 1:
                    pos.z = 0.0f;
                    s = VuToFloat(VuLength(*(qwordV *)&pos));
                    break;
                case 2:
                    pos.y = 0.0f;
                    s = VuToFloat(VuLength(*(qwordV *)&pos));
                    break;
                case 3:
                    pos.x = 0.0f;
                    s = VuToFloat(VuLength(*(qwordV *)&pos));
                    break;
                case 4:
                    s = pos.x;
                    break;
                case 5:
                    s = pos.y;
                    break;
                case 6:
                    s = pos.z;
                    break;
                default:
                    *pu = 1.0f;
                    continue;
                }
                *pu = Clamp(clq.g0 + s * (clq.g1 + s * clq.g2), lm.minValue, lm.maxValue);
            }

            MATRIX4V matWorld = *(MATRIX4V *)&((GLOBV *)group->pglob)->pxf->worldMatrix;
            if (((GLOBV *)group->pglob)->pmatUnk08)
                matWorld = __ml__FRC7MATRIX4T0(matWorld, *((GLOBV *)group->pglob)->pmatUnk08);
            posWorldBend = ConvertPosBetweenLos(group->pso, 0, posBend);

            // the state vectors of every warp entry
            for (int iwre = 0; iwre < warp->entryCount; iwre++)
            {
                WRE *entry = &warp->entries[iwre];
                WARPVTX *pwarpvtx = (WARPVTX *)pwrbgbatch->apvWarp[entry->matrixIndex];
                ppos = positions;
                pu = au;
                for (; ppos < pposEnd; ppos++, pwarpvtx++, pu++)
                {
                    VECTOR4 posWorld;
                    *(VU_VECTOR *)&posWorld = matWorld * VU_VECTOR(*ppos);
                    switch (entry->kind)
                    {
                    case 0:
                    {
                        int is = entry->data.circle.column;
                        float gSin, gCos;
                        VU_FLOAT gDot = VuDot(VU_VECTOR(posWorld), Normalize(VU_VECTOR(entry->vecAxis)));
                        float rScale = entry->data.circle.spatialFrequency * 6.2831855f;
                        float rad = FloatFromVu(gDot) * rScale;
                        CalculateSinCos__FfPfT1(NormalizeAngle__Ff(rad), &gSin, &gCos);
                        pwarpvtx->ag[is] = gCos * *pu;
                        pwarpvtx->ag[is + 1] = gSin * *pu;
                        break;
                    }
                    case 1:
                    {
                        VECTOR4 dpos;
                        *(VU_VECTOR *)&dpos = VU_VECTOR(posWorld) - VU_VECTOR(posWorldBend);
                        pwarpvtx->ag[0] = *pu * dpos.x;
                        pwarpvtx->ag[1] = *pu * dpos.y;
                        pwarpvtx->ag[2] = *pu * dpos.z;
                        pwarpvtx->ag[3] = 1.0f;
                        break;
                    }
                    }
                }
            }
        }
    }
    warp->valuesChanged = 0;
}

// 0x22BE00: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0xe0");

// Integer hash noise: maps an integer lattice point to a pseudo-random value in [0, 1). Used as the sample source for
// the smooth 1D noise below.
float UNoise(int n)
{
    int x = (n << 13) ^ n;
    int y = x * x * 0x3d73 + 0xc0ae5;
    int z = x * y + 0x5208dd0d;
    x = z & 0x7fffffff;
    return (float)x / 2147483648.0f;
}

// Interpolate alternating noise samples with smoothstep.
static inline float Smoothstep(float s)
{
    float twice=s+s;
    register float square __asm__("$f3")=s;
    __asm__("mul.s %0,%0,%0":"+f"(square));
    return square*(3.0f-twice);
}
// a * b (single mul.s in the original operand order).
static inline float Product(float a,float b)
{
    __asm__("mul.s %0,%1,%0":"+f"(b):"f"(a));
    return b;
}

// Smooth 1D noise over time: samples the hash noise at the two lattice points around (frequency * 2 * time + phase) and
// blends them with smoothstep; uRandom (0..1) scales how far alternate samples may stray from 0/1, i.e. the noise's
// randomness. Returns a value in [0, 1].
float UNoise(float frequency, float phase, float u1)
{
    float randomness = u1;
    float t = frequency * 2.0f * g_clock.time + phase;
    int n = (int)t;
    float u0 = UNoise(n);
    u1 = UNoise(n + 1);
    if (n & 1) {
        u0 *= randomness;
        u1 = randomness * (u1 - 1.0f) + 1.0f;
    } else {
        u1 *= randomness;
        u0 = randomness * (u0 - 1.0f) + 1.0f;
    }
    return u0+Product(Smoothstep(t-n),u1-u0);
}

// Schlick-style bias curve: remaps u in [0, 1] so that u = 0.5 maps to uBias, keeping 0 and 1 fixed.
// (Sly 1-derived name: UBias.)
// Also known as: UBias (upstream readable / Sly 1 name).
float ApplyBiasCurve(float u, float bias)
{
    return u / ((((1.0f / bias) - 2.0f) * (1.0f - u)) + 1.0f);
}

// Evaluates a noise channel (ONZ): the amplitude-weighted average of its up to four noise octaves (ONZE), biased with
// the channel's bias and mapped into [minValue, maxValue]. Drives the bend and swivel angles of bend warps.
// (Sly 1-derived name: GFromOnz.)
// Also known as: GFromOnz (upstream readable / Sly 1 name).
float EvaluateNoiseChannel(ONZ *noise)
{
    float gAmpl;
    float gSum = 0.0f;
    gAmpl = gSum;
    int i;
    for (i = 0; i < noise->octaveCount; i++) {
        ONZE *octave = &noise->octaves[i];
        float u = UNoise(octave->frequency, octave->phase, octave->randomness);
        gSum += octave->amplitude * u;
        gAmpl += octave->amplitude;
    }
    float u = ApplyBiasCurve(gSum / gAmpl, noise->bias);
    return noise->minValue + u * (noise->maxValue - noise->minValue);
}

// UpdateWrMatrixes (0x22C028). Circle case: the scalar-home sqc2 stores and one combine carry "memory" clobbers to fix their order (Claude agent).
static inline qwordV ScalarLoad(const float &f){int bits;qwordV q;__asm__("lw %0,%1":"=r"(bits):"m"(f));q = VuFromFloat<qwordV>(bits);return q;}
// Store value to home and return it.
static inline qwordV ScalarHome(V &home,qwordV value){__asm__("sqc2 %1,%0":"=m"(home.q):"j"(value));return value;}
// Return a*fa + b*fb.
static inline qwordV Combine(qwordV fa,qwordV a,qwordV fb,qwordV b){qwordV r;__asm__("vmulax.xyzw ACC,%2,%1x\n\tvmaddx.xyzw %0,%4,%3x":"=j"(r):"j"(fa),"j"(a),"j"(fb),"j"(b));return r;}
// Return a*fa + b*fb (memory variant).
static inline qwordV CombineM(qwordV fa,qwordV a,qwordV fb,qwordV b){qwordV r;__asm__("vmulax.xyzw ACC,%2,%1x\n\tvmaddx.xyzw %0,%4,%3x":"=j"(r):"j"(fa),"j"(a),"j"(fb),"j"(b):"memory");return r;}
// Store value to home (with a memory clobber) and return it.
static inline qwordV ScalarHomeM(V &home,qwordV value){__asm__("sqc2 %1,%0":"=m"(home.q):"j"(value):"memory");return value;}
// Copy the xyz floats of v into destination.
static inline void CopyXYZ(V &destination,const V &v){float x=v.f[0],y=v.f[1],z=v.f[2];destination.f[0]=x;destination.f[1]=y;destination.f[2]=z;}
// Copy the xy floats of v into destination.
static inline void CopyXY(V &destination,const V &v){float x=v.f[0],y=v.f[1];destination.f[0]=x;destination.f[1]=y;}
// Unit vector along v.
static inline qwordV Unit(qwordV v){qwordV len,r;len = VuDot(v, v);__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(len));__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(r):"j"(v),"0"(len));return r;}
// Unit vector along v into a fresh register.
static inline qwordV UnitFresh(qwordV v){qwordV len,r;len = VuDot(v, v);__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(len));__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=&j"(r):"j"(v),"j"(len));return r;}
// Rotate v by the 3x3 matrix m.
static inline qwordV Rotate(const M3 &m,qwordV v){v = VuMulMatrix3(*(const qwordV *)m.f[0], *(const qwordV *)m.f[1], *(const qwordV *)m.f[2], v);return v;}
void UpdateWrMatrixes(WR *p) __asm__("UpdateWrMatrixes__FP2WR");
// Per-frame update of a space warp: refreshes the state vectors if parameters changed, then builds each warp entry's warp
// matrix and UV matrix for the current time (circle: a rotating wave phase; bend: noise-driven bend and swivel
// rotations). Skipped when already updated this frame.
void UpdateWrMatrixes(WR *p){
 UpdateLo(p);
 if(g_clock.time==p->lastUpdateTime)return;
 if(p->valuesChanged)UpdateWrStateVectors(p);
 for(int i=0;i<p->entryCount;i++){
  WRE *entry=&p->entries[i];
  M4 *matrix=&p->warpMatrices[entry->matrixIndex];
  M4 *coordinates=&p->uvMatrices[entry->matrixIndex];
  switch(entry->kind){
  case 0:{
   V position,uv,temporary;
   float sine,cosine;
   float angle=(g_clock.time*entry->data.circle.frequency+entry->data.circle.phase)*6.2831855f;
   CalculateSinCos__FfPfT1(NormalizeAngle__Ff(angle),&sine,&cosine);
   int column=entry->data.circle.column,next=column+1;
   V *pos0=&matrix->r[column],*pos1=&matrix->r[next],*uv0=&coordinates->r[column],*uv1=&coordinates->r[next];
   qwordV qn=VuFromFloat<qwordV>(-sine),qc=ScalarLoad(cosine),qs=VuFromFloat<qwordV>(sine);
 {position.q=Combine(qc,entry->vectorA.q,qs,entry->data.circle.vectorB.q);}
   CopyXYZ(*pos0,position);
 ScalarHomeM(temporary,qc);
 {qwordV t=CombineM(qn,entry->vectorA.q,qc,entry->data.circle.vectorB.q);ScalarHomeM(temporary,qs);position.q=t;ScalarHome(temporary,qc);}
   CopyXYZ(*pos1,position);
 {ScalarHomeM(temporary,qs);uv.q=Combine(qc,entry->data.circle.uvA.q,qs,entry->data.circle.uvB.q);}
   CopyXY(*uv0,uv);
 {qwordV a=entry->data.circle.uvA.q;ScalarHomeM(temporary,qc);uv.q=Combine(qn,a,qc,entry->data.circle.uvB.q);}
   CopyXY(*uv1,uv);
   break;
  }
  case 1:{
   M3 result,first;
   V rotated;
   float bend=EvaluateNoiseChannel(&entry->data.bend.bend);
   float swivel=EvaluateNoiseChannel(&entry->data.bend.swivel);
   first=BuildAxisAngleRotationMatrix(UnitFresh(entry->vectorA.q),swivel);
   rotated.q=Rotate(first,Unit(entry->axis.q));
   result=BuildAxisAngleRotationMatrix(UnitFresh(rotated.q),bend);

   matrix->r[0].f[0]=result.f[0][0]-1.0f;matrix->r[0].f[1]=result.f[0][1];matrix->r[0].f[2]=result.f[0][2];matrix->r[1].f[0]=result.f[1][0];matrix->r[1].f[1]=result.f[1][1]-1.0f;matrix->r[1].f[2]=result.f[1][2];matrix->r[2].f[0]=result.f[2][0];matrix->r[2].f[1]=result.f[2][1];matrix->r[2].f[2]=result.f[2][2]-1.0f;
   break;
  }
  }
 }
 p->lastUpdateTime=g_clock.time;
}

void AddWrCircleWarp(WR *warp) __asm__("AddWrCircleWarp__FP2WR");
// Adds a circle (travelling wave) warp entry and makes it current; two circle entries share one warp matrix (columns 0-1
// and 2-3), so the second one takes the free half left by the first.
void AddWrCircleWarp(WR *warp)
{
    WRE *entry = &warp->entries[warp->entryCount++];
    warp->currentEntryIndex++;
    entry->kind = 0;
    entry->axis.q = *(qwordV *)&D_002A2DA0;
    if (warp->freeMatrixIndex >= 0) {
        entry->matrixIndex = warp->freeMatrixIndex;
        warp->freeMatrixIndex = -1;
        entry->data.circle.column = 2;
    } else {
        entry->matrixIndex = warp->matrixCount++;
        warp->freeMatrixIndex = entry->matrixIndex;
        entry->data.circle.column = 0;
    }
}

void AddWrBendWarp(WR *warp) __asm__("AddWrBendWarp__FP2WR");
// Adds a bend warp entry with default axis and up vectors and neutral (0.5) bias on its bend and swivel noise channels,
// makes it current and gives it its own warp matrix.
void AddWrBendWarp(WR *warp)
{
    WRE *entry = &warp->entries[warp->entryCount++];
    warp->currentEntryIndex++;
    entry->kind = 1;
    entry->axis.q = *(qwordV *)&D_002A2DB0;
    entry->vectorA.q = *(qwordV *)&g_normalZ;
    entry->data.bend.bend.bias = 0.5f;
    entry->data.bend.swivel.bias = 0.5f;
    warp->matrixCount++;
}

// Returns the warp's current entry (0 if it has none) for an option setter; a "set" access (ensk == 1) also marks the
// warp's values changed so state vectors are rebuilt. The kind argument is not checked.
// (Sly 1-derived name: PwreGetWrCur.)
// Also known as: PwreGetWrCur (upstream readable / Sly 1 name).
WRE *GetCurrentWarpEntry(WR *warp, ENSK ensk, WREK kind)
{
    if (warp->entryCount == 0) {
        return 0;
    }
    if (ensk == 1) {
        warp->valuesChanged = ensk;
    }
    return &warp->entries[warp->currentEntryIndex];
}

// Returns the current warp entry for a circle-warp option (see GetCurrentWarpEntry).
// (Sly 1-derived name: PwreGetWrCircle.)
// Also known as: PwreGetWrCircle (upstream readable / Sly 1 name).
WRE *GetCurrentCircleWarpEntry(WR *warp, ENSK ensk)
{
    return GetCurrentWarpEntry(warp, ensk, WREK_Circle);
}

// Returns the current warp entry for a bend-warp option (see GetCurrentWarpEntry). (Sly 1-derived name: PwreGetWrBend.)
// Also known as: PwreGetWrBend (upstream readable / Sly 1 name).
WRE *GetCurrentBendWarpEntry(WR *warp, ENSK ensk)
{
    return GetCurrentWarpEntry(warp, ensk, WREK_Bend);
}

void SetWrWavelength(WR *warp, float sWavelength) __asm__("SetWrWavelength__FP2WRf");
// Option setter: the current circle warp's wavelength, stored as its spatial frequency (0 means no spatial variation).
void SetWrWavelength(WR *warp, float sWavelength)
{
    WRE *entry = GetCurrentCircleWarpEntry(warp, ENSK_Set);
    if (sWavelength == 0.0f)
        entry->data.circle.spatialFrequency = 0.0f;
    else
        entry->data.circle.spatialFrequency = 1.0f / sWavelength;
}

// Option getter: the current circle warp's wavelength (inverse of its spatial frequency, 0 when that is 0).
// Also known as: GetWrWavelength (upstream readable / Sly 1 name; GetWrWavelength__FP2WRPf).
extern "C" float GetWrWavelength(WR *warp)
{
    WRE *entry = GetCurrentCircleWarpEntry(warp, ENSK_Set);
    if (entry->data.circle.spatialFrequency == 0.0f) return 0.0f;
    return 1.0f / entry->data.circle.spatialFrequency;
}

// Adds one noise octave (amplitude, frequency, phase, randomness) to a noise channel; at most four, extras are ignored.
// (Sly 1-derived name: AddOnzOnze.)
// Also known as: AddOnzOnze (upstream readable / Sly 1 name).
void AddNoiseOctave(ONZ *noise, float amplitude, float frequency, float phase, float randomness)
{
    unsigned int c = noise->octaveCount;
    if (c < 4U) {
        ONZE *octave;
        noise->octaveCount = c + 1;
        octave = &noise->octaves[c];
        octave->amplitude = amplitude;
        octave->frequency = frequency;
        octave->phase = phase;
        octave->randomness = randomness;
    }
}

void AddWrBendNoise(WR *warp, float amplitude, float frequency, float phase, float randomness) __asm__("AddWrBendNoise__FP2WRffff");
// Option setter: adds a noise octave to the current bend warp's bend-angle channel.
void AddWrBendNoise(WR *warp, float amplitude, float frequency, float phase, float randomness)
{
    WRE *entry = GetCurrentBendWarpEntry(warp, ENSK_Set);
    AddNoiseOctave(&entry->data.bend.bend, amplitude, frequency, phase, randomness);
}

void AddWrSwivelNoise(WR *warp, float amplitude, float frequency, float phase, float randomness) __asm__("AddWrSwivelNoise__FP2WRffff");
// Option setter: adds a noise octave to the current bend warp's swivel-angle channel.
void AddWrSwivelNoise(WR *warp, float amplitude, float frequency, float phase, float randomness)
{
    WRE *entry = GetCurrentBendWarpEntry(warp, ENSK_Set);
    AddNoiseOctave(&entry->data.bend.swivel, amplitude, frequency, phase, randomness);
}
