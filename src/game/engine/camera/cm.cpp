// cm.cpp -- the camera (CM, the global g_cm at D_002DDF40): construction with its embedded policies, setup, the
// view matrix / frustum / fog setters, the visibility zone, adjustment and cuts, the policy stack, the camera
// filters and the linkonce ~CM. TU 0x14D3D8..0x14FAC8 (55 functions with the linkonce copies at 0x256278.. and
// 0x258290.., address order).
//
// Types: sly2/cm.h's CM and policy classes for the m2c bodies; the hand-written per-function files keep their views
// in namespaces named after each function (as jt.cpp does), and their C++-named definitions are C-linkage
// definitions of their literal symbols there (the namespace would change the mangling), as in wave 9.
// g_cm is defined here: its constructor (CM_ctor) and destructor (func_00258290, ~CM with g++'s in-charge flag 2)
// make the static initializer a constructor-and-destructor one (86 instructions) with a _GLOBAL_$D$ stub
// (func_0014FAA8) next to the _GLOBAL_$I$ one.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_cm
// SLY2_ALIAS: _GLOBAL_$I$*=func_0014FA88
// SLY2_ALIAS: _GLOBAL_$D$*=func_0014FAA8

#include "sly2/basic.h"
#include "sly2/cm.h"
#include "sly2/slotheap.h"

/* g_cm: a view of the CM bytes whose inline constructor and destructor call the C-named CM_ctor and ~CM (as heap.cpp's
   level heap and gc.cpp's g_gc are spelled). Code in this file reads it as a CM through G_CM. */
struct CMG;
extern "C" {
CM *CM_ctor(CM *pcm);
void func_00258290(CMG *pcm, int grfDelete);   /* ~CM (linkonce, below) */
}
struct CMG
{
    char ab[sizeof(CM)];
    CMG() { CM_ctor((CM *)this); }
    ~CMG() { func_00258290(this, 2); }
} __attribute__((aligned(16)));
CMG D_002DDF40;
#define G_CM (*(CM *)&D_002DDF40)

// 0x14F930 StaticInit_cm (g_cm's construction and destruction, then the six slot pools) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The vtable pointer of a CM or a policy (after three words of data). */
#define PVTCM(p) (*(void **)((char *)(p) + 0xC))

extern "C" {
extern void *D_002CA4D0[];  /* CM vtable */
extern void *D_002CD378[];  /* CPTWOSHOT vtable */
CMPART1 *CMPART1_ctor(CMPART1 *p);
CMPART2 *CPTN_ctor(CMPART2 *p);
CPLOOK *CPLOOK_ctor(CPLOOK *p);
CPCAMERA *CPCAMERA_ctor(CPCAMERA *p);
CPHANG *CPHANG_ctor(CPHANG *p);
CPCAMBLEND *CPCAMBLEND_ctor(CPCAMBLEND *p);
CPZOOM *CPZOOM_ctor(CPZOOM *p);
CPTURRET *CPTURRET_ctor(CPTURRET *p);
CPLCY *CPLCY_ctor(CPLCY *p);
qword CalcCmAdjust(CM *pcm);
}
void UpdateCmMat4(CM *pcm);
void RecalcCmFrustrum(CM *pcm);
void SetCmNearClip(CM *pcm, float sNearClip);

/** @sly2-annot addr=0x14D3D8
 * Purpose: CM constructor: BASIC header, vptr D_002CA4D0 (CM vtable), then constructs the 9 embedded camera
 *   policies at +0x3A0..+0x600
 * Info: proposed name CM_ctor (T1); class CM; TU cm
 * Notes: mangled dropped (base does not match final name): __2CM
 * Confidence: high [T1 0.90] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#4-source-files, docs/engine/camera.md#5-key-functions,
 *   docs/engine/main-loop-startup.md#7-source-files
 * @end-sly2-annot */
extern "C" CM *CM_ctor(CM *pcm)
{
    CPTWOSHOT *pcptwoshot = &pcm->part600;
    pcm->pframe.m_p = 0;
    pcm->psidebag.m_p = 0;
    PVTCM(pcm) = D_002CA4D0;
    CMPART1_ctor(&pcm->part3a0);
    CPTN_ctor(&pcm->part3b0);
    CPLOOK_ctor(&pcm->cplook);
    CPCAMERA_ctor(&pcm->part4a8);
    CPHANG_ctor(&pcm->part4c0);
    CPCAMBLEND_ctor(&pcm->cpcamblend);
    CPZOOM_ctor(&pcm->cpzoom);
    CPTURRET_ctor(&pcm->part5f0);
    CPLCY_ctor(pcptwoshot);
    PVTCM(pcptwoshot) = D_002CD378;
    return pcm;
}

/** @sly2-annot addr=0x14D470
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name SetupCm__FP2CM (EXISTING); TU cm
 * Notes: DISPUTED (see disputed.csv) | RENAME proposed -> SetupCm__FP2CM (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.90]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/camera.md#5-key-functions, docs/engine/main-loop-startup.md#1-maincpp-0x1a7518-0x1a7c18,
 *   docs/engine/main-loop-startup.md#2-clock-clockcpp-0x14cfe0-0x14d3d8
 * @end-sly2-annot */
void SetupCm(CM *pcm)
{
}

