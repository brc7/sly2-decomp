// cm.cpp -- the camera (CM, the global g_cm at D_002DDF40): construction with its embedded policies, setup, the
// view matrix / frustum / fog setters, the visibility zone, adjustment and cuts, the policy stack, the camera
// filters. TU 0x14D3D8..0x14FAC8 (53 functions, address order), then the inline ~CM body DestroyCm (defined
// after its callers: the linkonce copy 0x258290); the matrix setter 0x256278 is an inline function in sly2/cm.h.
//
// Types: sly2/cm.h's CM and policy classes for the m2c bodies; the hand-written per-function files keep their views
// in namespaces named after each function (as jt.cpp does), and their C++-named definitions are C-linkage
// definitions of their literal symbols there (the namespace would change the mangling), as in wave 9.
// g_cm is defined here: its constructor (CM_ctor) and destructor (DestroyCm, ~CM with g++'s in-charge flag 2)
// make the static initializer a constructor-and-destructor one (86 instructions) with a _GLOBAL_$D$ stub
// (func_0014FAA8) next to the _GLOBAL_$I$ one.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_cm
// SLY2_ALIAS: _GLOBAL_$I$*=func_0014FA88
// SLY2_ALIAS: _GLOBAL_$D$*=func_0014FAA8

#include "sly2/basic.h"
#include "sly2/scrap.h"
#include "sly2/cm.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"

/* g_cm: a view of the CM bytes whose inline constructor and destructor call the C-named CM_ctor and ~CM (as heap.cpp's
   level heap and gc.cpp's g_gc are spelled). Code in this file reads it as a CM through G_CM. */
struct CMG;
extern "C" {
CM *CM_ctor(CM *pcm);
void DestroyCm(CMG *pcm, int grfDelete);   /* ~CM (linkonce, below) */
}
struct CMG
{
    char ab[sizeof(CM)];
    CMG() { CM_ctor((CM *)this); }
    ~CMG() { DestroyCm(this, 2); }
} __attribute__((aligned(16)));
CMG D_002DDF40;
#define G_CM (*(CM *)&D_002DDF40)

// 0x14F930 StaticInit_cm (g_cm's construction and destruction, then the six slot pools) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The vtable pointer of a CM or a policy (after three words of data). */
#define PVTCM(p) (*(void **)((char *)(p) + 0xC))

extern "C" {
extern void *D_002CA4D0[] __asm__("_vt$2CM");  /* CM vtable */
extern void *D_002CD378[] __asm__("_vt$9CPTWOSHOT");  /* CPTWOSHOT vtable */
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

// CM constructor: BASIC header, vptr D_002CA4D0 (CM vtable), then constructs the 9 embedded camera policies at
// +0x3A0..+0x600.
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

// Camera setup hook called once at startup by main (after the CM constructor has run); empty in the shipped game.
void SetupCm(CM *pcm)
{
}

namespace M_UpdateCmLast__FP2CM { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; struct O *poTarget; int b; };
struct CM {
    char pad[0x20]; float fov; float targetFov; float fovSpeed; float fovSmoothing;
    char pad1[0x260 - 0x30]; struct O *adjustSo; char pad2[0x270 - 0x264]; int policyDefaults[36];
    int cutCount; int pad3[2]; int policyCount; struct E policies[8];
    char pad4[0x620 - 0x390]; float stereoEyeOffset; float stereoFocusDistance;
};
struct G { char pad[0x3E38]; int unknown3E38; };
extern struct CLOCK D_002DDED0;
extern int D_002DFBC0;
extern char D_002DFC00[];
extern char D_002E0D00[];
extern struct G D_003DD490;
extern char D_002BD810[];
void ResetCmAdjust(struct CM *p, void *pv);
void AdjustCmCpdefi(struct CM *p, struct O *po, void *pv);
void SetCmFov__FP2CMf(struct CM *p, float g);
float SmoothValueWithAccel__FffffP4SMPAPf(float g, float h, float i, float dt, float *pa, float *pb);
void RecalcCmFrustrum__FP2CM(struct CM *p);
float SmoothValue__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr);
// Per-frame camera update: runs the top policy of the stack (vtable slot 6 Update, with the focus object's CPDEFI
// from its vtable slot 50 and the current joypad), again whenever the update replaced the top policy; then the fov:
// snapped on a cut, else smoothed toward the target; clears the cut and, in stereo mode, smooths the eye offset.
void UpdateCmLast__FP2CM(struct CM *cm)
{
    struct O *policy;
    struct O *lastUpdated;
    struct O *lastFocus;
    if (cm->policyCount > 0) {
        lastUpdated = 0;
        lastFocus = 0;
        while ((policy = cm->policies[0].po) != lastUpdated) {
            struct O *focus = cm->policies[0].poTarget;
            void *cpdefi;
            if (focus) {
                cpdefi = cm->policyDefaults;
                if (focus != lastFocus) {
                    struct VTE *e = &focus->vt[50];
                    e->pfn((char *)focus + e->d, cpdefi);
                    if (lastUpdated || focus != cm->adjustSo)
                        ResetCmAdjust(cm, cpdefi);
                    AdjustCmCpdefi(cm, focus, cpdefi);
                    cm->adjustSo = focus;
                    lastFocus = focus;
                }
            } else {
                cm->adjustSo = 0;
                cpdefi = 0;
                lastFocus = 0;
            }
            {
                struct VTE *e = &policy->vt[6];
                e->pfn((char *)policy + e->d, cpdefi, (D_002DFBC0 & 1) ? D_002DFC00 : D_002E0D00);
            }
            lastUpdated = policy;
            if (cm->policyCount <= 0)
                break;
        }
    }
    if (cm->cutCount) {
        SetCmFov__FP2CMf(cm, cm->targetFov);
    } else if (cm->targetFov != cm->fov) {
        cm->fov = SmoothValueWithAccel__FffffP4SMPAPf(cm->fov, cm->fovSpeed, cm->targetFov, D_002DDED0.dt, &cm->fovSmoothing, &cm->fovSpeed);
        RecalcCmFrustrum__FP2CM(cm);
    }
    cm->cutCount = 0;
    if (D_003DD490.unknown3E38)   /* stereo mode (draw.cpp) */
        cm->stereoEyeOffset = SmoothValue__FfffP3SMPPf(cm->stereoEyeOffset, cm->stereoFocusDistance, g_clock.cameraDeltaTime, D_002BD810, 0);
}
} } // namespace M_UpdateCmLast__FP2CM