namespace M_UpdateCmLast__FP2CM { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; struct O *poTarget; int b; };
struct CM {
    char pad[0x20]; float g20; float g24; float g28; float g2C;
    char pad1[0x260 - 0x30]; struct O *po260; char pad2[0x270 - 0x264]; int x270[36];
    int x300; int pad3[2]; int c; struct E ae[8];
    char pad4[0x620 - 0x390]; float g620; float g624;
};
struct G { char pad[0x3E38]; int x3E38; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002DDEE0;
extern int D_002DFBC0;
extern char D_002DFC00[];
extern char D_002E0D00[];
extern struct G D_003DD490;
extern char D_002BD810[];
void ResetCmAdjust(struct CM *p, void *pv);
void AdjustCmCpdefi(struct CM *p, struct O *po, void *pv);
void SetCmFov__FP2CMf(struct CM *p, float g);
float GSmoothA__FffffP4SMPAPf(float g, float h, float i, float dt, float *pa, float *pb);
void RecalcCmFrustrum__FP2CM(struct CM *p);
float GSmooth__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr);
/** @sly2-annot addr=0x14D478
 * Purpose: Per-frame camera update: for the top policy of acpr[] (+0x310), fetch focus CPDEFI via focus->vt50
 *   GetCpdefi into cm+0x270, ResetCmAdjust/AdjustCmCpdefi, then policy vt6 Update(pcpdefi, joy); then fov
 *   smoothing (SetCmFov if nCut else GSmoothA+RecalcCmFrustrum); clears nCut; smooths +0x620
 * Info: proposed name UpdateCmLast (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void UpdateCmLast__FP2CM(struct CM *p)
{
    struct O *po;
    struct O *poPrev;
    struct O *poLast;
    if (p->c > 0) {
    poPrev = 0;
    poLast = 0;
    while ((po = p->ae[0].po) != poPrev) {
        struct O *poT = p->ae[0].poTarget;
        void *pv;
        if (poT) {
            pv = p->x270;
            if (poT != poLast) {
                struct VTE *e = &poT->vt[50];
                e->pfn((char *)poT + e->d, pv);
                if (poPrev || poT != p->po260)
                    ResetCmAdjust(p, pv);
                AdjustCmCpdefi(p, poT, pv);
                p->po260 = poT;
                poLast = poT;
            }
        } else {
            p->po260 = 0;
            pv = 0;
            poLast = 0;
        }
        {
            struct VTE *e = &po->vt[6];
            e->pfn((char *)po + e->d, pv, (D_002DFBC0 & 1) ? D_002DFC00 : D_002E0D00);
        }
        poPrev = po;
        if (p->c <= 0)
            break;
    }
    }
    if (p->x300) {
        SetCmFov__FP2CMf(p, p->g24);
    } else if (p->g24 != p->g20) {
        p->g20 = GSmoothA__FffffP4SMPAPf(p->g20, p->g28, p->g24, D_002DDED0.dt, &p->g2C, &p->g28);
        RecalcCmFrustrum__FP2CM(p);
    }
    p->x300 = 0;
    if (D_003DD490.x3E38)
        p->g620 = GSmooth__FfffP3SMPPf(p->g620, p->g624, D_002DDEE0, D_002BD810, 0);
}
} } // namespace M_UpdateCmLast__FP2CM

namespace M_func_0014D650 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { char pad[0xC]; struct VTE *vt; };
struct E { struct O *po; int pad[3]; };
struct P { char pad[0x30C]; int c; int pad2; struct E ae[1]; };
/** @sly2-annot addr=0x14D650
 * Purpose: Calls vt7 (unknown 'reset/close' slot) on every policy in acpr[], then rUnk620=0, rUnk624=10.0
 * Info: proposed name ResetCmPolicies (T2); TU cm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70+verdicts_A1
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014D650(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct O *o = p->ae[i].po;
        struct VTE *e = &o->vt[7];
        ((void (*)(void *))e->pfn)((char *)o + e->d);
    }
    *(int *)((char *)p + 0x620) = 0;
    *(float *)((char *)p + 0x624) = 10.0f;
}
} } // namespace M_func_0014D650

namespace M_InitCm__FP2CM {
// InitCm: near/far and LM init via inline helpers (parameter pseudos give the original FP register choice).
typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned char r, g, b, a; RGBA(unsigned int n) { *(unsigned int *)this = n; } };
struct V3 { float x, y, z; };
struct LM { float g0; int n4; float g8; float gC; };
struct CM {
    char pad[0x10]; int x10; int x14; float g18; float g1C; float g20; float g24; int pad28;
    float g2C; int x30; float g34; float g38; float g3C; int pad40[3]; float g4C; float g50; int pad54[3];
    float mat[16]; char padA0[0x240 - 0xA0]; V3 v240; RGBA rgba;
    char pad250[0x30C - 0x250]; int c30C; char pad310[0x3A0 - 0x310]; int x3A0[4]; int x3B0[40]; int x450[22];
    int x4A8[6]; int x4C0[16]; int x500[8]; int x520[52]; int x5F0[10]; float g618; int x61C;
};
extern "C" {
extern float D_002BD81C, D_002BD820, D_002BD824;
extern qword D_002BD830;
extern qword D_002BD840;
void *func_002333B4(void *pv, int b, int cb);
void RecalcCmFrustrum__FP2CM(CM *p);
void func_0014E1A8(CM *p, qword pos, qword tgt, float da, float db);
void CutCm__FP2CM(CM *p);
void InitCmpart1(void *p);
void func_002219C8(void *p);
void InitCplcy(void *p);
void SetCmPolicy(CM *p, int prio, void *po, int a, int b);
}
static inline void SetNF(CM *p, float n, float f) { p->g4C = n; p->g50 = f; }
static inline void InitLm(LM *plm, float g0, float g8, float gC) { plm->g0 = g0; plm->gC = gC; plm->g8 = g8; plm->n4 = 0; }
/** @sly2-annot addr=0x14D6D8
 * Purpose: Camera init: rgba fog 0x80303030, aspect 1.2987, near/far 100/100000, identity mat,
 *   RecalcCmFrustrum, SetCmLookAt, CutCm, Init on each embedded policy, push default policy (+0x3A0) with
 *   SetCmPolicy(prio 0), tJolt=-10
 * Info: proposed name InitCm (T1); TU cm
 * Notes: names_v2 v1 proposed AdjustCmCpdefi__FP2CMP2SOfP6CPDEFI (0.638)
 * Confidence: high [T1 0.90] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void InitCm__FP2CM(CM *p)
{
    static RGBA s_rgba(0x80303030);
    p->v240.x = D_002BD81C;
    p->v240.y = D_002BD820;
    p->v240.z = D_002BD824;
    p->g3C = 1.29870152f;
    SetNF(p, 100.0f, 100000.0f);
    p->g1C = 1.0f;
    p->g18 = 1.0f;
    p->g24 = 1.0f;
    p->g20 = 1.0f;
    p->rgba = s_rgba;
    InitLm((LM *)&p->g2C, 0.5f, 0.1f, 10.0f);
    float *pm = p->mat;
    func_002333B4(pm, 0, 0x40);
    pm[15] = 1.0f;
    pm[10] = 1.0f;
    pm[5] = 1.0f;
    pm[0] = 1.0f;
    RecalcCmFrustrum__FP2CM(p);
    func_0014E1A8(p, D_002BD830, D_002BD840, 0.0f, 0.0f);
    CutCm__FP2CM(p);
    p->c30C = 0;
    InitCmpart1(p->x3A0);
    func_002219C8(p->x3B0);
    InitCplcy(p->x450);
    InitCplcy(p->x4A8);
    InitCplcy(p->x4C0);
    InitCplcy(p->x500);
    InitCplcy(p->x520);
    InitCplcy(p->x5F0);
    p->x10 = 0;
    p->x14 = 0xFF;
    SetCmPolicy(p, 0, p->x3A0, 0, 0);
    p->x61C = -1;
    p->g618 = -10.0f;
}
} // namespace M_InitCm__FP2CM

namespace M_CutCm__FP2CM { extern "C" {
/** @sly2-annot addr=0x14D8C0
 * Purpose: nCut(+0x300) = max(nCut,1): request a camera cut (no smoothing) this frame
 * Info: proposed name CutCm (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void CutCm__FP2CM(char *p)
{
    int n = *(int *)(p + 0x300);
    int m = 1;
    if (m < n) m = n;
    *(int *)(p + 0x300) = m;
}
} } // namespace M_CutCm__FP2CM

namespace M_SetCmCut { extern "C" {
struct CMX { char pad[0x300]; int n; float a; float b; };
/** @sly2-annot addr=0x14D8D8
 * Purpose: Cut with explicit pan/tilt: radCutPan(+0x304)/radCutTilt(+0x308) = args, nCut = max(nCut,2)
 * Info: proposed name SetCmCut (T1); TU cm
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmCut(struct CMX *p, float a, float b)
{
    int m = 2;
    p->b = b;
    p->a = a;
    if (m < p->n) m = p->n;
    p->n = m;
}
} } // namespace M_SetCmCut

namespace M_func_0014D8F8 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct { float m[4][4]; } M4F;
typedef struct { qword r[4]; } M4;
void *func_002333B4(void *p, int c, int cb);
void InvertMatrix4Rigid(M4F *pmOut, M4F *pm);
void __ml__FRC7MATRIX4T0(M4 *pmOut, M4F *pa, M4F *pb);
/** @sly2-annot addr=0x14D8F8
 * Purpose: matViewProj = proj * inverse(eye matrix with game->camera axis swap: x=-y, y=-z, z=x)
 * Info: proposed name CombineEyeLookAtProj (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014D8F8(M4F *pm, M4F *pproj, M4 *pmOut)
{
    M4F m;
    M4 r;
    int i;
    func_002333B4(&m, 0, sizeof(m));
    m.m[3][3] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[0][0] = 1.0f;
    for (i = 0; i < 3; i++) {
        m.m[0][i] = -pm->m[1][i];
        m.m[1][i] = -pm->m[2][i];
        m.m[2][i] = pm->m[0][i];
        m.m[3][i] = pm->m[3][i];
    }
    {
        M4F t;
        M4 *pr = &r;
        InvertMatrix4Rigid(&t, &m);
        __ml__FRC7MATRIX4T0(pr, pproj, &t);
    }
    *pmOut = r;
}
} } // namespace M_func_0014D8F8

namespace M_BuildFrustrum {
// BuildFrustrum(pmat, gx, gy, anormal): 4 side-plane normals. Hard-reg VF vars in Normalize1 pin the first block allocation.
typedef int qword __attribute__((mode(TI)));
struct MATRIX3 { qword r[3]; };
extern "C" void BuildFrustrum(MATRIX3 *pmat, float gx, float gy, qword *anormal);
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };

static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}
static inline qword Normalize(qword v)
{
    qword t, one, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=&j"(t), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(r) : "j"(t), "j"(v));
    return r;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}

static inline qword Normalize1(qword v)
{
    register qword t __asm__("vf2");
    register qword r __asm__("vf2");
    qword one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=&j"(t), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(r) : "j"(t), "j"(v));
    return r;
}

/** @sly2-annot addr=0x14DA08
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/camera.md#2-the-policy-stack, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void BuildFrustrum(MATRIX3 *pmat, float gx, float gy, qword *anormal)
{
    anormal[0] = Cross(pmat->r[2], Normalize1(AddScaled(pmat->r[0], pmat->r[1], VU_FLOAT(gx))));
    anormal[1] = Cross(Normalize(AddScaled(pmat->r[0], pmat->r[1], VU_FLOAT(-gx))), pmat->r[2]);
    anormal[2] = Cross(Normalize(AddScaled(pmat->r[0], pmat->r[2], VU_FLOAT(gy))), pmat->r[1]);
    anormal[3] = Cross(pmat->r[1], Normalize(AddScaled(pmat->r[0], pmat->r[2], VU_FLOAT(-gy))));
}
} // namespace M_BuildFrustrum

namespace M_RecalcCmFrustrum__FP2CM {
#include "sly2/cm.h"
// sqrt.s as volatile inline asm (as in other matched cm/util code): it orders the stores like the original.
extern "C" float tanf(float);
static inline float sqrtf(float x){float r; __asm__ __volatile__("sqrt.s %0,%1":"=&f"(r):"f"(x)); return r;}
void BuildProj(void *p, float a, float b, float c, float d, float e, float f) __asm__("BuildSimpleProjectionMatrix__FffffffP7MATRIX4");
void SetCmLookAt(CM *p, float g, void *pv) __asm__("BuildCmFgfn");
void RecalcCm(CM *p) __asm__("UpdateCmMat4__FP2CM");
static inline float GMax(float a, float b) { return a > b ? a : b; }
/** @sly2-annot addr=0x14DB70
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/camera.md#2-the-policy-stack, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void RecalcCmFrustrum__FP2CM(CM *pcm)
{
    float t = tanf(pcm->fov * 0.5f);
    float r = 1.0f / pcm->fov;
    float c = pcm->rAspect;
    pcm->gTanHalfY = t;
    pcm->gTanHalfX = t * c;
    pcm->rCullScale = GMax(pcm->rFovCullRef * r, 1.0f);
    pcm->sNearCorner = t * sqrtf(c * c + 1.0f) * pcm->sNearClip + 1.0f;
    BuildProj(&pcm->matProj, 512.0f / (pcm->gTanHalfX * 4096.0f), 224.0f / (pcm->gTanHalfY * 4096.0f), 0.0f, 0.0f, pcm->sNearClip, pcm->sFarClip);
    SetCmLookAt(pcm, 1.0f, pcm->agRecipFit);
    RecalcCm(pcm);
}
} // namespace M_RecalcCmFrustrum__FP2CM

namespace M_UpdateCmMat4__FP2CM {
// UpdateCmMat4: ported from Lilly's personal Sly 2 decomp (src/P2/cm.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DE0 (D_002A2DE0), D_002BD850 (D_002BD850), D_002BD854 (SW_CmJolt), D_002DDED0 (g_clock), D_002BBB60 (g_lmZeroOne), D_002BD860 (s_clqUToUJolt), D_002BD870 (s_dposJolt), D_002BD880 (s_rotJolt), D_002BD8A0 (s_vecOffset), D_002BD890 (s_vecRange)
// 0x0014DC58 UpdateCmMat4__FP2CM  (T1386; Sly 1 never matched it)
// The camera's placement CM::mat (3 rotation rows, row 3 = the position; SetCmMat4 / SetCmPanTilt / the policies set it) or its projection changed:
//  1. the world -> clip matrix matViewProj = CombineEyeLookAtProj(eye, matProj), where eye is CM::mat itself, or, for 0.5 s after a camera jolt
//     (tJolt, set by 0x14F750), CM::mat times a small shake matrix (rotation about x of 0.05 rad * jolt, a position kick) and a projection whose scale
//     and offset the jolt changes (the kick, scale and offset data are zero in the ELF: only the rotation shakes). jolt = sin(50 * dt) * (1 - dt / 0.5)^2;
//  2. matViewProjInv = FInvertMatrix4(matViewProj);
//  3. the four frustum side planes (BuildFrustrum) of the UNshaken CM::mat: aplaneFrustum for the real fov and aplaneFrustumWide for tan(x) * 7.93 and
//     tan(y) * 18.1, each also stored transposed (x, y, z of the four normals = agFrustumXyz / agFrustumWideXyz, the VU0 cull microprogram's vf10-vf12);
//  4. UpdateCmVisZone picks the vis zone the camera is in.
// How it matches: the jolt's float is moved into a GPR once (VuFloatOfFloat is CSEd) and spilled across the calls; the shake matrix is built in
// matEye (rotation rows from LoadRotateMatrix, the position row through VECTOR_opAssign, w column last) and multiplied in place (a temp + copy); the
// frustum copy loops index the four planes with separate pointers; the wide copy continues at agFrustumXyz[4 * i] (agFrustumWideXyz directly follows
// agFrustumXyz) with the first loop's counter, so loop.c, not gcse, makes its start address (a plain pcm->agFrustumWideXyz is hoisted into an s register
// by PRE); the pointers of the wide planes are the PRE-hoisted copies the game has before the first BuildFrustrum call.
#include "sly2/basic.h"
#include "sly2/aseg.h"
#include "sly2/cm.h"
MATRIX3 LoadRotateMatrix(qword vec) __asm__("func_0011B570");
MATRIX4 operator*(const MATRIX4 &matL, const MATRIX4 &matR) __asm__("__ml__FRC7MATRIX4T0");
bool FInvertMatrix4(MATRIX4 *pmatSrc, MATRIX4 *pmatDst) __asm__("func_0011C2F0");
void BuildSimpleProjectionMatrix(float rx, float ry, float dxOffset, float dyOffset, float sFar, float sNear, MATRIX4 *pmat) __asm__("BuildSimpleProjectionMatrix__FffffffP7MATRIX4");
extern "C" { extern LM D_002BBB60; }
extern "C" { extern CLOCK D_002DDED0; }
struct CAMEVENT
{
      uint32_t   _pad_376    [ 3 ] ;
};
struct CAMPOSE
{
      VECTOR4 pos;
      VECTOR4 eul;
      float radFov;
};
struct CAMMOVE
{
      CAMPOSE *pposeFrom;
      CAMPOSE *pposeTo;
      float dt;
      int fCutOnLeave;
      int iCurve;
      CAMEVENT *pevent;
      int nUnk18;
};
extern "C" void CombineEyeLookAtProj(MATRIX4 *pmatEye, MATRIX4 *pmatProj, MATRIX4 *pmatOut) __asm__("func_0014D8F8");
extern "C" void BuildFrustrum(float gTanX, float gTanY, MATRIX4 *pmat, qword *paplane);
void UpdateCmVisZone(CM *pcm) __asm__("UpdateCmVisZone__FP2CM");
struct VU_FLOAT
{
    qword qw;
};
static inline qword QwAddScaled(qword qwBase, qword qwD, VU_FLOAT g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwD), "j"(g.qw));
    return qw;
}
extern "C" { extern float D_002BD850; }
extern "C" { extern float D_002BD854; }
extern "C" { extern CLQ D_002BD860; }
extern "C" { extern qword D_002BD870; }
extern "C" { extern qword D_002BD880; }
extern "C" { extern qword D_002BD890; }
extern "C" { extern qword D_002BD8A0; }
extern "C" { extern qword D_002A2DE0; }
extern "C" { extern float *VECTOR_opAssign(float *pg, qword qw); }
static inline qword QwMulX(qword qwRow, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwRow), "j"(gScale.qw));
    return qw;
}
static inline VU_FLOAT VuFloatOfFloat(float g)
{
    VU_FLOAT v;
    __asm__("mfc1 %0, %1" : "=r"(v.qw) : "f"(g));
    return v;
}
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = pclq->g0 + x * (pclq->g1 + x * pclq->g2);
    float gMin = plm->gMin;
    float gMax = plm->gMax;
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
void UpdateCmMat4(CM *pcm) __asm__("UpdateCmMat4__FP2CM");
/** @sly2-annot addr=0x14DC58
 * Purpose: Recomputes matViewProj (+jolt shake for 0.5 s), its inverse, frustum side planes (normal and wide)
 *   and transposed VU0 cull rows, then UpdateCmVisZone
 * Info: proposed name UpdateCmMat4 (T1); TU cm
 * Confidence: high [T1 0.90] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#2-the-policy-stack, docs/engine/camera.md#5-key-functions,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void UpdateCmMat4__FP2CM(CM *pcm)
{
    MATRIX4 matEye;
    float dtJolt = D_002DDED0.t - pcm->tJolt;
    if (dtJolt > D_002BD850)
    {
        CombineEyeLookAtProj(&pcm->mat, &pcm->matProj, &pcm->matViewProj);
    }
    else
    {
        float gEnv = GEvaluateClqLm(&D_002BD860, &D_002BBB60, dtJolt / D_002BD850);
        float gJolt = sinf(dtJolt * D_002BD854) * gEnv;
        {
            MATRIX3 matRot = LoadRotateMatrix(QwMulX(D_002BD880, VuFloatOfFloat(gJolt)));
            VECTOR_opAssign(matEye.mat[3], QwMulX(D_002BD870, VuFloatOfFloat(gJolt)));
            *(MATRIX3 *)&matEye = matRot;
        }
        matEye.mat[3][3] = 1.0f;
        matEye.mat[2][3] = 0.0f;
        matEye.mat[1][3] = 0.0f;
        matEye.mat[0][3] = 0.0f;
        matEye = pcm->mat * matEye;
        VECTOR4 vecRange;
        VECTOR4 vecOffset;
        MATRIX4 matProj;
        *(qword *)&vecRange = QwAddScaled(D_002A2DE0, D_002BD890, VuFloatOfFloat(gJolt));
        *(qword *)&vecOffset = QwMulX(D_002BD8A0, VuFloatOfFloat(gJolt));
        BuildSimpleProjectionMatrix(vecRange.x * 512.0f / (pcm->gTanHalfX * 4096.0f), vecRange.y * 224.0f / (pcm->gTanHalfY * 4096.0f),
                                    vecOffset.x, vecOffset.y, pcm->sNearClip, pcm->sFarClip, &matProj);
        CombineEyeLookAtProj(&matEye, &matProj, &pcm->matViewProj);
    }
    FInvertMatrix4(&pcm->matViewProj, &pcm->matViewProjInv);
    qword *paplane = pcm->aplaneFrustum;
    qword *paplaneWide = pcm->aplaneFrustumWide;
    float *pgWide1 = (float *)&paplaneWide[1];
    float *pgWide2 = (float *)&paplaneWide[2];
    BuildFrustrum(pcm->gTanHalfX, pcm->gTanHalfY, &pcm->mat, paplane);
    float *pgWide3 = (float *)&paplaneWide[3];
    float *pg0 = (float *)&paplane[0];
    float *pg3 = pg0 + 12;
    float *pgDst = pcm->agFrustumXyz;
    float *pg2 = pg0 + 8;
    float *pg1 = pg0 + 4;
    int i;
    for (i = 0; i < 3; i++)
    {
        pgDst[0] = *pg0++;
        pgDst[1] = *pg1++;
        pgDst[2] = *pg2++;
        pgDst[3] = *pg3++;
        pgDst += 4;
    }
    BuildFrustrum(pcm->gTanHalfX * 7.93f, pcm->gTanHalfY * 18.1f, &pcm->mat, paplaneWide);
    float *pgDstWide = &pcm->agFrustumXyz[4 * i];
    float *pgW3 = pgWide3;
    float *pgW2 = pgWide2;
    float *pgW1 = pgWide1;
    float *pgW0 = (float *)&paplaneWide[0];
    i = 2;
    do
    {
        pgDstWide[0] = *pgW0++;
        pgDstWide[1] = *pgW1++;
        pgDstWide[2] = *pgW2++;
        pgDstWide[3] = *pgW3++;
        pgDstWide += 4;
    } while (--i >= 0);
    UpdateCmVisZone(pcm);
}
} // namespace M_UpdateCmMat4__FP2CM

namespace M_func_0014E000 { extern "C" {
struct T { char pad[0xE4]; int nE4; char pad2[0x110 - 0xE8]; int n110; float g114; char pad3[0x120 - 0x118]; };
/** @sly2-annot addr=0x14E000
 * Purpose: qsort comparator for vis-zone candidates: by +0x110 desc, +0xE4 desc, then +0x114 (distance) asc
 * Info: proposed name ICompareCmVisZone (T2); TU cm
 * Notes: names_v2 B proposed DrawCm__FP2CM (0.856)
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int func_0014E000(struct T **ppa, struct T **ppb)
{
    struct T *a = *ppa;
    struct T *b = *ppb;
    int d = b->n110 - a->n110;
    if (d) return d;
    d = b->nE4 - a->nE4;
    if (d) return d;
    if (a->g114 < b->g114) return -1;
    if (b->g114 < a->g114) return 1;
    return a - b;
}
} } // namespace M_func_0014E000

namespace M_UpdateCmVisZone__FP2CM { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x110]; int k; };
struct CM { char pad[0x90]; qword pos; };
extern char D_003DD490[];
int CploFindSwObjectsByClass(void *g, int a, int b, int c, int n, struct O **ap);
void func_002273F0(struct O *po, qword pos);
void qsort(void *base, int n, int cb, int (*cmp)(const void *, const void *));
int func_0014E000(const void *a, const void *b);
void SetCmVisZone(struct CM *p, struct O *po);
/** @sly2-annot addr=0x14E078
 * Purpose: Gathers up to 32 zone objects (0x15E9C8 query), scores each against cm pos (0x2273F0), sorts, and
 *   selects the best via SetCmVisZone if its kind==1
 * Info: proposed name UpdateCmVisZone (T1); TU cm
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void UpdateCmVisZone__FP2CM(struct CM *p)
{
    struct O *apo[32];
    qword pos = p->pos;
    int n = CploFindSwObjectsByClass(D_003DD490, 0x105, 5, 0, 0x20, apo);
    int i;
    for (i = 0; i < n; i++)
        func_002273F0(apo[i], pos);
    qsort(apo, n, 4, func_0014E000);
    if (n > 0 && apo[0]->k == 1)
        SetCmVisZone(p, apo[0]);
}
} } // namespace M_UpdateCmVisZone__FP2CM

/** @sly2-annot addr=0x14E148
 * Purpose: If changed: pzonVis(+0x10)=pzon, grfvis(+0x14)=pzon->+0xE0
 * Info: proposed name SetCmVisZone (T1); TU cm
 * Notes: names_v2 v1 proposed SetCmUFogMax__FP2CMf (0.001)
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void SetCmVisZone(CM *pcm, VISZONE *pzon)
{
    if (pzon != pcm->pzonVis) {
        pcm->pzonVis = pzon;
        pcm->grfvis = *(uint *)((char *)pzon + 0xE0);   /* the zone's visibility mask (VISZONE is opaque here) */
    }
}

/** @sly2-annot addr=0x14E168
 * Purpose: Copies a 4x4 placement into CM::mat (+0x60..0x9F) then UpdateCmMat4
 * Info: proposed name SetCmMat4 (T1); TU cm
 * Notes: names_v2 B proposed ResetCmAdjust__FP2CMP2SOP6CPDEFI (0.856)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void SetCmMat4(CM *pcm, qword *pmat)
{
    ((qword *)&pcm->mat)[0] = pmat[0];
    ((qword *)&pcm->mat)[1] = pmat[1];
    ((qword *)&pcm->mat)[2] = pmat[2];
    ((qword *)&pcm->mat)[3] = pmat[3];
    UpdateCmMat4(pcm);
}

namespace M_func_0014E1A8 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } V4;
struct M3 { V4 r[3]; } __attribute__((aligned(16)));
typedef struct { M3 m; V4 pos; } M4;
struct CM { char pad[0x60]; M4 mat; };
void GetVectorSphere(qword v, float *a, float *b, int c);
M3 LoadRotateMatrixPanTilt(float a, float b);
void VECTOR_opAssign(V4 *p, qword v);
void UpdateCmMat4__FP2CM(struct CM *p);
static __inline__ void LoadM4(M4 *pm, M3 m, qword pos)
{
    VECTOR_opAssign(&pm->pos, pos);
    pm->m = m;
    pm->m.r[1].w = 0;
    pm->m.r[2].w = 0;
    pm->m.r[0].w = 0;
    pm->pos.w = 1.0f;
}
/** @sly2-annot addr=0x14E1A8
 * Purpose: Place camera at pos looking at target, with extra pan/tilt offsets: pan/tilt from (tgt-pos)
 *   (0x11EFB0), rotation 0x11B720, pos row, UpdateCmMat4
 * Info: proposed name SetCmLookAt (T1); TU cm
 * Notes: names_v2 B proposed GetCmCpdefi__FP2CMP2SOfP6CPDEFI (0.985)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E1A8(struct CM *p, qword pos, qword tgt, float da, float db)
{
    qword d;
    float a, b;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(tgt), "j"(pos));
    GetVectorSphere(d, &a, &b, 0);
    LoadM4(&p->mat, LoadRotateMatrixPanTilt(a + da, b + db), pos);
    UpdateCmMat4__FP2CM(p);
}
} } // namespace M_func_0014E1A8

namespace M_BuildCmFgfn { extern "C" {
extern float D_002A4FF8;
void FitRecipFunction__FffffPfT4(float a, float b, float c, float d, float *pa, float *pb);
struct P { char pad[0x240]; float g240, g244, g248; };
/** @sly2-annot addr=0x14E270
 * Purpose: Builds the GS fog function (FGFN) from sNearFog/sFarFog/uFogMax (+0x240..0x248): linear map to
 *   0..4080, +8417280 bias
 * Info: proposed name BuildCmFgfn (T1); TU cm
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void BuildCmFgfn(struct P *p, float *af, float f)
{
    if (f == D_002A4FF8) f = 1.0f;
    FitRecipFunction__FffffPfT4(1.0f, 4080.0f, p->g244 / p->g240, (1.0f - p->g248 * f) * 4080.0f, &af[0], &af[1]);
    af[0] += 8417280.0f;
    af[2] = p->g240;
    af[3] = af[0] + 8192.0f;
}
} } // namespace M_BuildCmFgfn

namespace M_SetCmPolicy { extern "C" {
/* Insert an entry into a priority-sorted array of 8 (Claude agent). memmove goes through an inline helper: its
   parameters get their own pseudos, giving the target's (p + i*16) base copied into a1 then +0x320 / +0x310. */
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; int a; int b; };
struct P { char pad[0x30C]; int c; struct E ae[8]; char pad2[0x390 - 0x390]; float t; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void *memmove(void *dst, void *src, int cb);
static inline void Move(void *pvDst, void *pvSrc, int cb) { memmove(pvDst, pvSrc, cb); }
/** @sly2-annot addr=0x14E328
 * Purpose: Insert {cpp,pcplcy,psoFocus,pv} into priority-sorted acpr[8] (+0x30C count); call pcplcy vt4
 *   Set(pv); if new top: prev vt3 Deactivate, tModeStart=clock.t, vt2 Activate
 * Info: proposed name SetCmPolicy (T1); TU cm
 * Notes: runtime: CM policy stack acpr[8] at +0x310 sorted by priority confirmed
 * Confidence: high [T1 0.95] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmPolicy(struct P *p, int prio, struct O *po, int a, int b)
{
    int i;
    if (p->c < sizeof(p->ae) / sizeof(p->ae[0])) {
        for (i = 0; i < p->c; i++) {
            if (p->ae[i].prio < prio)
                break;
        }
        Move(&p->ae[i + 1], &p->ae[i], (p->c - i) * sizeof(struct E));
        p->ae[i].prio = prio;
        p->ae[i].po = po;
        p->ae[i].a = a;
        p->ae[i].b = b;
        p->c++;
        {
            struct VTE *e = &po->vt[4];
            ((void (*)(void *, int))e->pfn)((char *)po + e->d, b);
        }
        if (i == 0) {
            if (p->c >= 2) {
                struct O *o = p->ae[1].po;
                struct VTE *e = &o->vt[3];
                ((void (*)(void *, int, int))e->pfn)((char *)o + e->d, p->ae[1].a, p->ae[1].b);
            }
            p->t = D_002DDED0.t;
            {
                struct VTE *e = &po->vt[2];
                ((void (*)(void *, int, int))e->pfn)((char *)po + e->d, a, b);
            }
        }
    }
}
} } // namespace M_SetCmPolicy

namespace M_RevokeCmPolicy { extern "C" {
/* Matched by Claude agent (2026-10-17): compare against the parameters directly (no local E copy) and `for (i = 0, pe = p->ae; ...)`. */
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; int a; int b; };
struct P { char pad[0x30C]; int c; struct E ae[8]; char pad2[0x390 - 0x390]; float t; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
/** @sly2-annot addr=0x14E4C8
 * Purpose: Remove acpr[] entries matching grf mask (1 cpp, 2 pcplcy, 4 psoFocus, 8 pv), vt5 Revoke each; if
 *   top removed: Deactivate it and Activate new top
 * Info: proposed name RevokeCmPolicy (T1); TU cm
 * Confidence: high [T1 0.90] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void RevokeCmPolicy(struct P *p, int grf, int prio, struct O *po, int a, int b)
{
    struct E *peOut;
    struct E *pe;
    int i;
    struct O *poTop;
    int aTop;
    int bTop;
    poTop = 0;
    aTop = 0;
    bTop = 0;
    peOut = p->ae;
    for (i = 0, pe = p->ae; i < p->c; i++, pe++) {
        if ((((grf ^ 1) & 1) || prio == pe->prio) &&
            (!(grf & 2) || po == pe->po) &&
            (!(grf & 4) || a == pe->a) &&
            (!(grf & 8) || b == pe->b)) {
            if (i == 0) {
                poTop = pe->po;
                aTop = pe->a;
                bTop = pe->b;
            }
            {
                struct O *o = pe->po;
                struct VTE *v = &o->vt[5];
                ((void (*)(void *, int))v->pfn)((char *)o + v->d, pe->b);
            }
        } else {
            *peOut++ = *pe;
        }
    }
    p->c = peOut - p->ae;
    if (poTop) {
        struct VTE *v = &poTop->vt[3];
        ((void (*)(void *, int, int))v->pfn)((char *)poTop + v->d, aTop, bTop);
    if (p->c > 0) {
        struct O *o;
        struct VTE *v;
        p->t = D_002DDED0.t;
        o = p->ae[0].po;
        v = &o->vt[2];
        ((void (*)(void *, int, int))v->pfn)((char *)o + v->d, p->ae[0].a, p->ae[0].b);
    }
    }
}
} } // namespace M_RevokeCmPolicy

namespace M_CalcCmAdjust { extern "C" {
void SetVectorCylind(void *a, void *b, float x, float y, float z);
/** @sly2-annot addr=0x14E698
 * Purpose: dposAdjust = cylindrical vector (radHome+PI, sAdjust) from CPDEFI +0x30/+0x34 via 0x11E1A0
 * Info: proposed name CalcCmAdjust (T1); TU cm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void CalcCmAdjust(void *a, char *b)
{
    SetVectorCylind(a, b, *(float *)(b + 0x30) + 3.14159265f, *(float *)(b + 0x34), 0.0f);
}
} } // namespace M_CalcCmAdjust

namespace M_func_0014E6D0 { extern "C" {
// MATCH (Claude agent): hard-register VF locals; clock pointer pinned to v0 and the second dt read via a v0-pinned int.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct CLOCK { float t; union { float g; int n; } dt; };
extern struct CLOCK D_002DDED0;
struct P { char pad[0x300]; int f300; };
/** @sly2-annot addr=0x14E6D0
 * Purpose: Critically-damped spring of *ppos/ *pv toward target with (rSpring,rDamping) and clock dt; snaps
 *   and zeroes v when nCut
 * Info: proposed name SpringCm (T1); TU cm
 * Notes: names_v2 v1 proposed RemoveCmFadeObject__FP2CMP3ALO (0.167)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E6D0(struct P *p, qword *ppos, float *pvel, qword target, float k1, float k2)
{
    register struct CLOCK *pclock __asm__("v0");
    register qword s1 __asm__("vf3");
    register qword s2 __asm__("vf4");
    register qword s3 __asm__("vf5");
    register qword d __asm__("vf1");
    register qword v __asm__("vf2");
    if (p->f300) {
        *ppos = target;
        pvel[2] = 0.0f;
        pvel[1] = 0.0f;
        pvel[0] = 0.0f;
        return;
    }
    pclock = &D_002DDED0;
    __asm__("qmtc2.ni %1, %0" : "=j"(s1) : "r"(k1));
    d = *ppos;
    __asm__("qmtc2.ni %1, %0" : "=j"(s2) : "r"(k2));
    __asm__("vsub.xyzw %0, %1, %0" : "+j"(d) : "j"(target));
    __asm__("qmtc2.ni %1, %0" : "=j"(s3) : "r"(pclock->dt.n));
    v = *(qword *)pvel;
    __asm__("vmulax.xyzw ACC, %0, %2x\n\tvmaddx.xyzw %0, %1, %3x" : "+j"(d) : "j"(v), "j"(s1), "j"(s2));
    { VU u; u.q = s1; }
    __asm__("vadda.xyzw ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(v) : "j"(d), "j"(s3));
    { VU u; u.q = s2; }
    *(qword *)pvel = v;
    {
        register qword s4 __asm__("vf3");
        qword x;
        { register int n __asm__("v0") = pclock->dt.n;
        __asm__("qmtc2.ni %1, %0" : "=j"(s4) : "r"(n)); }
        x = *ppos;
        { VU u; u.q = s3; }
        __asm__("vadda.xyzw ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(x) : "j"(v), "j"(s4));
        { VU u; u.q = s4; }
        *ppos = x;
    }
}
} } // namespace M_func_0014E6D0

namespace M_AdjustCmCpdefi { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x2C0]; qword pos; qword vel; qword pos2; qword vel2; };
struct O { char pad[0x4F4]; int x4F4; };
struct S { qword a; qword b; qword c[2]; qword d; };
extern qword D_002A2DD0;
qword CalcCmAdjust(struct P *p, struct S *ps);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int k);
void func_0014E6D0(struct P *p, qword *ppos, qword *pvel, qword target, float k1, float k2);
/** @sly2-annot addr=0x14E768
 * Purpose: Springs dposAdjust/dposFocus (+0x2C0/+0x2E0) toward CalcCmAdjust; per STEPPLAYER mode +0x4F4
 *   (0/1/2) different gains; writes cpdefi.posAdjust = posBase + dposAdjust
 * Info: proposed name AdjustCmCpdefi (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void AdjustCmCpdefi(struct P *p, struct O *po, struct S *ps)
{
    qword q = CalcCmAdjust(p, ps);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x34)) {
        switch (po->x4F4) {
        case 0:
            func_0014E6D0(p, &p->pos, &p->vel, q, 5.0f, -3.5f);
            func_0014E6D0(p, &p->pos2, &p->vel2, D_002A2DD0, 5.0f, -3.5f);
            break;
        case 1:
            func_0014E6D0(p, &p->pos, &p->vel, q, 1.0f, -10.0f);
            func_0014E6D0(p, &p->pos2, &p->vel2, p->pos2, 0.0f, -10.0f);
            break;
        case 2:
            func_0014E6D0(p, &p->pos, &p->vel, q, 5.0f, -3.5f);
            break;
        }
    } else {
        func_0014E6D0(p, &p->pos, &p->vel, q, 5.0f, -3.5f);
        func_0014E6D0(p, &p->pos2, &p->vel2, D_002A2DD0, 5.0f, -3.5f);
    }
    {
        qword r;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(ps->b), "j"(p->pos));
        ps->d = r;
    }
}
} } // namespace M_AdjustCmCpdefi

/** @sly2-annot addr=0x14E920
 * Purpose: dposAdjust = CalcCmAdjust(); zero vAdjust (+0x2D0) and vFocus (+0x2F0)
 * Info: proposed name ResetCmAdjust (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
static inline void ZeroVector(VECTOR *pv)
{
    pv->z = 0.0f;
    pv->y = 0.0f;
    pv->x = 0.0f;
}
extern "C" void ResetCmAdjust(CM *pcm)
{
    pcm->dposAdjust = CalcCmAdjust(pcm);
    ZeroVector(&pcm->vAdjust);
    ZeroVector(&pcm->vFocus);
}

namespace M_GetCmCpdefi { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct S { qword a[5]; };
struct VTE { short d; short pad; void (*pfn)(void *, struct S *); };
struct O { int pad[3]; struct VTE *vt; };
struct P { char pad[0x260]; struct O *po; int pad2[3]; struct S s; };
qword CalcCmAdjust(struct P *p, struct S *ps);
/** @sly2-annot addr=0x14E968
 * Purpose: If pso == psoAdjust(+0x260) copy cached cpdefi (+0x270) else pso->vt50 GetCpdefi and posAdjust =
 *   posBase + CalcCmAdjust
 * Info: proposed name GetCmCpdefi (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void GetCmCpdefi(struct P *p, struct O *po, struct S *ps)
{
    if (po == p->po) {
        *ps = p->s;
    } else {
        struct VTE *e = &po->vt[50];
        qword q, r;
        e->pfn((char *)po + e->d, ps);
        q = CalcCmAdjust(p, ps);
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(ps->a[1]), "j"(q));
        ps->a[4] = r;
    }
}
} } // namespace M_GetCmCpdefi

namespace M_FFindCmClearInterval { extern "C" {
/* float g placed after the pointer params: its param copy comes after pgMax, lengthening pgMax so the hoisted 0x40 wins $fp in global alloc */
typedef int qword __attribute__((mode(TI)));
struct T { int a, b, c; };
struct O { char pad[0x70]; int c; struct T *at; };
struct HIT { char pad[0x40]; float g40; float g44; char pad2[0x18]; };
int func_00118668(struct T *pBegin, struct T *pEnd, qword pos, qword dir, int c, struct HIT *ahit);
int ClsgMergeAlsg(int c, struct HIT *ahit, int cNew, struct HIT *ahitNew, int cMax, struct HIT *ahitOut, int f);
/** @sly2-annot addr=0x14EA08
 * Purpose: Clips segment pos->dir against each SO's camera BSP (0x118668), merges intervals (0x14C9F8),
 *   returns first clear gap > duMin in *puFrom/ *puTo
 * Info: proposed name FFindCmClearInterval (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int FFindCmClearInterval(int n, struct O **apo, qword pos, qword dir, float *pgMin, float *pgMax, float g)
{
    struct HIT ahit[0x40];
    int c = 0;
    int i;
    int j;
    float g0, g1;
    for (i = 0; i < n; i++) {
        struct O *po = apo[i];
        int cNew = func_00118668(po->at, po->at + po->c, pos, dir, 0x40 - c, &ahit[c]);
        if (cNew)
            c = ClsgMergeAlsg(c, ahit, cNew, &ahit[c], 0x40, ahit, 0);
    }
    for (j = 0; ; j++) {
        if (c < j)
            return 0;
        g0 = 0.0f;
        if (j > 0)
            g0 = ahit[j - 1].g44;
        if (j < c)
            g1 = ahit[j].g40;
        else
            g1 = 1.0f;
        if (g < g1 - g0)
            break;
    }
    if (pgMin)
        *pgMin = g0;
    if (pgMax)
        *pgMax = g1;
    return 1;
}
} } // namespace M_FFindCmClearInterval

namespace M_FindCmClosestClearPos {
typedef int qword __attribute__((mode(TI)));
extern "C" {
extern int D_002BD8B0[];
qword SetVectorSphere(float pan, float tilt, float s);
int FFindCmClearInterval(void *p, void *x, qword a, qword b, float *pt, float *pu, float g);
qword FindCmClosestClearPos(void *p, void *x, qword pos, float g);
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };

static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}

static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2, VU_FLOAT f3, VU_VECTOR v3)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %1, %4x\n\tvmaddax.xyzw $ACC, %2, %5x\n\tvmaddx.xyzw %0, %3, %6x"
            : "=j"(r) : "j"(v1.q), "j"(v2.q), "j"(v3.q), "j"(f1.q), "j"(f2.q), "j"(f3.q));
    return r;
}

/** @sly2-annot addr=0x14EB70
 * Purpose: Samples 9 tilts x D_002BD8B0[i] pans at radius g, padding 5/g, returns closest clear position
 * Info: proposed name FindCmClosestClearPos (T1); TU cm
 * Notes: names_v2 v1 proposed UpdateCmFade (0.200)
 * Confidence: high [T1 0.90] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword FindCmClosestClearPos(void *p, void *x, qword pos, float g)
{
    float t, u;
    qword best;
    float dg = 5.0f / g;
    float gBest = 3.40282346638528859812e+38f;
    int i, j;

    best = pos;
    for (i = 0; i < 9; i++) {
        float tilt = (float)(i - 4) * 0.3926991f;
        int c = D_002BD8B0[i];
        for (j = 0; j < c; j++) {
            qword dir = SetVectorSphere((float)j * 3.1415927f / (float)c, tilt, 1.0f);
            qword test = AddScaled(pos, dir, VU_FLOAT(g));
            if (FFindCmClearInterval(p, x, pos, test, &t, &u, dg + dg)) {
                if (0.0f < t)
                    t += dg;
                if (t < gBest) {
                    gBest = t;
                    best = VecCombo(VU_FLOAT(1.0f - gBest), pos, VU_FLOAT(gBest), test, VU_FLOAT(-1.0f), dir);
                }
            }
        }
    }
    return best;
}
} // namespace M_FindCmClosestClearPos

namespace M_SquishCmEye { extern "C" {
/* MATCH (Claude agent, lane D4): v2 inline Dot(qword *pa, qword *pb) (keeps &ax[i].normal hoisted in a reg), returning int from a qmfc2 into `register int d asm("$7")`, stored through `*(int *)pg++` (sw $a3 like the target). */
typedef int qword __attribute__((mode(TI)));
struct X { qword pos; qword normal; qword x20; };
struct E { int a, b, c; };
struct LO { char pad[0x70]; int c70; struct E *ae74; };
struct AL { int cb; int cbAlign; };
extern float D_002BD8D4;
void STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n);
void *func_001AE318(struct AL *pal);
int CsbiIntersectSphereBsp(qword pos, struct LO *po, struct E *pe, float g, int a, int b, int cMax, struct X *ax);
void SolveInequalities(int c, float *aDot, float *aW, float *aZ, float g);
qword LimitVectorLength(qword v, float g);