namespace M_ResetCameraPolicies { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { char pad[0xC]; struct VTE *vt; };
struct E { struct O *po; int pad[3]; };
struct P { char pad[0x30C]; int policyCount; int pad2; struct E policies[8]; char pad3[0x620 - 0x394];
            int stereoEyeOffset; float stereoFocusDistance; };   /* +0x620 stored as a word (0) here */
// Calls vtable slot 7 (unknown: reset) on every policy on the stack, then resets the stereo eye offset (0) and focus
// distance (10).
void ResetCameraPolicies(struct P *p)
{
    int i;
    for (i = 0; i < p->policyCount; i++) {
        struct O *o = p->policies[i].po;
        struct VTE *e = &o->vt[7];
        ((void (*)(void *))e->pfn)((char *)o + e->d);
    }
    p->stereoEyeOffset = 0;
    p->stereoFocusDistance = 10.0f;
}
} } // namespace M_ResetCameraPolicies

namespace M_InitCm__FP2CM {
// InitCm: near/far and LM init via inline helpers (parameter pseudos give the original FP register choice).
typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned char r, g, b, a; RGBA(unsigned int n) { *(unsigned int *)this = n; } };
struct V3 { float x, y, z; };
struct LM { float g0; int n4; float g8; float gC; };
struct CM {
    char pad[0x10]; int visZone; int visFlags; float fovCullRef; float cullScale; float fov; float targetFov; int pad28;
    float fovSmoothing; int fovSmoothing30; float fovSmoothing34; float fovSmoothing38; float aspect; int pad40[3]; float nearClip; float farClip; int pad54[3];
    float mat[16]; char padA0[0x240 - 0xA0]; V3 nearFog; RGBA fogColor;
    char pad250[0x30C - 0x250]; int policyCount; char pad310[0x3A0 - 0x310]; int defaultPolicy[4]; int followPolicy[40]; int lookPolicy[22];
    int unknownPolicy4A8[6]; int hangPolicy[16]; int blendPolicy[8]; int zoomPolicy[52]; int unknown5F0[10]; float joltTime; int joltKind;
};
extern "C" {
extern float D_002BD81C, D_002BD820, D_002BD824;
extern qword D_002BD830;
extern qword D_002BD840;
void *func_002333B4(void *pv, int b, int cb) __asm__("memset");
void RecalcCmFrustrum__FP2CM(CM *p);
void SetCameraLookAt(CM *p, qword pos, qword tgt, float da, float db);
void CutCm__FP2CM(CM *p);
void InitCmpart1(void *p);
void InitCptn(void *p);
void InitCplcy(void *p);
void SetCmPolicy(CM *p, int prio, void *po, int a, int b);
}
static inline void SetNF(CM *cm, float n, float f) { cm->nearClip = n; cm->farClip = f; }
static inline void InitLm(LM *plm, float g0, float g8, float gC) { plm->g0 = g0; plm->gC = gC; plm->g8 = g8; plm->n4 = 0; }
// Camera init: fogColor fog 0x80303030, aspect 1.2987, near/far 100/100000, identity mat, RecalcCmFrustrum, SetCmLookAt,
// CutCm, Init on each embedded policy, push default policy (+0x3A0) with SetCmPolicy(prio 0), tJolt=-10.
extern "C" void InitCm__FP2CM(CM *cm)
{
    static RGBA s_rgba(0x80303030);
    cm->nearFog.x = D_002BD81C;
    cm->nearFog.y = D_002BD820;
    cm->nearFog.z = D_002BD824;
    cm->aspect = 1.29870152f;
    SetNF(cm, 100.0f, 100000.0f);
    cm->cullScale = 1.0f;
    cm->fovCullRef = 1.0f;
    cm->targetFov = 1.0f;
    cm->fov = 1.0f;
    cm->fogColor = s_rgba;
    InitLm((LM *)&cm->fovSmoothing, 0.5f, 0.1f, 10.0f);
    float *pm = cm->mat;
    func_002333B4(pm, 0, 0x40);
    pm[15] = 1.0f;
    pm[10] = 1.0f;
    pm[5] = 1.0f;
    pm[0] = 1.0f;
    RecalcCmFrustrum__FP2CM(cm);
    SetCameraLookAt(cm, D_002BD830, D_002BD840, 0.0f, 0.0f);
    CutCm__FP2CM(cm);
    cm->policyCount = 0;
    InitCmpart1(cm->defaultPolicy);
    InitCptn(cm->followPolicy);
    InitCplcy(cm->lookPolicy);
    InitCplcy(cm->unknownPolicy4A8);
    InitCplcy(cm->hangPolicy);
    InitCplcy(cm->blendPolicy);
    InitCplcy(cm->zoomPolicy);
    InitCplcy(cm->unknown5F0);
    cm->visZone = 0;
    cm->visFlags = 0xFF;
    SetCmPolicy(cm, 0, cm->defaultPolicy, 0, 0);
    cm->joltKind = -1;
    cm->joltTime = -10.0f;
}
} // namespace M_InitCm__FP2CM

namespace M_CutCm__FP2CM { extern "C" {
struct CMC { char pad[0x300]; int cutCount; };
// Requests a camera cut (no smoothing) this frame: cutCount = max(cutCount, 1).
void CutCm__FP2CM(struct CMC *p)
{
    int n = p->cutCount;
    int m = 1;
    if (m < n) m = n;
    p->cutCount = m;
}
} } // namespace M_CutCm__FP2CM

namespace M_SetCmCut { extern "C" {
struct CMX { char pad[0x300]; int cutCount; float cutPan; float cutTilt; };
// Cut with explicit pan/tilt: radCutPan(+0x304)/radCutTilt(+0x308) = args, nCut = max(nCut,2) (inferred)
void SetCmCut(struct CMX *p, float a, float b)
{
    int m = 2;
    p->cutTilt = b;
    p->cutPan = a;
    if (m < p->cutCount) m = p->cutCount;
    p->cutCount = m;
}
} } // namespace M_SetCmCut

namespace M_CombineEyeLookAtProj { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct { float m[4][4]; } M4F;
typedef struct { qword r[4]; } M4;
void *func_002333B4(void *p, int c, int cb) __asm__("memset");
void InvertMatrix4Rigid(M4F *pmOut, M4F *pm);
void __ml__FRC7MATRIX4T0(M4 *pmOut, M4F *pa, M4F *pb);
// MatViewProj = proj * inverse(eye matrix with game->camera axis swap: x=-y, y=-z, z=x)
void CombineEyeLookAtProj(M4F *pm, M4F *pproj, M4 *pmOut)
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
} } // namespace M_CombineEyeLookAtProj

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

// Builds the four side planes of a view frustum from a camera matrix (row 0 forward, rows 1/2 the side axes) and the
// tangents of the half field of view gx, gy: each normal is the cross product of a side axis and the normalized edge
// direction. UpdateCmMat4 calls it for the real and the widened culling frustum.
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
static inline float sqrtf(float x)
{
    float r;
    __asm__ __volatile__("sqrt.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}
void BuildProj(void *p, float a, float b, float c, float d, float e, float f) __asm__("BuildSimpleProjectionMatrix__FffffffP7MATRIX4");
void SetCmLookAt(CM *p, float g, void *pv) __asm__("BuildCmFgfn");
void RecalcCm(CM *p) __asm__("UpdateCmMat4__FP2CM");
static inline float GMax(float a, float b) { return a > b ? a : b; }
// Recomputes everything that depends on the field of view: the half-fov tangents, the cull scale, the near-plane
// corner distance, the projection matrix (512x224 half-screen, near/far clip), the fog/LOD fit coefficients
// (BuildCmFgfn), then the view matrices (UpdateCmMat4).
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
MATRIX3 LoadRotateMatrix(qword vec) __asm__("BuildRotationMatrixFromVector");
MATRIX4 operator*(const MATRIX4 &matL, const MATRIX4 &matR) __asm__("__ml__FRC7MATRIX4T0");
bool FInvertMatrix4(MATRIX4 *pmatSrc, MATRIX4 *pmatDst) __asm__("InvertMatrix4");
void BuildSimpleProjectionMatrix(float rx, float ry, float dxOffset, float dyOffset, float sNear, float sFar, MATRIX4 *pmat) __asm__("BuildSimpleProjectionMatrix__FffffffP7MATRIX4");
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
extern "C" void CombineEyeLookAtProj(MATRIX4 *pmatEye, MATRIX4 *pmatProj, MATRIX4 *pmatOut) __asm__("CombineEyeLookAtProj");
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
// Recomputes matViewProj (+jolt shake for 0.5 s), its inverse, frustum side planes (normal and wide) and transposed
// VU0 cull rows, then UpdateCmVisZone.
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

namespace M_CompareCameraVisZones { extern "C" {
struct T { char pad[0xE4]; int unknownE4; char pad2[0x110 - 0xE8]; int priority; float distance; char pad3[0x120 - 0x118]; };
// Qsort comparator for vis-zone candidates: by +0x110 desc, +0xE4 desc, then +0x114 (distance) asc. (inferred)
int CompareCameraVisZones(struct T **ppa, struct T **ppb)
{
    struct T *a = *ppa;
    struct T *b = *ppb;
    int d = b->priority - a->priority;
    if (d) return d;
    d = b->unknownE4 - a->unknownE4;
    if (d) return d;
    if (a->distance < b->distance) return -1;
    if (b->distance < a->distance) return 1;
    return a - b;
}
} } // namespace M_CompareCameraVisZones

namespace M_UpdateCmVisZone__FP2CM { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x110]; int k; };
struct CM { char pad[0x90]; qword pos; };
extern char D_003DD490[];
int CploFindSwObjectsByClass(void *g, int a, int b, int c, int n, struct O **ap);
void UpdateVisInside(struct O *po, qword pos);
void qsort(void *base, int n, int cb, int (*cmp)(const void *, const void *));
int CompareCameraVisZones(const void *a, const void *b);
void SetCmVisZone(struct CM *p, struct O *po);
// Gathers up to 32 zone objects (0x15E9C8 query), scores each against cm pos (0x2273F0), sorts, and selects the best
// via SetCmVisZone if its kind==1.
void UpdateCmVisZone__FP2CM(struct CM *cm)
{
    struct O *apo[32];
    qword pos = cm->pos;
    int n = CploFindSwObjectsByClass(D_003DD490, 0x105, 5, 0, 0x20, apo);
    int i;
    for (i = 0; i < n; i++)
        UpdateVisInside(apo[i], pos);
    qsort(apo, n, 4, CompareCameraVisZones);
    if (n > 0 && apo[0]->k == 1)
        SetCmVisZone(cm, apo[0]);
}
} } // namespace M_UpdateCmVisZone__FP2CM

// If changed: pzonVis(+0x10)=pzon, grfvis(+0x14)=pzon->+0xE0. (inferred)
extern "C" void SetCmVisZone(CM *pcm, VISZONE *pzon)
{
    if (pzon != pcm->pzonVis) {
        pcm->pzonVis = pzon;
        pcm->grfvis = *(uint *)((char *)pzon + 0xE0);   /* the zone's visibility mask (VISZONE is opaque here) */
    }
}

// Copies a 4x4 placement into CM::mat (+0x60..0x9F) then UpdateCmMat4. (inferred)
extern "C" void SetCmMat4(CM *pcm, qword *pmat)
{
    ((qword *)&pcm->mat)[0] = pmat[0];
    ((qword *)&pcm->mat)[1] = pmat[1];
    ((qword *)&pcm->mat)[2] = pmat[2];
    ((qword *)&pcm->mat)[3] = pmat[3];
    UpdateCmMat4(pcm);
}

namespace M_SetCameraLookAt { extern "C" {
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
// Place camera at pos looking at target, with extra pan/tilt offsets: pan/tilt from (tgt-pos) (0x11EFB0), rotation
// 0x11B720, pos row, UpdateCmMat4. (inferred)
void SetCameraLookAt(struct CM *cm, qword pos, qword tgt, float da, float db)
{
    qword d;
    float a, b;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(tgt), "j"(pos));
    GetVectorSphere(d, &a, &b, 0);
    LoadM4(&cm->mat, LoadRotateMatrixPanTilt(a + da, b + db), pos);
    UpdateCmMat4__FP2CM(cm);
}
} } // namespace M_SetCameraLookAt