static inline int Dot(qword *pa, qword *pb)
{
    register qword nj __asm__("vf1") = *pb;
    register qword t __asm__("vf2") = *pa;
    qword one;
    register int d __asm__("$7");
    __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "+j"(t), "=&j"(one) : "j"(nj));
    __asm__("qmfc2.ni %0, %1" : "=r"(d) : "j"(t));
    return d;
}

static inline void *PvAlloc(int cb, int cbAlign)
{
    struct AL al;
    al.cb = cb;
    al.cbAlign = cbAlign;
    return func_001AE318(&al);
}

/** @sly2-annot addr=0x14EDA0
 * Purpose: Collects sphere-vs-BSP contacts (0x1A93C8) for the eye, builds n x n normal dot matrix +
 *   constraint values via pfn, solves (0x11AD30) and returns v + sum of normal impulses
 * Info: proposed name SquishCmEye (T1); TU cm
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword SquishCmEye(void *p, int c, struct LO **apo, float (*pfn)(void *, qword, qword, struct X *),
                    qword pos, qword v, float g, float h)
{
    struct X ax[64];
    int n = 0;
    int i, j;

    {
    int k;
    for (k = 0; k < c; k++) {
        struct LO *po = apo[k];
        n += CsbiIntersectSphereBsp(pos, po, &po->ae74[po->c70], g, 0, 0, 64 - n, &ax[n]);
    }
    }
    if (n == 0)
        return v;
    {
        struct { int a[4]; } frame;
        float *pg;
        qword r, sum, z;
        float *aDot, *aW, *aZ;
        STACKSCOPE_ctor(&frame);
        __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(z));
        aDot = (__typeof__(aDot))PvAlloc(n * n * 4, 0x10);
        aW = (__typeof__(aW))PvAlloc(n * 4, 0x10);
        aZ = (__typeof__(aZ))PvAlloc(n * 4, 0x10);
        pg = aDot;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++)
                *(int *)pg++ = Dot(&ax[i].normal, &ax[j].normal);
        }
        pg = aW;
        for (i = 0; i < n; i++)
            *pg++ = pfn(p, pos, v, &ax[i]);
        SolveInequalities(n, aDot, aW, aZ, D_002BD8D4 * h);
        pg = aZ;
        sum = z;
        for (i = 0; i < n; i++) {
            union { qword q; float f[4]; } s;
            {
                qword qwsqOut;
                __asm__("qmtc2.ni %1, %0" : "=j"(qwsqOut) : "r"(*pg++));
                s.q = qwsqOut;
            }
            __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(sum) : "0"(sum), "j"(ax[i].normal), "j"(s.q));
        }
        {
            qword q;
            __asm__("qmfc2.ni %0, %1" : "=r"(q) : "j"(sum));
            r = LimitVectorLength(q, h);
        }
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(v), "j"(r));
        STACKSCOPE_ctor_1AE4A0(&frame, 2);
        return r;
    }
}
} } // namespace M_SquishCmEye

/** @sly2-annot addr=0x14F030
 * Purpose: Filter callback: true if SO has camera-BSP nodes (+0x70 count > 0)
 * Info: proposed name FFilterCamera (T1); TU cm
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
/* Camera filter callback: accepts objects whose +0x70 word is positive. */
extern "C" int FFilterCamera(void *pv, LO *plo)
{
    return *(int *)((char *)plo + 0x70) > 0;
}