namespace M_BuildCmFgfn { extern "C" {
extern float D_002A4FF8;
void FitRecipFunction__FffffPfT4(float a, float b, float c, float d, float *pa, float *pb);
struct P { char pad[0x240]; float nearFog, farFog, maxFog; };
// Builds the GS fog function (FGFN) from sNearFog/sFarFog/uFogMax (+0x240..0x248): linear map to 0..4080, +8417280
// bias.
void BuildCmFgfn(struct P *p, float *af, float f)
{
    if (f == D_002A4FF8) f = 1.0f;
    FitRecipFunction__FffffPfT4(1.0f, 4080.0f, p->farFog / p->nearFog, (1.0f - p->maxFog * f) * 4080.0f, &af[0], &af[1]);
    af[0] += 8417280.0f;
    af[2] = p->nearFog;
    af[3] = af[0] + 8192.0f;
}
} } // namespace M_BuildCmFgfn

// 0x14E320: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("daddu $2, $0, $0");

namespace M_SetCmPolicy { extern "C" {
/* Insert an entry into a priority-sorted array of 8 (Claude agent). memmove goes through an inline helper: its
   parameters get their own pseudos, giving the target's (p + i*16) base copied into a1 then +0x320 / +0x310. */
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; int a; int b; };
struct P { char pad[0x30C]; int policyCount; struct E policies[8]; char pad2[0x390 - 0x390]; float modeStartTime; };
extern struct CLOCK D_002DDED0;
void *memmove(void *dst, void *src, int cb);
static inline void Move(void *pvDst, void *pvSrc, int cb) { memmove(pvDst, pvSrc, cb); }
// Insert {cpp,pcplcy,psoFocus,pv} into priority-sorted acpr[8] (+0x30C count); call pcplcy vt4 Set(pv); if new top:
// prev vt3 Deactivate, tModeStart=clock.t, vt2 Activate.
void SetCmPolicy(struct P *p, int prio, struct O *po, int a, int b)
{
    int i;
    if (p->policyCount >= sizeof(p->policies) / sizeof(p->policies[0]))
        return;     /* the stack is full */
    for (i = 0; i < p->policyCount; i++) {
        if (p->policies[i].prio < prio)
            break;
    }
    Move(&p->policies[i + 1], &p->policies[i], (p->policyCount - i) * sizeof(struct E));
    p->policies[i].prio = prio;
    p->policies[i].po = po;
    p->policies[i].a = a;
    p->policies[i].b = b;
    p->policyCount++;
    {
        struct VTE *e = &po->vt[4];
        ((void (*)(void *, int))e->pfn)((char *)po + e->d, b);
    }
    if (i == 0) {
        /* the new top: deactivate the previous top (slot 3), activate this one (slot 2) */
        if (p->policyCount >= 2) {
            struct O *o = p->policies[1].po;
            struct VTE *e = &o->vt[3];
            ((void (*)(void *, int, int))e->pfn)((char *)o + e->d, p->policies[1].a, p->policies[1].b);
        }
        p->modeStartTime = D_002DDED0.t;
        {
            struct VTE *e = &po->vt[2];
            ((void (*)(void *, int, int))e->pfn)((char *)po + e->d, a, b);
        }
    }
}
} } // namespace M_SetCmPolicy

namespace M_RevokeCmPolicy { extern "C" {
/* Matched by Claude agent (2026-10-17): compare against the parameters directly (no local E copy) and `for (i = 0, pe = p->policies; ...)`. */
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; int a; int b; };
struct P { char pad[0x30C]; int policyCount; struct E policies[8]; char pad2[0x390 - 0x390]; float modeStartTime; };
extern struct CLOCK D_002DDED0;
/* which fields of a stack entry a revoke must match */
enum { REVOKE_BY_PRIO = 1, REVOKE_BY_POLICY = 2, REVOKE_BY_FOCUS = 4, REVOKE_BY_PARAM = 8 };
// Removes the policy-stack entries that match on every field the mask selects (priority, policy, focus object,
// parameter), calling each one's Revoke (vtable slot 5); when the top entry went, deactivates it and activates the new
// top.
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
    peOut = p->policies;
    for (i = 0, pe = p->policies; i < p->policyCount; i++, pe++) {
        if ((!(grf & REVOKE_BY_PRIO) || prio == pe->prio) &&
            (!(grf & REVOKE_BY_POLICY) || po == pe->po) &&
            (!(grf & REVOKE_BY_FOCUS) || a == pe->a) &&
            (!(grf & REVOKE_BY_PARAM) || b == pe->b)) {
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
    p->policyCount = peOut - p->policies;
    if (poTop) {
        /* the top policy went away: deactivate it (slot 3) and activate the new top (slot 2) */
        struct VTE *v = &poTop->vt[3];
        ((void (*)(void *, int, int))v->pfn)((char *)poTop + v->d, aTop, bTop);
        if (p->policyCount > 0) {
            struct O *o;
            struct VTE *v;
            p->modeStartTime = D_002DDED0.t;
            o = p->policies[0].po;
            v = &o->vt[2];
            ((void (*)(void *, int, int))v->pfn)((char *)o + v->d, p->policies[0].a, p->policies[0].b);
        }
    }
}
} } // namespace M_RevokeCmPolicy

namespace M_CalcCmAdjust { extern "C" {
void SetVectorCylind(void *a, void *b, float x, float y, float z);
struct CPDEFIV { char pad[0x30]; float radHome; float sAdjust; };
// The camera adjust offset: a cylindrical vector pointing opposite the focus's home angle (radHome + pi) with length
// sAdjust (CPDEFI +0x30 / +0x34). (inferred)
void CalcCmAdjust(void *a, struct CPDEFIV *b)
{
    SetVectorCylind(a, b, b->radHome + 3.14159265f, b->sAdjust, 0.0f);
}
} } // namespace M_CalcCmAdjust

namespace M_ApplyCameraSpring { extern "C" {
// MATCH (Claude agent): hard-register VF locals; clock pointer pinned to v0 and the second dt read via a v0-pinned int.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct CLOCK { float t; union { float g; int n; } dt; };
extern struct CLOCK D_002DDED0;
struct P { char pad[0x300]; int cutCount; };
// Critically-damped spring of *ppos/ *pv toward target with (rSpring,rDamping) and clock dt; snaps and zeroes v when
// nCut.
void ApplyCameraSpring(struct P *p, qword *ppos, float *pvel, qword target, float k1, float k2)
{
    register struct CLOCK *pclock __asm__("v0");
    register qword s1 __asm__("vf3");
    register qword s2 __asm__("vf4");
    register qword s3 __asm__("vf5");
    register qword d __asm__("vf1");
    register qword v __asm__("vf2");
    if (p->cutCount) {
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
} } // namespace M_ApplyCameraSpring

namespace M_AdjustCmCpdefi { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x2C0]; qword adjustOffset; qword adjustVelocity; qword focusOffset; qword focusVelocity; };
struct O { char pad[0x4F4]; int stepMode; };
struct S { qword a; qword b; qword c[2]; qword d; };
extern qword D_002A2DD0;
qword CalcCmAdjust(struct P *p, struct S *ps);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int k);
void ApplyCameraSpring(struct P *p, qword *ppos, qword *pvel, qword target, float k1, float k2);
// Springs dposAdjust/dposFocus (+0x2C0/+0x2E0) toward CalcCmAdjust; per STEPPLAYER mode +0x4F4 (0/1/2) different
// gains; writes cpdefi.posAdjust = posBase + dposAdjust.
void AdjustCmCpdefi(struct P *p, struct O *po, struct S *ps)
{
    qword q = CalcCmAdjust(p, ps);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x34)) {
        switch (po->stepMode) {
        case 0:
            ApplyCameraSpring(p, &p->adjustOffset, &p->adjustVelocity, q, 5.0f, -3.5f);
            ApplyCameraSpring(p, &p->focusOffset, &p->focusVelocity, D_002A2DD0, 5.0f, -3.5f);
            break;
        case 1:
            ApplyCameraSpring(p, &p->adjustOffset, &p->adjustVelocity, q, 1.0f, -10.0f);
            ApplyCameraSpring(p, &p->focusOffset, &p->focusVelocity, p->focusOffset, 0.0f, -10.0f);
            break;
        case 2:
            ApplyCameraSpring(p, &p->adjustOffset, &p->adjustVelocity, q, 5.0f, -3.5f);
            break;
        }
    } else {
        ApplyCameraSpring(p, &p->adjustOffset, &p->adjustVelocity, q, 5.0f, -3.5f);
        ApplyCameraSpring(p, &p->focusOffset, &p->focusVelocity, D_002A2DD0, 5.0f, -3.5f);
    }
    {
        qword r;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(ps->b), "j"(p->adjustOffset));
        ps->d = r;
    }
}
} } // namespace M_AdjustCmCpdefi

static inline void ZeroVector(VECTOR *pv)
{
    pv->z = 0.0f;
    pv->y = 0.0f;
    pv->x = 0.0f;
}
// Resets the camera's adjust state: the adjust offset from CalcCmAdjust, and zero adjust / focus velocities.
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
// If pso == psoAdjust(+0x260) copy cached cpdefi (+0x270) else pso->vt50 GetCpdefi and posAdjust = posBase +
// CalcCmAdjust.
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