namespace M_SvSquishCmEyeConstraint { extern "C" {
// MATCH (Claude agent, lane D24, 2026-10-23): v6 with the mtc1 asm's allocation-only "j" operands replaced by an
// "r,j" constraint on td (never-chosen alternative, forced off by an unused "i,r"(0) operand). With fewer operands
// the mtc1 has 4 dependents like the `one` sqc2, so the luid tie-break issues the sqc2 first, like the target.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct PL { qword pos; qword plane; };
struct P { char pad[0x48]; float g48; };
extern float D_002BD8D8;
extern char D_002BD8E0[];
extern float D_002DDEE0;
void *func_002564B8(void *p);
void GSmooth__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr);
static __inline__ float GDistPlane(qword plane, qword pos)
{
    U d, one;
    qword td, tone;
    float dist;
    func_002564B8(&d);
    func_002564B8(&one);
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w" : "=&j"(td), "=&j"(tone) : "j"(plane), "j"(pos));
    /* "r,j"(td): the j alternative is allocation-only (keeps td a VF reg, so it is stored with sqc2 and moved with
       qmfc2 v0); the unused "i,r"(0) operand makes that alternative always need a reload, so reload picks "r".
       Only td is read, so the asm has no anti-dependence on the later vf1 load and the sqc2 of `one` wins the tie. */
    __asm__("mtc1 %1, %0" : "=f,f"(dist) : "r,j"(td), "i,r"(0));
    one.q = tone;
    d.q = td;
    return dist;
}
static __inline__ float GDot(const qword *pa, qword b)
{
    qword a = *pa;
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
/** @sly2-annot addr=0x14F040
 * Purpose: Constraint for SquishCmEye: GSmooth(dist-to-plane, sNearCorner(+0x48) - k) ; return dot(vEye,
 *   normal) - smoothed
 * Info: proposed name SvSquishCmEyeConstraint (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
float SvSquishCmEyeConstraint(struct P *p, int unused, qword vIn, struct PL *pl)
{
    float r;
    qword plane = pl->plane;
    qword pos = pl->pos;
    GSmooth__FfffP3SMPPf(GDistPlane(plane, pos), p->g48 - D_002BD8D8, D_002DDEE0, D_002BD8E0, &r);
    return GDot(&vIn, pl->plane) - r;
}
} } // namespace M_SvSquishCmEyeConstraint

namespace M_func_0014F110 {
// func_0014F110 (Claude agent, lane K5, 2026-10-24): MATCH. Built on laneK4v1 (K3/K4 fresh rewrite in the func_0014C748 style).
// Keys: the VU_FLOAT(const float *) ctor's word input has a never-chosen "V" alternative ("r,V"), so it stays a MEM until
// reload: the clock dt load is then an input reload (lw v0 for the vdiv; for the final AddScaled reload reuses the dying
// pclock register, lw s0,0x10(s0)), which also shortens the clock chain in sched1 so no extra operand is needed on the vdiv;
// v is computed before posMin so the spill slots come out v 0x30 / posMin 0x40. No register pins.
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(*(const int *)pg)); } /* never-chosen "V" (non-offsettable mem) alternative: allocation-only (the word stays a MEM until reload) */ };
struct CLOCK { float t, dt, x8, xC, dtPrev; };
struct FRAME { int n; FRAME() __asm__("STACKSCOPE_ctor"); ~FRAME(); }; /* out-of-line ctor/dtor: STACKSCOPE_ctor / STACKSCOPE_ctor_1AE4A0(this, 2) */
extern "C" {
extern CLOCK D_002DDED0;
extern qword D_002BD8F0, D_002BD900;
extern float D_002BD8D8, D_002BD8DC;
extern char D_003DD490[];
void *STACKSCOPE_ctor(FRAME *p);
void STACKSCOPE_ctor_1AE4A0(FRAME *p, int n);
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj);
qword SquishCmEye(void *p, int i, int j, void *pfn, qword a, qword v, float g, float h);
void FFilterCamera(void);
void SvSquishCmEyeConstraint(void);
qword func_0014F110(void *p, qword pos, qword posTarget);
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Add(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword DivideBy(const qword &v, const VU_FLOAT &g) { qword r; __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
static inline qword AddScaled(qword base, const qword &vec, const VU_FLOAT &s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(base), "j"(vec), "j"(s.q));
    return r;
}
static inline qword QwDot(const qword &a, const qword &b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword QwSqrt(const qword &a) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(a)); return r; }
static inline float GXOfQw(const qword &q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static inline float Length(const qword &v) { return GXOfQw(QwSqrt(QwDot(v, v))); }

/** @sly2-annot addr=0x14F110
 * Purpose: vEye=(next-prev)/dtPrev; gather camera-colliding SOs in a box around prev; SquishCmEye; return
 *   prev + vClip*dt
 * Info: proposed name ClipCmEye (T1); TU cm
 * Notes: names_v2 v1 proposed SetupCmRotateToCam (0.005)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword func_0014F110(void *p, qword pos, qword posTarget)
{
    CLOCK *pclock = &D_002DDED0;
    int ii, jj;
    qword dpos = Sub(posTarget, pos);
    qword v = DivideBy(dpos, VU_FLOAT(&pclock->dtPrev));
    qword posMin = Add(pos, D_002BD8F0);
    qword posMax = Add(pos, D_002BD900);
    FRAME frame;
    IntersectSwBoundingBox(D_003DD490, posMin, posMax, (void *)FFilterCamera, 0, &ii, &jj);
    qword w = SquishCmEye(p, ii, jj, (void *)SvSquishCmEyeConstraint, pos, v, D_002BD8D8, Length(v) + D_002BD8DC);
    qword r = AddScaled(pos, w, VU_FLOAT(&pclock->dtPrev));
    return r;
}
} // namespace M_func_0014F110

namespace M_ConvertCmScreenToWorld { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x40]; float sx, sy; char pad2[0x18]; M4 mat; };
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Mul(M4 *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(o) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]));
    return o;
}
/** @sly2-annot addr=0x14F268
 * Purpose: Screen (x,y,depth) -> world: axis swap, scale by tanHalfX/Y (+0x40/+0x44), multiply by CM::mat
 * Info: proposed name ConvertCmScreenToWorld (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword ConvertCmScreenToWorld(struct CM *p, qword v)
{
    union { qword q; float f[4]; } r;
    float x = GetX(v);
    float z = GetZ(v);
    float y = GetY(v);
    r.f[2] = y;
    r.f[0] = z;
    r.f[1] = -x;
    if (z > 1e-4f) {
        r.f[1] = -x * z;
        r.f[2] = y * z;
    }
    r.f[1] *= p->sx;
    r.f[2] *= p->sy;
    return Mul(&p->mat, r.q);
}
} } // namespace M_ConvertCmScreenToWorld

namespace M_ConvertCmWorldToScreen { extern "C" {

typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x40]; float sx, sy; char pad2[0x18]; M4 mat; };
void InvertMatrix4Rigid(M4 *pm, M4 *pn);
/** @sly2-annot addr=0x14F308
 * Purpose: World -> screen: inverse(CM::mat) * pos, axis swap, divide by tanHalf and depth (if > 1e-4)
 * Info: proposed name ConvertCmWorldToScreen (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword ConvertCmWorldToScreen(struct CM *p, qword v)
{
    union { qword q; float f[4]; } r;
    M4 m;
    float x, y, z;
    InvertMatrix4Rigid(&m, &p->mat);
    {
        qword qwrqOut;
        __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(qwrqOut) : "j"(m.r[0]), "j"(v), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]));
        r.q = qwrqOut;
    }
    x = r.f[0];
    y = r.f[1];
    z = r.f[2];
    r.f[2] = x;
    r.f[0] = -y / p->sx;
    r.f[1] = z / p->sy;
    if (x > 1e-4f) {
        r.f[0] = r.f[0] / x;
        r.f[1] = r.f[1] / x;
    }
    return r.q;
}
} } // namespace M_ConvertCmWorldToScreen

namespace M_SetCmPos { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { struct { qword r[4]; } q; float af[4][4]; } M4;
struct CM { char pad[0x60]; M4 mat; };
qword *VECTOR_opAssign(qword *p, qword v);
void SetCmMat4(struct CM *p, M4 *m);
/** @sly2-annot addr=0x14F3C8
 * Purpose: Option setter (g_aeopid[171], otyp 2 vector): keep rotation, set CM::mat pos row, SetCmMat4
 * Info: proposed name SetCmPos (T1); TU cm
 * Notes: names_v2 v1 proposed SetupCm__FP2CM (0.438)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmPos(struct CM *p, qword pos)
{
    M4 m = p->mat;
    VECTOR_opAssign(&m.q.r[3], pos);
    m.af[3][3] = 1.0f;
    SetCmMat4(p, &m);
}
} } // namespace M_SetCmPos

namespace M_SetCmMat { extern "C" {
#include "mat.h"
struct CM { char pad[0x90]; struct VECTOR pos; };
void SetCmMat4(struct CM *p, struct MATRIX4 *m);
/** @sly2-annot addr=0x14F428
 * Purpose: Set rotation from MATRIX3, keep pos, SetCmMat4
 * Info: proposed name SetCmMat (T1); TU cm
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void SetCmMat(struct CM *p, struct MATRIX3 *pmat)
{
    struct MATRIX4 m;
    *(struct MATRIX3 *)&m = *pmat;
    *(struct VECTOR *)m.mat[3] = p->pos;
    m.mat[2][3] = 0.0f;
    m.mat[1][3] = 0.0f;
    m.mat[0][3] = 0.0f;
    SetCmMat4(p, &m);
}
} } // namespace M_SetCmMat

namespace M_GetCmRotDeg { extern "C" {
typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
qword func_0011BFF0(void *p);
/** @sly2-annot addr=0x14F478
 * Purpose: Option getter (g_aeopid[173]): euler angles of CM::mat (0x11BFF0) in degrees
 * Info: proposed name GetCmRotDeg (T1); TU cm
 * Notes: names_v2 v1 proposed SetCmPosMat__FP2CMP6VECTORP7MATRIX3 (0.167)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources:
 *   r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword GetCmRotDeg(char *p)
{
    union { qword q; float f; } r;
    qword v = func_0011BFF0(p + 0x60);
    r.q = Make(GetX(v) * 57.295776f, GetY(v) * 57.295776f, GetZ(v) * 57.295776f);
    return r.q;
}
} } // namespace M_GetCmRotDeg

namespace M_SetCmRotDeg { extern "C" {
typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
typedef struct { qword r[3]; } M3;
typedef union { struct { qword r[4]; } q; M3 m3; float af[4][4]; } M4;
struct CM { char pad[0x60]; M4 mat; };
void LoadRotateMatrixEuler(M3 *pm, qword v);
void SetCmMat4(struct CM *p, M4 *m);
/** @sly2-annot addr=0x14F4E0
 * Purpose: Option setter (g_aeopid[173]): degrees -> radians, rotation matrix 0x11B628, keep pos, SetCmMat4
 * Info: proposed name SetCmRotDeg (T1); TU cm
 * Notes: names_v2 v1 proposed ConvertCylindToWorldVelocity (0.200)
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmRotDeg(struct CM *p, qword v)
{
    M4 mat = p->mat;
    union { qword q; float f; } r;
    M3 m;
    r.q = Make(GetX(v) * 0.017453294f, GetY(v) * 0.017453294f, GetZ(v) * 0.017453294f);
    LoadRotateMatrixEuler(&m, r.q);
    mat.m3 = m;
    mat.af[2][3] = 0.0f;
    mat.af[1][3] = 0.0f;
    mat.af[0][3] = 0.0f;
    SetCmMat4(p, &mat);
}
} } // namespace M_SetCmRotDeg

namespace M_SetCmFov__FP2CMf { extern "C" {
struct CM { char pad[0x20]; float a, b; int c; };
void RecalcCmFrustrum__FP2CM(struct CM *p);
/** @sly2-annot addr=0x14F5A0
 * Purpose: fov(+0x20)=fovTarget(+0x24)=f, dfov=0; RecalcCmFrustrum if changed > 1e-4
 * Info: proposed name SetCmFov (T1); TU cm
 * Confidence: high [T1 0.90] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmFov__FP2CMf(struct CM *p, float f)
{
    int fChanged = __builtin_fabsf(f - p->a) > 0.0001f;
    p->a = f;
    p->b = f;
    p->c = 0;
    if (fChanged) RecalcCmFrustrum__FP2CM(p);
}
} } // namespace M_SetCmFov__FP2CMf

/** @sly2-annot addr=0x14F5F8
 * Purpose: Option setter g_aeopid[162] (field +0x24): fovTarget=f and the active policy's saved fov
 *   (acpr[0].pcplcy+4) so Deactivate restores it
 * Info: proposed name SetCmBaseFovTarget (T1); TU cm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void SetCmBaseFovTarget(CM *pcm, float fovTarget)
{
    CPLCY *pcplcy = pcm->acpr[0].pcplcy;
    pcm->fovTarget = fovTarget;
    if (pcplcy != 0) {
        pcplcy->gFovTargetSaved = fovTarget;
    }
}

/** @sly2-annot addr=0x14F610
 * Purpose: sNearClip(+0x4C)=f; RecalcCmFrustrum
 * Info: proposed name SetCmNearClip (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmNearClip(CM *pcm, float sNearClip)
{
    pcm->sNearClip = sNearClip;
    RecalcCmFrustrum(pcm);
}

/** @sly2-annot addr=0x14F630
 * Purpose: Option setter g_aeopid[164]: SetCmNearClip and the active policy's saved near clip (pcplcy+8)
 * Info: proposed name SetCmBaseNearClip (T1); TU cm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void SetCmBaseNearClip(CM *pcm, float sNearClip)
{
    CPLCY *pcplcy;
    SetCmNearClip(pcm, sNearClip);
    pcplcy = pcm->acpr[0].pcplcy;
    if (pcplcy != 0) {
        pcplcy->sNearClipSaved = sNearClip;
    }
}

/** @sly2-annot addr=0x14F670
 * Purpose: sFarClip(+0x50)=f; RecalcCmFrustrum
 * Info: proposed name SetCmFarClip (T1); TU cm
 * Notes: names_v2 v1 proposed SetCmCut__FP2CMPf (0.000)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmFarClip(CM *pcm, float g)
{
    pcm->sFarClip = g;
    RecalcCmFrustrum(pcm);
}

/** @sly2-annot addr=0x14F690
 * Purpose: sNearFog(+0x240)=f; RecalcCmFrustrum
 * Info: proposed name SetCmNearFog (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmNearFog(CM *pcm, float g)
{
    pcm->sNearFog = g;
    RecalcCmFrustrum(pcm);
}

/** @sly2-annot addr=0x14F6B0
 * Purpose: sFarFog(+0x244)=f; RecalcCmFrustrum
 * Info: proposed name SetCmFarFog (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_B
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmFarFog(CM *pcm, float g)
{
    pcm->sFarFog = g;
    RecalcCmFrustrum(pcm);
}

/** @sly2-annot addr=0x14F6D0
 * Purpose: uFogMax(+0x248)=f; RecalcCmFrustrum
 * Info: proposed name SetCmUFogMax (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmUFogMax(CM *pcm, float g)
{
    pcm->uFogMax = g;
    RecalcCmFrustrum(pcm);
}

namespace M_SetCmRgbaFog__FP2CMP4RGBA { extern "C" {
struct S4 { char a[4]; };
struct CM { char pad[0x24C]; struct S4 s; };
void ColorCorrectRgba(void *p);
void RecalcCmFrustrum__FP2CM(struct CM *p);
/** @sly2-annot addr=0x14F6F0
 * Purpose: rgbaFog(+0x24C)=*prgba; 0x1ED278 on it; RecalcCmFrustrum
 * Info: proposed name SetCmRgbaFog (T1); TU cm
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmRgbaFog__FP2CMP4RGBA(struct CM *p, struct S4 *s)
{
    p->s = *s;
    ColorCorrectRgba(&p->s);
    RecalcCmFrustrum__FP2CM(p);
}
} } // namespace M_SetCmRgbaFog__FP2CMP4RGBA

/** @sly2-annot addr=0x14F730
 * Purpose: rMRD ratio (+0x18)=f; RecalcCmFrustrum (which derives +0x1C = max(r/fov,1))
 * Info: proposed name SetCmMrdRatio (T1); TU cm
 * Notes: names_v2 v1 proposed ClearCmCut__FP2CM (0.000)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmMrdRatio(CM *pcm, float g)
{
    pcm->rFovCullRef = g;
    RecalcCmFrustrum(pcm);
}

namespace M_func_0014F750 { extern "C" {
extern float D_002DDED0;
/** @sly2-annot addr=0x14F750
 * Purpose: tJolt(+0x618)=clock.t, nUnk61c=arg: starts the 0.5 s camera shake applied by UpdateCmMat4
 * Info: proposed name JoltCm (T1); TU cm
 * Notes: names_v2 v1 proposed FUN_00145dd8__FPvP2SO (0.000)
 * Confidence: high [T1 0.80] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F750(char *p, int a)
{
    *(float *)(p + 0x618) = D_002DDED0;
    *(int *)(p + 0x61C) = a;
}
} } // namespace M_func_0014F750

namespace M_func_0014F768 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x60]; M3 mat; qword pos; char pad2[0x100 - 0xA0]; char m100[0x520]; float x620; };
void VECTOR_opAssign(qword *p, qword v);
void func_0014D8F8(M4 *pm, void *pv, int n);
static __inline__ void LoadM4(M4 *pm, M3 *pmat)
{
    pm->r[0] = pmat->r[0];
    pm->r[1] = pmat->r[1];
    pm->r[2] = pmat->r[2];
    ((int *)&pm->r[1])[3] = 0;
    ((int *)&pm->r[2])[3] = 0;
    ((int *)&pm->r[0])[3] = 0;
    ((float *)&pm->r[3])[3] = 1.0f;
}
/** @sly2-annot addr=0x14F768
 * Purpose: Builds a view-proj from CM::mat with pos offset along the right axis by +/- rUnk620 (k=1/2) via
 *   CombineEyeLookAtProj into pm (proj = pv or matProj)
 * Info: proposed name BuildCmEyeViewProj (T2); TU cm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/rendering.md#8-blips-hud-screen-effects,
 *   docs/guides/port-plan.md#54-native-overrides-of-game-functions-when-and-how,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F768(struct CM *p, int k, void *pv, int n, M4 *pm)
{
    U r, g;
    float f = 0.0f;
    switch (k) {
    case 1: f = p->x620; break;
    case 2: f = -p->x620; break;
    }
    {
        qword qwgqOut;
        __asm__("qmtc2.ni %1, %0" : "=j"(qwgqOut) : "r"(f));
        g.q = qwgqOut;
    }
    {
        qword qwrqOut;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwrqOut) : "j"(p->pos), "j"(p->mat.r[1]), "j"(g.q));
        r.q = qwrqOut;
    }
    VECTOR_opAssign(&pm->r[3], r.q);
    LoadM4(pm, &p->mat);
    func_0014D8F8(pm, pv ? pv : p->m100, n);
}
} } // namespace M_func_0014F768

namespace M_FInsideCmFrustum { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct X { char pad[0xA0]; qword r[3]; };
/** @sly2-annot addr=0x14F850
 * Purpose: True if point/sphere is inside all 4 frustum side planes using the transposed cull rows (+0xA0)
 *   and VU clip flags
 * Info: proposed name FInsideCmFrustum (T1); TU cm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int FInsideCmFrustum(struct X *p, qword v, qword w)
{
    qword one;
    int f;
    __asm__("vmaxw.xyzw %0, $vf0, $vf0w" : "=j"(one));
    __asm__("vmulax.xyzw $ACC, %1, %4x\n\tvmadday.xyzw $ACC, %2, %4y\n\tvmaddaz.xyzw $ACC, %3, %4z\n\tvmsubax.xyzw $ACC, %5, %6x\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tcfc2.ni %0, $vi17"
            : "=r"(f) : "j"(p->r[0]), "j"(p->r[1]), "j"(p->r[2]), "j"(v), "j"(one), "j"(w));
    return (f ^ 0xF0) == 0;
}
} } // namespace M_FInsideCmFrustum

namespace M_FInsideCmMrd { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct P { int pad[7]; float g1C; };
/** @sly2-annot addr=0x14F898
 * Purpose: True if |dpos| < s*rCullScale; *pu = fade fraction between outer radius and (r-400*rCullScale)
 * Info: proposed name FInsideCmMrd (T1); TU cm
 * Notes: names_v2 v1 proposed FUN_00145de8 (0.000)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/rendering.md#10-models-globset-at-render-time-cstatic,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
int FInsideCmMrd(struct P *p, qword v, float g, float *pu)
{
    qword l, one;
    float l2, r, r2, d2;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(v));
    r = g * p->g1C;
    __asm__("qmfc2.ni %0, %1" : "=r"(l2) : "j"(l));
    r2 = r * r;
    if (r2 < l2)
        return 0;
    d2 = r - p->g1C * 400.0f;
    d2 = d2 * d2;
    if (d2 < l2)
        *pu = (r2 - l2) / (r2 - d2);
    else
        *pu = 1.0f;
    return 1;
}
} } // namespace M_FInsideCmMrd


namespace M_func_00256278 { extern "C" {
void SetCmMat(void);
void func_00256278(void) { SetCmMat(); }
} } // namespace M_func_00256278

namespace M_func_00258290 { extern "C" {
typedef struct { int pad[4]; short d; short p; void (*f)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;
typedef struct { char pad[0x484]; O *x484; O *x488; char pad2[0x154]; O *x5E0; O *x5E4; } S;
void func_00258348(S *, int);
void func_00258290(S *p, int x)
{
    if (p->x5E4) p->x5E4->vt->f((char *)p->x5E4 + p->x5E4->vt->d);
    if (p->x5E0) p->x5E0->vt->f((char *)p->x5E0 + p->x5E0->vt->d);
    if (p->x488) p->x488->vt->f((char *)p->x488 + p->x488->vt->d);
    if (p->x484) p->x484->vt->f((char *)p->x484 + p->x484->vt->d);
    func_00258348(p, x);
}
} } // namespace M_func_00258290

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const float D_002A4FF8 = 3.40282347e+38f;
SMP D_002BD810 = {20.0f, 10.0f, 0.5f};
float D_002BD81C = 500.0f;
float D_002BD820 = 20000.0f;
float D_002BD824 = 0.5f;
VECTOR4 D_002BD830 = {0.0f, -2000.0f, 500.0f, 0.0f};
VECTOR4 D_002BD840 = {0.0f, 0.0f, 0.0f, 0.0f};
float D_002BD850 = 0.5f;
float D_002BD854 = 50.0f;
CLQ D_002BD860 = {{{1.0f, -2.0f, 1.0f, 0.0f}}};
VECTOR4 D_002BD870 = {0.0f, 0.0f, 0.0f, 0.0f};
VECTOR4 D_002BD880 = {0.05f, 0.0f, 0.0f, 0.0f};
VECTOR4 D_002BD890 = {0.0f, 0.0f, 0.0f, 0.0f};
VECTOR4 D_002BD8A0 = {0.0f, 0.0f, 0.0f, 0.0f};
int D_002BD8B0[9] = {1, 5, 10, 12, 15, 12, 10, 5, 1};  /* smoothing weights */
float D_002BD8D4 = 4.0f;
float D_002BD8D8 = 125.0f;
float D_002BD8DC = 1500.0f;
SMPA D_002BD8E0 = {2500.0f, 0.0f, 0.25f, 0.0f};
VECTOR4 D_002BD8F0 = {-250.0f, -250.0f, -250.0f, 0.0f};
VECTOR4 D_002BD900 = {250.0f, 250.0f, 250.0f, 0.0f};