namespace M_FindCameraClearInterval { extern "C" {
/* float g placed after the pointer params: its param copy comes after pgMax, lengthening pgMax so the hoisted 0x40 wins $fp in global alloc */
typedef int qword __attribute__((mode(TI)));
struct T { int a, b, c; };
struct O { char pad[0x70]; int c; struct T *at; };
struct HIT { char pad[0x40]; float enterTime; float exitTime; char pad2[0x18]; };
int func_00118668(struct T *pBegin, struct T *pEnd, qword pos, qword dir, int c, struct HIT *ahit);
int MergeSegmentLists(int c, struct HIT *ahit, int cNew, struct HIT *ahitNew, int cMax, struct HIT *ahitOut, int f);
// Clips segment pos->dir against each SO's camera BSP (0x118668), merges intervals (0x14C9F8), returns first clear gap
// > duMin in *puFrom/ *puTo.
int FindCameraClearInterval(int n, struct O **apo, qword pos, qword dir, float *pgMin, float *pgMax, float g)
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
            c = MergeSegmentLists(c, ahit, cNew, &ahit[c], 0x40, ahit, 0);
    }
    for (j = 0; ; j++) {
        if (c < j)
            return 0;
        g0 = 0.0f;
        if (j > 0)
            g0 = ahit[j - 1].exitTime;
        if (j < c)
            g1 = ahit[j].enterTime;
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
} } // namespace M_FindCameraClearInterval

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A4FF8 __asm__("D_002A4FF8") = 3.40282347e+38f; }
namespace M_FindCmClosestClearPos {
typedef int qword __attribute__((mode(TI)));
extern "C" {
extern int D_002BD8B0[];
qword SetVectorSphere(float pan, float tilt, float s);
int FindCameraClearInterval(void *p, void *x, qword a, qword b, float *pt, float *pu, float g);
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

// Samples 9 tilts x D_002BD8B0[i] pans at radius g, padding 5/g, returns closest clear position.
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
            if (FindCameraClearInterval(p, x, pos, test, &t, &u, dg + dg)) {
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
struct X { qword pos; qword normal; qword unknown20; };
struct E { int a, b, c; };
struct LO { char pad[0x70]; int rootNodeIndex; struct E *bspNodes; };
struct AL { int cb; int cbAlign; };
extern float D_002BD8D4;
void STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
void *AllocFromStack(struct AL *pal);
int IntersectSphereBsp(qword pos, struct LO *po, struct E *pe, float g, int a, int b, int cMax, struct X *ax);
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
    return AllocFromStack(&al);
}

// Collects sphere-vs-BSP contacts (0x1A93C8) for the eye, builds n x n normal dot matrix + constraint values via pfn,
// solves (0x11AD30) and returns v + sum of normal impulses.
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
        n += IntersectSphereBsp(pos, po, &po->bspNodes[po->rootNodeIndex], g, 0, 0, 64 - n, &ax[n]);
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

struct LOBSP { char pad[0x70]; int numCameraBspNodes; };
// Camera object filter: accepts objects with camera-collision BSP nodes (a positive count at +0x70) (inferred).
extern "C" int HasCameraBsp(void *pv, LO *plo)
{
    return ((LOBSP *)plo)->numCameraBspNodes > 0;
}

namespace M_SvSquishCmEyeConstraint { extern "C" {
// MATCH (Claude agent, lane D24, 2026-10-23): v6 with the mtc1 asm's allocation-only "j" operands replaced by an
// "r,j" constraint on td (never-chosen alternative, forced off by an unused "i,r"(0) operand). With fewer operands
// the mtc1 has 4 dependents like the `one` sqc2, so the luid tie-break issues the sqc2 first, like the target.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct PL { qword pos; qword plane; };
struct P { char pad[0x48]; float nearCorner; };
extern float D_002BD8D8;
extern char D_002BD8E0[];
void *InitVuTemp(void *p);
void SmoothValue__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr);
static __inline__ float GDistPlane(qword plane, qword pos)
{
    U d, one;
    qword td, tone;
    float dist;
    InitVuTemp(&d);
    InitVuTemp(&one);
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
// Constraint for SquishCmEye: SmoothValue(dist-to-plane, sNearCorner(+0x48) - k) ; return dot(vEye, normal) - smoothed.
float SvSquishCmEyeConstraint(struct P *p, int unused, qword vIn, struct PL *pl)
{
    float r;
    qword plane = pl->plane;
    qword pos = pl->pos;
    SmoothValue__FfffP3SMPPf(GDistPlane(plane, pos), p->nearCorner - D_002BD8D8, g_clock.cameraDeltaTime, D_002BD8E0, &r);
    return GDot(&vIn, pl->plane) - r;
}
} } // namespace M_SvSquishCmEyeConstraint

namespace M_ClipCameraEye {
// ClipCameraEye (Claude agent, lane K5, 2026-10-24): MATCH. Built on laneK4v1 (K3/K4 fresh rewrite in the ClipEdgeToSphere style).
// Keys: the VU_FLOAT(const float *) ctor's word input has a never-chosen "V" alternative ("r,V"), so it stays a MEM until
// reload: the clock dt load is then an input reload (lw v0 for the vdiv; for the final AddScaled reload reuses the dying
// pclock register, lw s0,0x10(s0)), which also shortens the clock chain in sched1 so no extra operand is needed on the vdiv;
// v is computed before posMin so the spill slots come out v 0x30 / posMin 0x40. No register pins.
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(*(const int *)pg)); } /* never-chosen "V" (non-offsettable mem) alternative: allocation-only (the word stays a MEM until reload) */ };
struct CLOCK { float t, dt, unknown8, unknownC, dtPrev; };
extern "C" {
extern CLOCK D_002DDED0;
extern qword D_002BD8F0, D_002BD900;
extern float D_002BD8D8, D_002BD8DC;
extern char D_003DD490[];
void *STACKSCOPE_ctor(STACKSCOPE *p);
void STACKSCOPE_ctor_1AE4A0(STACKSCOPE *p, int n) __asm__("_$_10STACKSCOPE");
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj);
qword SquishCmEye(void *p, int i, int j, void *pfn, qword a, qword v, float g, float h);
void HasCameraBsp(void);
void SvSquishCmEyeConstraint(void);
qword ClipCameraEye(void *p, qword pos, qword posTarget);
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

// VEye=(next-prev)/dtPrev; gather camera-colliding SOs in a box around prev; SquishCmEye; return prev + vClip*dt.
qword ClipCameraEye(void *p, qword pos, qword posTarget)
{
    CLOCK *pclock = &D_002DDED0;
    int ii, jj;
    qword dpos = Sub(posTarget, pos);
    qword v = DivideBy(dpos, VU_FLOAT(&pclock->dtPrev));
    qword posMin = Add(pos, D_002BD8F0);
    qword posMax = Add(pos, D_002BD900);
    STACKSCOPE frame;
    IntersectSwBoundingBox(D_003DD490, posMin, posMax, (void *)HasCameraBsp, 0, &ii, &jj);
    qword w = SquishCmEye(p, ii, jj, (void *)SvSquishCmEyeConstraint, pos, v, D_002BD8D8, Length(v) + D_002BD8DC);
    qword r = AddScaled(pos, w, VU_FLOAT(&pclock->dtPrev));
    return r;
}
} // namespace M_ClipCameraEye

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
// Screen (x,y,depth) -> world: axis swap, scale by tanHalfX/Y (+0x40/+0x44), multiply by CM::mat.
qword ConvertCmScreenToWorld(struct CM *cm, qword v)
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
    r.f[1] *= cm->sx;
    r.f[2] *= cm->sy;
    return Mul(&cm->mat, r.q);
}
} } // namespace M_ConvertCmScreenToWorld

namespace M_ConvertCmWorldToScreen { extern "C" {

typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x40]; float sx, sy; char pad2[0x18]; M4 mat; };
void InvertMatrix4Rigid(M4 *pm, M4 *pn);
// World -> screen: inverse(CM::mat) * pos, axis swap, divide by tanHalf and depth (if > 1e-4)
qword ConvertCmWorldToScreen(struct CM *cm, qword v)
{
    union { qword q; float f[4]; } r;
    M4 m;
    float x, y, z;
    InvertMatrix4Rigid(&m, &cm->mat);
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
    r.f[0] = -y / cm->sx;
    r.f[1] = z / cm->sy;
    if (x > 1e-4f) {
        r.f[0] = r.f[0] / x;
        r.f[1] = r.f[1] / x;
    }
    return r.q;
}
} } // namespace M_ConvertCmWorldToScreen

// 0x14F3C0: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x40");

namespace M_SetCmPos { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { struct { qword r[4]; } q; float af[4][4]; } M4;
struct CM { char pad[0x60]; M4 mat; };
qword *VECTOR_opAssign(qword *p, qword v);
void SetCmMat4(struct CM *p, M4 *m);
// Option setter (g_aeopid[171], otyp 2 vector): keep rotation, set CM::mat pos row, SetCmMat4.
void SetCmPos(struct CM *cm, qword pos)
{
    M4 m = cm->mat;
    VECTOR_opAssign(&m.q.r[3], pos);
    m.af[3][3] = 1.0f;
    SetCmMat4(cm, &m);
}
} } // namespace M_SetCmPos

namespace M_SetCmMat { extern "C" {
#include "mat.h"
struct CM { char pad[0x90]; struct VECTOR pos; };
void SetCmMat4(struct CM *p, struct MATRIX4 *m);
// Set rotation from MATRIX3, keep pos, SetCmMat4. (inferred)
void SetCmMat(struct CM *cm, struct MATRIX3 *pmat)
{
    struct MATRIX4 m;
    *(struct MATRIX3 *)&m = *pmat;
    *(struct VECTOR *)m.mat[3] = cm->pos;
    m.mat[2][3] = 0.0f;
    m.mat[1][3] = 0.0f;
    m.mat[0][3] = 0.0f;
    SetCmMat4(cm, &m);
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
qword DecomposeRotationMatrixToEuler(void *p);
// Option getter (g_aeopid[173]): euler angles of CM::mat (0x11BFF0) in degrees.
#define DEGREES_PER_RADIAN 57.295776f
struct CMR { char pad[0x60]; char mat[0x40]; };   /* CM::mat */
qword GetCmRotDeg(struct CMR *p)
{
    union { qword q; float f; } r;
    qword v = DecomposeRotationMatrixToEuler(p->mat);
    r.q = Make(GetX(v) * DEGREES_PER_RADIAN, GetY(v) * DEGREES_PER_RADIAN, GetZ(v) * DEGREES_PER_RADIAN);
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
// Option setter (g_aeopid[173]): degrees -> radians, rotation matrix 0x11B628, keep pos, SetCmMat4. (inferred)
void SetCmRotDeg(struct CM *cm, qword v)
{
    M4 mat = cm->mat;
    union { qword q; float f; } r;
    M3 m;
#define RADIANS_PER_DEGREE 0.017453294f
    r.q = Make(GetX(v) * RADIANS_PER_DEGREE, GetY(v) * RADIANS_PER_DEGREE, GetZ(v) * RADIANS_PER_DEGREE);
    LoadRotateMatrixEuler(&m, r.q);
    mat.m3 = m;
    mat.af[2][3] = 0.0f;
    mat.af[1][3] = 0.0f;
    mat.af[0][3] = 0.0f;
    SetCmMat4(cm, &mat);
}
} } // namespace M_SetCmRotDeg

namespace M_SetCmFov__FP2CMf { extern "C" {
struct CM { char pad[0x20]; float fov, targetFov; int fovSpeed; };
void RecalcCmFrustrum__FP2CM(struct CM *p);
// Fov(+0x20)=fovTarget(+0x24)=f, dfov=0; RecalcCmFrustrum if changed > 1e-4.
void SetCmFov__FP2CMf(struct CM *cm, float f)
{
    int fChanged = __builtin_fabsf(f - cm->fov) > 0.0001f;
    cm->fov = f;
    cm->targetFov = f;
    cm->fovSpeed = 0;
    if (fChanged) RecalcCmFrustrum__FP2CM(cm);
}
} } // namespace M_SetCmFov__FP2CMf

// Option setter g_aeopid[162] (field +0x24): fovTarget=f and the active policy's saved fov (acpr[0].pcplcy+4) so
// Deactivate restores it. (inferred)
extern "C" void SetCmBaseFovTarget(CM *pcm, float fovTarget)
{
    CPLCY *pcplcy = pcm->acpr[0].pcplcy;
    pcm->fovTarget = fovTarget;
    if (pcplcy != 0) {
        pcplcy->gFovTargetSaved = fovTarget;
    }
}

// SNearClip(+0x4C)=f; RecalcCmFrustrum.
void SetCmNearClip(CM *pcm, float sNearClip)
{
    pcm->sNearClip = sNearClip;
    RecalcCmFrustrum(pcm);
}

// Option setter g_aeopid[164]: SetCmNearClip and the active policy's saved near clip (pcplcy+8) (inferred)
extern "C" void SetCmBaseNearClip(CM *pcm, float sNearClip)
{
    CPLCY *pcplcy;
    SetCmNearClip(pcm, sNearClip);
    pcplcy = pcm->acpr[0].pcplcy;
    if (pcplcy != 0) {
        pcplcy->sNearClipSaved = sNearClip;
    }
}

// SFarClip(+0x50)=f; RecalcCmFrustrum.
void SetCmFarClip(CM *pcm, float g)
{
    pcm->sFarClip = g;
    RecalcCmFrustrum(pcm);
}

// SNearFog(+0x240)=f; RecalcCmFrustrum.
void SetCmNearFog(CM *pcm, float g)
{
    pcm->sNearFog = g;
    RecalcCmFrustrum(pcm);
}

// SFarFog(+0x244)=f; RecalcCmFrustrum.
void SetCmFarFog(CM *pcm, float g)
{
    pcm->sFarFog = g;
    RecalcCmFrustrum(pcm);
}

// UFogMax(+0x248)=f; RecalcCmFrustrum.
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
// RgbaFog(+0x24C)=*prgba; 0x1ED278 on it; RecalcCmFrustrum.
void SetCmRgbaFog__FP2CMP4RGBA(struct CM *cm, struct S4 *s)
{
    cm->s = *s;
    ColorCorrectRgba(&cm->s);
    RecalcCmFrustrum__FP2CM(cm);
}
} } // namespace M_SetCmRgbaFog__FP2CMP4RGBA

// RMRD ratio (+0x18)=f; RecalcCmFrustrum (which derives +0x1C = max(r/fov,1)) (inferred)
void SetCmMrdRatio(CM *pcm, float g)
{
    pcm->rFovCullRef = g;
    RecalcCmFrustrum(pcm);
}

namespace M_JoltCamera { extern "C" {
extern float D_002DDED0;
struct CMJ { char pad[0x618]; float joltTime; int joltKind; };
// Starts a camera shake of the given kind now (joltTime, +0x618; joltKind, +0x61C); UpdateCmMat4 applies it for 0.5 s.
void JoltCamera(struct CMJ *p, int a)
{
    p->joltTime = D_002DDED0;
    p->joltKind = a;
}
} } // namespace M_JoltCamera

namespace M_BuildStereoEyeViewProj { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x60]; M3 mat; qword pos; char pad2[0x100 - 0xA0]; char projection[0x520]; float stereoEyeOffset; };
void VECTOR_opAssign(qword *p, qword v);
void CombineEyeLookAtProj(M4 *pm, void *pv, int n);
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
// Builds a view-proj from CM::mat with pos offset along the right axis by +/- rUnk620 (k=1/2) via CombineEyeLookAtProj
// into pm (proj = pv or matProj) (inferred)
void BuildStereoEyeViewProj(struct CM *cm, int k, void *pv, int n, M4 *pm)
{
    U r, g;
    float f = 0.0f;
    switch (k) {
    case 1: f = cm->stereoEyeOffset; break;
    case 2: f = -cm->stereoEyeOffset; break;
    }
    {
        qword qwgqOut;
        __asm__("qmtc2.ni %1, %0" : "=j"(qwgqOut) : "r"(f));
        g.q = qwgqOut;
    }
    {
        qword qwrqOut;
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwrqOut) : "j"(cm->pos), "j"(cm->mat.r[1]), "j"(g.q));
        r.q = qwrqOut;
    }
    VECTOR_opAssign(&pm->r[3], r.q);
    LoadM4(pm, &cm->mat);
    CombineEyeLookAtProj(pm, pv ? pv : cm->projection, n);
}
} } // namespace M_BuildStereoEyeViewProj

namespace M_IsInCameraFrustum { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct X { char pad[0xA0]; qword r[3]; };
// True if point/sphere is inside all 4 frustum side planes using the transposed cull rows (+0xA0) and VU clip flags.
// (inferred)
int IsInCameraFrustum(struct X *p, qword v, qword w)
{
    qword one;
    int f;
    __asm__("vmaxw.xyzw %0, $vf0, $vf0w" : "=j"(one));
    __asm__("vmulax.xyzw $ACC, %1, %4x\n\tvmadday.xyzw $ACC, %2, %4y\n\tvmaddaz.xyzw $ACC, %3, %4z\n\tvmsubax.xyzw $ACC, %5, %6x\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tcfc2.ni %0, $vi17"
            : "=r"(f) : "j"(p->r[0]), "j"(p->r[1]), "j"(p->r[2]), "j"(v), "j"(one), "j"(w));
    return (f ^ 0xF0) == 0;
}
} } // namespace M_IsInCameraFrustum

namespace M_IsWithinCameraRenderDistance { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct P { int pad[7]; float cullScale; };
// True if |dpos| < s*rCullScale; *pu = fade fraction between outer radius and (r-400*rCullScale) (inferred)
int IsWithinCameraRenderDistance(struct P *p, qword v, float g, float *pu)
{
    qword l, one;
    float l2, r, r2, d2;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(v));
    r = g * p->cullScale;
    __asm__("qmfc2.ni %0, %1" : "=r"(l2) : "j"(l));
    r2 = r * r;
    if (r2 < l2)
        return 0;
    d2 = r - p->cullScale * 400.0f;
    d2 = d2 * d2;
    if (d2 < l2)
        *pu = (r2 - l2) / (r2 - d2);
    else
        *pu = 1.0f;
    return 1;
}
} } // namespace M_IsWithinCameraRenderDistance


/* 0x256278 (the matrix option's setter, forwarding to SetCmMat): an inline function in sly2/cm.h. */

namespace M_DestroyCm { extern "C" {
typedef struct { int pad[4]; short d; short p; void (*f)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;   /* a reference-counted SND; the called vtable entry (+0x10) is its release (inferred) */
/* CM: the CRefPtr<SND> members of its embedded look (+0x450) and zoom (+0x520) policies (sly2/cm.h's CPLOOK::psndZoom /
   psndUnk38 and CPZOOM::psndZoom / psndUnkc4) */
typedef struct { char pad[0x484]; O *lookZoomSound; O *lookSound2; char pad2[0x154]; O *zoomZoomSound; O *zoomSound2; } S;
void DestroyBasic(S *, int);
// The CM destructor (an inline, emitted linkonce): releases the policies' four sound references in reverse member
// order, then runs the base destructor (func_00258348, shared with SW and the screens) with the in-charge flag.
inline void DestroyCm(S *p, int x)
{
    if (p->zoomSound2) p->zoomSound2->vt->f((char *)p->zoomSound2 + p->zoomSound2->vt->d);
    if (p->zoomZoomSound) p->zoomZoomSound->vt->f((char *)p->zoomZoomSound + p->zoomZoomSound->vt->d);
    if (p->lookSound2) p->lookSound2->vt->f((char *)p->lookSound2 + p->lookSound2->vt->d);
    if (p->lookZoomSound) p->lookZoomSound->vt->f((char *)p->lookZoomSound + p->lookZoomSound->vt->d);
    DestroyBasic(p, x);
}
} } // namespace M_DestroyCm

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const float D_002A4FF8;   /* defined below where the ELF has it (.rodata order) */













  /* smoothing weights */

// ---- cm's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
SMP D_002BD810 = {20.0f, 10.0f, 0.5f};
float D_002BD81C = 500.0f;
float D_002BD820 = 20000.0f;
float D_002BD824 = 0.5f;
VECTOR4 D_002BD830 = {0.0f, -2000.0f, 500.0f, 0.0f};
VECTOR4 D_002BD840 = {0.0f, 0.0f, 0.0f, 0.0f};
float D_002BD850 = 0.5f;
float D_002BD854 = 50.0f;
CLQ D_002BD860 __attribute__((aligned(16))) = {{{1.0f, -2.0f, 1.0f, 0.0f}}};
VECTOR4 D_002BD870 = {0.0f, 0.0f, 0.0f, 0.0f};
VECTOR4 D_002BD880 = {0.05f, 0.0f, 0.0f, 0.0f};
VECTOR4 D_002BD890 = {0.0f, 0.0f, 0.0f, 0.0f};
VECTOR4 D_002BD8A0 = {0.0f, 0.0f, 0.0f, 0.0f};
int D_002BD8B0[9] = {1, 5, 10, 12, 15, 12, 10, 5, 1};
float D_002BD8D4 = 4.0f;
float D_002BD8D8 = 125.0f;
float D_002BD8DC = 1500.0f;
SMPA D_002BD8E0 = {2500.0f, 0.0f, 0.25f, 0.0f};
VECTOR4 D_002BD8F0 = {-250.0f, -250.0f, -250.0f, 0.0f};
VECTOR4 D_002BD900 = {250.0f, 250.0f, 250.0f, 0.0f};
