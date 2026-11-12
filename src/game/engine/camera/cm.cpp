// cm.cpp -- the camera (CM, the global g_cm at g_cm): construction with its embedded policies, setup, the
// view matrix / frustum / fog setters, the visibility zone, adjustment and cuts, the policy stack, the camera
// filters. TU 0x14D3D8..0x14FAC8 (53 functions, address order), then ~CM (_$_2CM, the linkonce copy 0x258290,
// defined over a view at the end of the file); the matrix setter 0x256278 is an inline function in sly2/cm.h.
//
// Types: sly2/cm.h's CM and policy classes for the m2c bodies; the hand-written per-function files keep their views
// in namespaces named after each function (as jt.cpp does), and their C++-named definitions are C-linkage
// definitions of their literal symbols there (the namespace would change the mangling), as in wave 9.
// g_cm is defined here: its constructor (CM_ctor) and destructor (_$_2CM, ~CM with g++'s in-charge flag 2)
// make the static initializer a constructor-and-destructor one (86 instructions) with a _GLOBAL_$D$ stub
// (GlobalDestructors_cm) next to the _GLOBAL_$I$ one.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_cm
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_cm
// SLY2_ALIAS: _GLOBAL_$D$*=GlobalDestructors_cm

#include "sly2/basic.h"
#include "sly2/scrap.h"
#include "sly2/cm.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/sw.h"
#include "sly2/clock.h"
#include "sly2/joy.h"
#include "sly2/vtables.h"
#include "sly2/port.h"   /* SLY2_EE_LAYOUT */
#if defined(SLY2_EE_LAYOUT)
#else
#include "sly2/so.h"   /* SO::GetCpdefi; the EE build calls it by hand (including so.h here renumbers s_rgba below) */
#endif

/* g_cm: the camera, constructed by CM_ctor and destroyed by ~CM (_$_2CM, defined at the end of the file). */
CM g_cm;

// 0x14F930 StaticInit_cm (g_cm's construction and destruction, then the six slot pools) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

extern "C" {
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

// CM constructor: BASIC header, vptr 0x2CA4D0 (CM vtable), then constructs the 9 embedded camera policies at
// +0x3A0..+0x600.
// Also known as: CM constructor (upstream readable / Sly 1 name).
extern "C" CM *CM_ctor(CM *pcm)
{
    CPTWOSHOT *pcptwoshot = &pcm->twoShotPolicy;
    pcm->scriptFrame.m_p = 0;
    pcm->scriptVariables.m_p = 0;
    SLY2_SET_VPTR(pcm, CM);
    CMPART1_ctor(&pcm->part3a0);
    CPTN_ctor(&pcm->tunnelPolicy);
    CPLOOK_ctor(&pcm->cplook);
    CPCAMERA_ctor(&pcm->fixedCameraPolicy);
    CPHANG_ctor(&pcm->hangPolicy);
    CPCAMBLEND_ctor(&pcm->cpcamblend);
    CPZOOM_ctor(&pcm->cpzoom);
    CPTURRET_ctor(&pcm->turretPolicy);
    CPLCY_ctor(pcptwoshot);
    SLY2_SET_VPTR(pcptwoshot, CPTWOSHOT);
    return pcm;
}

// Camera setup hook called once at startup by main (after the CM constructor has run); empty in the shipped game.
// Also known as: StartupCm (upstream readable / Sly 1 name; StartupCm__Fv).
void SetupCm(CM *pcm)
{
}

namespace M_UpdateCmLast__FP2CM {
#if defined(SLY2_EE_LAYOUT)
/* The focus SO's vtable, called by hand (slot 50, GetCpdefi; so.h is not included here). */
struct O { int pad[3]; VTE *vt; };
#endif
extern int g_grfjoyt __asm__("g_grfjoyt");
extern JOY D_002E0D00 __asm__("D_002E0D00");
extern char D_002BD810[] __asm__("D_002BD810");
void ResetCmAdjust(CM *p, CPDEFI *pcpdefi) __asm__("ResetCmAdjust");
void AdjustCmCpdefi(CM *p, SO *pso, CPDEFI *pcpdefi) __asm__("AdjustCmCpdefi");
void SetCmFov__FP2CMf(CM *p, float g) __asm__("SetCmFov__FP2CMf");
float SmoothValueWithAccel__FffffP4SMPAPf(float g, float h, float i, float dt, SMPA *psmpa, float *pb) __asm__("SmoothValueWithAccel__FffffP4SMPAPf");
float SmoothValue__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr) __asm__("SmoothValue__FfffP3SMPPf");
// Per-frame camera update: runs the top policy of the stack (vtable slot 6 Update, with the focus object's CPDEFI
// from its vtable slot 50 and the current joypad), again whenever the update replaced the top policy; then the fov:
// snapped on a cut, else smoothed toward the target; clears the cut and, in stereo mode, smooths the eye offset.
// Also known as: UpdateCm (upstream readable / Sly 1 name).
extern "C" void UpdateCmLast__FP2CM(CM *cm)
{
    CPLCY *policy;
    CPLCY *lastUpdated;
    SO *lastFocus;
    if (cm->numPolicies > 0) {
        lastUpdated = 0;
        lastFocus = 0;
        while ((policy = cm->policyStack[0].policy) != lastUpdated) {
            SO *focus = cm->policyStack[0].focus;
            CPDEFI *cpdefi;
            if (focus) {
                cpdefi = &cm->cpdefi;
                if (focus != lastFocus) {
#if defined(SLY2_EE_LAYOUT)
                    VTE *e = &((struct O *)focus)->vt[50];   /* SO::GetCpdefi; so.h is not included this early (it renumbers s_rgba below) */
                    ((void (*)(void *, ...))e->pfn)((char *)focus + e->d, cpdefi);
#else
                    focus->GetCpdefi(cpdefi);
#endif
                    if (lastUpdated || focus != cm->focus)
                        ResetCmAdjust(cm, cpdefi);
                    AdjustCmCpdefi(cm, focus, cpdefi);
                    cm->focus = focus;
                    lastFocus = focus;
                }
            } else {
                cm->focus = 0;
                cpdefi = 0;
                lastFocus = 0;
            }
            policy->Update(cpdefi, (g_grfjoyt & 1) ? &g_joy : &D_002E0D00);
            lastUpdated = policy;
            if (cm->numPolicies <= 0)
                break;
        }
    }
    if (cm->nCut) {
        SetCmFov__FP2CMf(cm, cm->targetFieldOfView);
    } else if (cm->targetFieldOfView != cm->fieldOfView) {
        cm->fieldOfView = SmoothValueWithAccel__FffffP4SMPAPf(cm->fieldOfView, cm->dfov, cm->targetFieldOfView, g_clock.deltaTime, &cm->fovSmoothing, &cm->dfov);
        RecalcCmFrustrum(cm);
    }
    cm->nCut = 0;
    if (g_sw.unknown3E38)   /* stereo mode (draw.cpp) */
        cm->stereoEyeOffset = SmoothValue__FfffP3SMPPf(cm->stereoEyeOffset, cm->stereoFocusDistance, g_clock.cameraDeltaTime, D_002BD810, 0);
}
} // namespace M_UpdateCmLast__FP2CM

namespace M_ResetCameraPolicies {
// Calls ResetPolicy (vtable slot 7) on every policy on the stack, then resets the stereo eye offset (+0x620: 0) and
// focus distance (+0x624: 10).
// Also known as: ResetCm (upstream readable / Sly 1 name; ResetCm__FP2CM).
extern "C" void ResetCameraPolicies(CM *p)
{
    int i;
    for (i = 0; i < p->numPolicies; i++)
        p->policyStack[i].policy->ResetPolicy();
    p->stereoEyeOffset = 0.0f;
    p->stereoFocusDistance = 10.0f;
}
} // namespace M_ResetCameraPolicies

namespace M_InitCm__FP2CM {
// InitCm: near/far and LM init via inline helpers (parameter pseudos give the original FP register choice).


extern float D_002BD81C __asm__("D_002BD81C"), D_002BD820 __asm__("D_002BD820"), D_002BD824 __asm__("D_002BD824");
extern qword g_posEyeDefault __asm__("g_posEyeDefault");
extern qword g_posCenterDefault __asm__("g_posCenterDefault");
void *func_002333B4(void *pv, int b, int cb) __asm__("memset");
void SetCameraLookAt(CM *p, qword pos, qword tgt, float da, float db) __asm__("SetCameraLookAt");
void CutCm__FP2CM(CM *p) __asm__("CutCm__FP2CM");
void InitCmpart1(void *p) __asm__("InitCmpart1");
void InitCptn(void *p) __asm__("InitCptn");
void InitCplcy(void *p) __asm__("InitCplcy");
void SetCmPolicy(CM *p, int prio, void *po, int a, int b) __asm__("SetCmPolicy");

// Set the camera's near and far clip distances.
static inline void SetNF(CM *cm, float n, float f) { cm->nearClip = n; cm->farClip = f; }
// Initialise a smoothing record: fast speed, fast duration and max speed change; slow speed 0.
static inline void InitSmpa(SMPA *psmpa, float g0, float g8, float gC) { psmpa->fastSpeed = g0; psmpa->maxSpeedChange = gC; psmpa->fastDuration = g8; psmpa->slowSpeed = 0.0f; }
// Camera init: fogColor fog 0x80303030, aspect 1.2987, near/far 100/100000, identity mat, RecalcCmFrustrum, SetCmLookAt,
// CutCm, Init on each embedded policy, push default policy (+0x3A0) with SetCmPolicy(prio 0), joltTime=-10.
extern "C" void InitCm__FP2CM(CM *cm)
{
    static RGBA s_rgba(0x80303030);
    cm->nearFogDistance = D_002BD81C;
    cm->farFogDistance = D_002BD820;
    cm->maxFog = D_002BD824;
    cm->aspectRatio = 1.29870152f;
    SetNF(cm, 100.0f, 100000.0f);
    cm->rCullScale = 1.0f;
    cm->rFovCullRef = 1.0f;
    cm->targetFieldOfView = 1.0f;
    cm->fieldOfView = 1.0f;
    *(RGBA *)&cm->rgbaFog = s_rgba;
    InitSmpa(&cm->fovSmoothing, 0.5f, 0.1f, 10.0f);
    float *pm = &cm->mat.mat[0][0];
    func_002333B4(pm, 0, 0x40);
    pm[15] = 1.0f;
    pm[10] = 1.0f;
    pm[5] = 1.0f;
    pm[0] = 1.0f;
    RecalcCmFrustrum(cm);
    SetCameraLookAt(cm, g_posEyeDefault, g_posCenterDefault, 0.0f, 0.0f);
    CutCm__FP2CM(cm);
    cm->numPolicies = 0;
    InitCmpart1(&cm->part3a0);
    InitCptn(&cm->tunnelPolicy);
    InitCplcy(&cm->cplook);
    InitCplcy(&cm->fixedCameraPolicy);
    InitCplcy(&cm->hangPolicy);
    InitCplcy(&cm->cpcamblend);
    InitCplcy(&cm->cpzoom);
    InitCplcy(&cm->turretPolicy);
    cm->pzonVis = 0;
    cm->grfvis = 0xFF;
    SetCmPolicy(cm, 0, &cm->part3a0, 0, 0);
    cm->joltKind = -1;
    cm->joltTime = -10.0f;
}
} // namespace M_InitCm__FP2CM

namespace M_CutCm__FP2CM {
// Requests a camera cut (no smoothing) this frame: cutCount = max(cutCount, 1).
extern "C" void CutCm__FP2CM(CM *p)
{
    int n = p->nCut;
    int m = 1;
    if (m < n) m = n;
    p->nCut = m;
}
} // namespace M_CutCm__FP2CM

namespace M_SetCmCut {
// Cut with explicit pan/tilt: cutPanAngle(+0x304)/cutTiltAngle(+0x308) = args, nCut = max(nCut,2) (inferred)
// Also known as: SetCmCut (upstream readable / Sly 1 name; SetCmCut__FP2CMff).
extern "C" void SetCmCut(CM *p, float a, float b)
{
    int m = 2;
    p->cutTiltAngle = b;
    p->cutPanAngle = a;
    if (m < p->nCut) m = p->nCut;
    p->nCut = m;
}
} // namespace M_SetCmCut

namespace M_CombineEyeLookAtProj {
typedef struct { float m[4][4]; } M4F;
typedef struct { qword r[4]; } M4;
void *func_002333B4(void *p, int c, int cb) __asm__("memset");
void InvertMatrix4Rigid(M4F *pmOut, M4F *pm) __asm__("InvertMatrix4Rigid");
void __ml__FRC7MATRIX4T0(M4 *pmOut, M4F *pa, M4F *pb) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
// MatViewProj = proj * inverse(eye matrix with game->camera axis swap: x=-y, y=-z, z=x)
// Also known as: CombineEyeLookAtProj (upstream readable / Sly 1 name; CombineEyeLookAtProj__FP7MATRIX4T0Pv).
extern "C" void CombineEyeLookAtProj(M4F *pm, M4F *pproj, M4 *pmOut)
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
} // namespace M_CombineEyeLookAtProj

namespace M_BuildFrustrum {
// BuildFrustrum(pmat, gx, gy, anormal): 4 side-plane normals. Hard-reg VF vars in Normalize1 pin the first block allocation.
struct MATRIX3 { qword r[3]; };
extern void BuildFrustrum(MATRIX3 *pmat, float gx, float gy, qword *anormal) __asm__("BuildFrustrum");
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };

// Return a + b * s.
static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}
// Unit vector along v.
static inline qword Normalize(qword v)
{
    qword t, one, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=&j"(t), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(r) : "j"(t), "j"(v));
    return r;
}

// Unit vector along v (register-pinned to vf2).
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
    anormal[0] = VuCross(pmat->r[2], Normalize1(AddScaled(pmat->r[0], pmat->r[1], VU_FLOAT(gx))));
    anormal[1] = VuCross(Normalize(AddScaled(pmat->r[0], pmat->r[1], VU_FLOAT(-gx))), pmat->r[2]);
    anormal[2] = VuCross(Normalize(AddScaled(pmat->r[0], pmat->r[2], VU_FLOAT(gy))), pmat->r[1]);
    anormal[3] = VuCross(pmat->r[1], Normalize(AddScaled(pmat->r[0], pmat->r[2], VU_FLOAT(-gy))));
}
} // namespace M_BuildFrustrum

namespace M_RecalcCmFrustrum__FP2CM {
#include "sly2/cm.h"
// sqrt.s as volatile inline asm (as in other matched cm/util code): it orders the stores like the original.
extern float tanf(float) __asm__("tanf");
void BuildProj(void *p, float a, float b, float c, float d, float e, float f) __asm__("BuildSimpleProjectionMatrix__FffffffP7MATRIX4");
void SetCmLookAt(CM *p, float g, void *pv) __asm__("BuildCmFgfn");
void RecalcCm(CM *p) __asm__("UpdateCmMat4__FP2CM");
// Recomputes everything that depends on the field of view: the half-fov tangents, the cull scale, the near-plane
// corner distance, the projection matrix (512x224 half-screen, near/far clip), the fog/LOD fit coefficients
// (BuildCmFgfn), then the view matrices (UpdateCmMat4).
extern "C" void RecalcCmFrustrum__FP2CM(CM *pcm)
{
    float t = tanf(pcm->fieldOfView * 0.5f);
    float r = 1.0f / pcm->fieldOfView;
    float c = pcm->aspectRatio;
    pcm->tanHalfFovY = t;
    pcm->tanHalfFovX = t * c;
    pcm->rCullScale = Max(pcm->rFovCullRef * r, 1.0f);
    pcm->nearCornerDistance = t * SqrtFloat(c * c + 1.0f) * pcm->nearClip + 1.0f;
    BuildProj(&pcm->matProj, 512.0f / (pcm->tanHalfFovX * 4096.0f), 224.0f / (pcm->tanHalfFovY * 4096.0f), 0.0f, 0.0f, pcm->nearClip, pcm->farClip);
    SetCmLookAt(pcm, 1.0f, pcm->agRecipFit);
    RecalcCm(pcm);
}
} // namespace M_RecalcCmFrustrum__FP2CM

namespace M_UpdateCmMat4__FP2CM {
// UpdateCmMat4: ported from Lilly's personal Sly 2 decomp (src/P2/cm.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DE0 (D_002A2DE0), DT_CmJoltMax, SW_CmJolt, g_clock, g_lmZeroOne, s_clqUToUJolt, s_dposJolt, s_rotJolt, s_vecOffset, s_vecRange
// 0x0014DC58 UpdateCmMat4__FP2CM  (T1386; Sly 1 never matched it)
// The camera's placement CM::mat (3 rotation rows, row 3 = the position; SetCmMat4 / SetCmPanTilt / the policies set it) or its projection changed:
//  1. the world -> clip matrix matViewProj = CombineEyeLookAtProj(eye, matProj), where eye is CM::mat itself, or, for 0.5 s after a camera jolt
//     (joltTime, set by 0x14F750), CM::mat times a small shake matrix (rotation about x of 0.05 rad * jolt, a position kick) and a projection whose scale
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
extern LM g_lmZeroOne __asm__("g_lmZeroOne");
extern void CombineEyeLookAtProj(MATRIX4 *pmatEye, MATRIX4 *pmatProj, MATRIX4 *pmatOut) __asm__("CombineEyeLookAtProj");
extern void BuildFrustrum(float gTanX, float gTanY, MATRIX4 *pmat, qword *paplane) __asm__("BuildFrustrum");
void UpdateCmVisZone(CM *pcm) __asm__("UpdateCmVisZone__FP2CM");
struct VU_FLOAT
{
    qword qw;
};
// Return qwBase + qwD * g.
static inline qword QwAddScaled(qword qwBase, qword qwD, VU_FLOAT g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwD), "j"(g.qw));
    return qw;
}
extern float DT_CmJoltMax __asm__("DT_CmJoltMax");
extern float SW_CmJolt __asm__("SW_CmJolt");
extern CLQ s_clqUToUJolt __asm__("s_clqUToUJolt");
extern qword s_dposJolt __asm__("s_dposJolt");
extern qword s_rotJolt __asm__("s_rotJolt");
extern qword s_vecRange __asm__("s_vecRange");
extern qword s_vecOffset __asm__("s_vecOffset");
extern qword D_002A2DE0 __asm__("D_002A2DE0");
extern float *VECTOR_opAssign(float *pg, qword qw) __asm__("VECTOR_opAssign");
// Return qwRow scaled by gScale.
static inline qword QwMulX(qword qwRow, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwRow), "j"(gScale.qw));
    return qw;
}
// Move a float's bits into a GPR qword (mfc1).
static inline VU_FLOAT VuFloatOfFloat(float g)
{
    VU_FLOAT v;
    __asm__("mfc1 %0, %1" : "=r"(v.qw) : "f"(g));
    return v;
}
// Evaluate the quadratic g0 + g1*x + g2*x^2 and clamp it to the limit [gMin, gMax].
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = pclq->g0 + x * (pclq->g1 + x * pclq->g2);
    float gMin = plm->minValue;
    float gMax = plm->maxValue;
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
    float dtJolt = g_clock.time - pcm->joltTime;
    if (dtJolt > DT_CmJoltMax)
    {
        CombineEyeLookAtProj(&pcm->mat, &pcm->matProj, &pcm->matViewProj);
    }
    else
    {
        float gEnv = GEvaluateClqLm(&s_clqUToUJolt, &g_lmZeroOne, dtJolt / DT_CmJoltMax);
        float gJolt = sinf(dtJolt * SW_CmJolt) * gEnv;
        {
            MATRIX3 matRot = LoadRotateMatrix(QwMulX(s_rotJolt, VuFloatOfFloat(gJolt)));
            VECTOR_opAssign(matEye.mat[3], QwMulX(s_dposJolt, VuFloatOfFloat(gJolt)));
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
        *(qword *)&vecRange = QwAddScaled(D_002A2DE0, s_vecRange, VuFloatOfFloat(gJolt));
        *(qword *)&vecOffset = QwMulX(s_vecOffset, VuFloatOfFloat(gJolt));
        BuildSimpleProjectionMatrix(vecRange.x * 512.0f / (pcm->tanHalfFovX * 4096.0f), vecRange.y * 224.0f / (pcm->tanHalfFovY * 4096.0f),
                                    vecOffset.x, vecOffset.y, pcm->nearClip, pcm->farClip, &matProj);
        CombineEyeLookAtProj(&matEye, &matProj, &pcm->matViewProj);
    }
    FInvertMatrix4(&pcm->matViewProj, &pcm->matViewProjInv);
    qword *paplane = pcm->aplaneFrustum;
    qword *paplaneWide = pcm->aplaneFrustumWide;
    float *pgWide1 = (float *)&paplaneWide[1];
    float *pgWide2 = (float *)&paplaneWide[2];
    BuildFrustrum(pcm->tanHalfFovX, pcm->tanHalfFovY, &pcm->mat, paplane);
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
    BuildFrustrum(pcm->tanHalfFovX * 7.93f, pcm->tanHalfFovY * 18.1f, &pcm->mat, paplaneWide);
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

namespace M_CompareCameraVisZones {
struct T { char pad[0xE4]; int unknownE4; char pad2[0x110 - 0xE8]; int priority; float distance; char pad3[0x120 - 0x118]; };
// Qsort comparator for vis-zone candidates: by +0x110 desc, +0xE4 desc, then +0x114 (distance) asc. (inferred)
extern "C" int CompareCameraVisZones(struct T **ppa, struct T **ppb)
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
} // namespace M_CompareCameraVisZones

namespace M_UpdateCmVisZone__FP2CM {
struct O { char pad[0x110]; int k; };
int FindSwObjectsByClass(void *g, int a, int b, int c, int n, struct O **ap) __asm__("FindSwObjectsByClass");
void UpdateVisInside(struct O *po, qword pos) __asm__("UpdateVisInside");
void qsort(void *base, int n, int cb, int (*cmp)(const void *, const void *)) __asm__("qsort");
int CompareCameraVisZones(const void *a, const void *b) __asm__("CompareCameraVisZones");
void SetCmVisZone(CM *p, struct O *po) __asm__("SetCmVisZone");
// Gathers up to 32 zone objects (0x15E9C8 query), scores each against cm pos (0x2273F0), sorts, and selects the best
// via SetCmVisZone if its kind==1.
// Also known as: SortCmObjects (upstream readable / Sly 1 name).
extern "C" void UpdateCmVisZone__FP2CM(CM *cm)
{
    struct O *apo[32];
    qword pos = *(qword *)&cm->pos;
    int n = FindSwObjectsByClass(&g_sw, 0x105, 5, 0, 0x20, apo);
    int i;
    for (i = 0; i < n; i++)
        UpdateVisInside(apo[i], pos);
    qsort(apo, n, 4, CompareCameraVisZones);
    if (n > 0 && apo[0]->k == 1)
        SetCmVisZone(cm, apo[0]);
}
} // namespace M_UpdateCmVisZone__FP2CM

// If changed: pzonVis(+0x10)=pzon, grfvis(+0x14)=pzon->+0xE0. (inferred)
// Also known as: SetCmCurObject (upstream readable / Sly 1 name; SetCmCurObject__FP2CMP2LO).
extern "C" void SetCmVisZone(CM *pcm, VISZONE *pzon)
{
    if (pzon != pcm->pzonVis) {
        pcm->pzonVis = pzon;
        pcm->grfvis = *(uint *)((char *)pzon + 0xE0);   /* CID5::grfvis, the zone's visibility mask (classes.h is not included here: it brings so.h, which renumbers s_rgba) */
    }
}

// Copies a 4x4 placement into CM::mat (+0x60..0x9F) then UpdateCmMat4. (inferred)
// Also known as: SetCmPosMat (upstream readable / Sly 1 name; SetCmPosMat__FP2CMP6VECTOR).
extern "C" void SetCmMat4(CM *pcm, qword *pmat)
{
    ((qword *)&pcm->mat)[0] = pmat[0];
    ((qword *)&pcm->mat)[1] = pmat[1];
    ((qword *)&pcm->mat)[2] = pmat[2];
    ((qword *)&pcm->mat)[3] = pmat[3];
    UpdateCmMat4(pcm);
}

namespace M_SetCameraLookAt {
typedef struct { float x, y, z, w; } V4;
struct M3 { V4 r[3]; } __attribute__((aligned(16)));
typedef struct { M3 m; V4 pos; } M4;
void GetVectorSphere(qword v, float *a, float *b, int c) __asm__("GetVectorSphere");
M3 LoadRotateMatrixPanTilt(float a, float b) __asm__("LoadRotateMatrixPanTilt");
void VECTOR_opAssign(V4 *p, qword v) __asm__("VECTOR_opAssign");
// Build a 4x4 matrix from rotation m and translation pos.
extern "C" { static __inline__ void LoadM4(M4 *pm, M3 m, qword pos)
{
    VECTOR_opAssign(&pm->pos, pos);
    pm->m = m;
    pm->m.r[1].w = 0;
    pm->m.r[2].w = 0;
    pm->m.r[0].w = 0;
    pm->pos.w = 1.0f;
} }
// Place camera at pos looking at target, with extra pan/tilt offsets: pan/tilt from (tgt-pos) (0x11EFB0), rotation
// 0x11B720, pos row, UpdateCmMat4. (inferred)
// Also known as: SetCmLookAt (upstream readable / Sly 1 name; SetCmLookAt__FP2CMG9VU_VECTORT1ff).
extern "C" void SetCameraLookAt(CM *cm, qword pos, qword tgt, float da, float db)
{
    qword d;
    float a, b;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(tgt), "j"(pos));
    GetVectorSphere(d, &a, &b, 0);
    LoadM4((M4 *)&cm->mat, LoadRotateMatrixPanTilt(a + da, b + db), pos);
    UpdateCmMat4(cm);
}
} // namespace M_SetCameraLookAt

namespace M_BuildCmFgfn {
extern float D_002A4FF8 __asm__("D_002A4FF8");
void FitRecipFunction__FffffPfT4(float a, float b, float c, float d, float *pa, float *pb) __asm__("FitRecipFunction__FffffPfT4");
// Builds the GS fog function (FGFN) from nearFogDistance/farFogDistance/maxFog (+0x240..0x248): linear map to 0..4080, +8417280
// bias.
// Also known as: BuildCmFogRecip (upstream readable / Sly 1 name; BuildCmFogRecip__FP2CMPff).
extern "C" void BuildCmFgfn(CM *p, float *af, float f)
{
    if (f == D_002A4FF8) f = 1.0f;
    FitRecipFunction__FffffPfT4(1.0f, 4080.0f, p->farFogDistance / p->nearFogDistance, (1.0f - p->maxFog * f) * 4080.0f, &af[0], &af[1]);
    af[0] += 8417280.0f;
    af[2] = p->nearFogDistance;
    af[3] = af[0] + 8192.0f;
}
} // namespace M_BuildCmFgfn

// 0x14E320: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("daddu $2, $0, $0");

namespace M_SetCmPolicy {
/* Insert an entry into a priority-sorted array of 8 (Claude agent). memmove goes through an inline helper: its
   parameters get their own pseudos, giving the target's (p + i*16) base copied into a1 then +0x320 / +0x310. */
void *memmove(void *dst, void *src, int cb) __asm__("memmove");
// memmove cb bytes.
extern "C" { static inline void Move(void *pvDst, void *pvSrc, int cb) { memmove(pvDst, pvSrc, cb); } }
// Insert {cpp,pcplcy,focus,pv} into priority-sorted acpr[8] (+0x30C count); call pcplcy vt4 Set(pv); if new top:
// prev vt3 Deactivate, modeStartTime=clock.time, vt2 Activate.
// Also known as: SetCmPolicy (upstream readable / Sly 1 name; SetCmPolicy__FP2CM3CPPP5CPLCYii).
extern "C" void SetCmPolicy(CM *p, CPP prio, CPLCY *po, SO *a, void *b)
{
    int i;
    if (p->numPolicies >= sizeof(p->policyStack) / sizeof(p->policyStack[0]))
        return;     /* the stack is full */
    for (i = 0; i < p->numPolicies; i++) {
        if (p->policyStack[i].priority < prio)
            break;
    }
    Move(&p->policyStack[i + 1], &p->policyStack[i], (p->numPolicies - i) * sizeof(CPR));
    p->policyStack[i].priority = prio;
    p->policyStack[i].policy = po;
    p->policyStack[i].focus = a;
    p->policyStack[i].argument = b;
    p->numPolicies++;
    po->Set(b);
    if (i == 0) {
        /* the new top: deactivate the previous top (slot 3), activate this one (slot 2) */
        if (p->numPolicies >= 2)
            p->policyStack[1].policy->Deactivate(p->policyStack[1].focus, p->policyStack[1].argument);
        p->modeStartTime = g_clock.time;
        po->Activate(a, b);
    }
}
} // namespace M_SetCmPolicy

namespace M_RevokeCmPolicy {
/* Matched by Claude agent (2026-10-17): compare against the parameters directly (no local E copy) and `for (i = 0, pe = p->policies; ...)`. */
/* which fields of a stack entry a revoke must match */
enum { REVOKE_BY_PRIO = 1, REVOKE_BY_POLICY = 2, REVOKE_BY_FOCUS = 4, REVOKE_BY_PARAM = 8 };
// Removes the policy-stack entries that match on every field the mask selects (priority, policy, focus object,
// parameter), calling each one's Revoke (vtable slot 5); when the top entry went, deactivates it and activates the new
// top.
// Also known as: RevokeCmPolicy (upstream readable / Sly 1 name; RevokeCmPolicy__FP2CMi3CPPP5CPLCYii).
extern "C" void RevokeCmPolicy(CM *p, int grf, CPP prio, CPLCY *po, SO *a, void *b)
{
    CPR *peOut;
    CPR *pe;
    int i;
    CPLCY *poTop;
    SO *aTop;
    void *bTop;
    poTop = 0;
    aTop = 0;
    bTop = 0;
    peOut = p->policyStack;
    for (i = 0, pe = p->policyStack; i < p->numPolicies; i++, pe++) {
        if ((!(grf & REVOKE_BY_PRIO) || prio == pe->priority) &&
            (!(grf & REVOKE_BY_POLICY) || po == pe->policy) &&
            (!(grf & REVOKE_BY_FOCUS) || a == pe->focus) &&
            (!(grf & REVOKE_BY_PARAM) || b == pe->argument)) {
            if (i == 0) {
                poTop = pe->policy;
                aTop = pe->focus;
                bTop = pe->argument;
            }
            pe->policy->Revoke(pe->argument);
        } else {
            *peOut++ = *pe;
        }
    }
    p->numPolicies = peOut - p->policyStack;
    if (poTop) {
        /* the top policy went away: deactivate it (slot 3) and activate the new top (slot 2) */
        poTop->Deactivate(aTop, bTop);
        if (p->numPolicies > 0) {
            p->modeStartTime = g_clock.time;
            p->policyStack[0].policy->Activate(p->policyStack[0].focus, p->policyStack[0].argument);
        }
    }
}
} // namespace M_RevokeCmPolicy

namespace M_CalcCmAdjust {
void SetVectorCylind(void *a, void *b, float x, float y, float z) __asm__("SetVectorCylind");
// The camera adjust offset: a cylindrical vector pointing opposite the focus's home angle (homeHeading + pi) with length
// adjustDistance (CPDEFI +0x30 / +0x34). (inferred)
// Also known as: CalcCmAdjust (upstream readable / Sly 1 name; CalcCmAdjust__FP2CMP6CPDEFI).
void CalcCmAdjust(void *a, CPDEFI *b) __asm__("CalcCmAdjust"); void CalcCmAdjust(void *a, CPDEFI *b)
{
    SetVectorCylind(a, b, b->homeHeading + 3.14159265f, b->adjustDistance, 0.0f);
}
} // namespace M_CalcCmAdjust

namespace M_ApplyCameraSpring {
// MATCH (Claude agent): hard-register VF locals; clock pointer pinned to v0 and the second dt read via a v0-pinned int.
// Critically-damped spring of *ppos/ *pv toward target with (rSpring,rDamping) and clock dt; snaps and zeroes v when
// nCut.
// Also known as: SpringCm (upstream readable / Sly 1 name; SpringCm__FP2CMP7VECTOR4T1G9VU_VECTORff).
extern "C" void ApplyCameraSpring(CM *p, qword *ppos, float *pvel, qword target, float k1, float k2)
{
    register CLOCK *pclock __asm__("v0");
    register qword s1 __asm__("vf3");
    register qword s2 __asm__("vf4");
    register qword s3 __asm__("vf5");
    register qword d __asm__("vf1");
    register qword v __asm__("vf2");
    if (p->nCut) {
        *ppos = target;
        pvel[2] = 0.0f;
        pvel[1] = 0.0f;
        pvel[0] = 0.0f;
        return;
    }
    pclock = &g_clock;
    __asm__("qmtc2.ni %1, %0" : "=j"(s1) : "r"(k1));
    d = *ppos;
    __asm__("qmtc2.ni %1, %0" : "=j"(s2) : "r"(k2));
    __asm__("vsub.xyzw %0, %1, %0" : "+j"(d) : "j"(target));
    __asm__("qmtc2.ni %1, %0" : "=j"(s3) : "r"(pclock->deltaTimeBits));
    v = *(qword *)pvel;
    __asm__("vmulax.xyzw ACC, %0, %2x\n\tvmaddx.xyzw %0, %1, %3x" : "+j"(d) : "j"(v), "j"(s1), "j"(s2));
    { QWROW u; u.q = s1; }
    __asm__("vadda.xyzw ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(v) : "j"(d), "j"(s3));
    { QWROW u; u.q = s2; }
    *(qword *)pvel = v;
    {
        register qword s4 __asm__("vf3");
        qword x;
        { register int n __asm__("v0") = pclock->deltaTimeBits;
        __asm__("qmtc2.ni %1, %0" : "=j"(s4) : "r"(n)); }
        x = *ppos;
        { QWROW u; u.q = s3; }
        __asm__("vadda.xyzw ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(x) : "j"(v), "j"(s4));
        { QWROW u; u.q = s4; }
        *ppos = x;
    }
}
} // namespace M_ApplyCameraSpring

namespace M_AdjustCmCpdefi {
struct O { char pad[0x4F4]; int stepMode; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");
qword CalcCmAdjust(CM *p, CPDEFI *ps) __asm__("CalcCmAdjust");
int IsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int k) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void ApplyCameraSpring(CM *p, ::qword *ppos, VECTOR *pvel, qword target, float k1, float k2) __asm__("ApplyCameraSpring");
// Springs dposAdjust/dposFocus (+0x2C0/+0x2E0) toward CalcCmAdjust; per STEPPLAYER mode +0x4F4 (0/1/2) different
// gains; writes cpdefi.adjustedPosition = basePosition + dposAdjust.
// Also known as: AdjustCmCpdefi (upstream readable / Sly 1 name; AdjustCmCpdefi__FP2CMP2SOP6CPDEFI).
extern "C" void AdjustCmCpdefi(CM *p, struct O *po, CPDEFI *ps)
{
    qword q = CalcCmAdjust(p, ps);
    if (IsBasicDerivedFrom__FP5BASIC3CID(po, 0x34)) {
        switch (po->stepMode) {
        case 0:
            ApplyCameraSpring(p, &p->dposAdjust, &p->vAdjust, q, 5.0f, -3.5f);
            ApplyCameraSpring(p, &p->dposFocus, &p->vFocus, D_002A2DD0, 5.0f, -3.5f);
            break;
        case 1:
            ApplyCameraSpring(p, &p->dposAdjust, &p->vAdjust, q, 1.0f, -10.0f);
            ApplyCameraSpring(p, &p->dposFocus, &p->vFocus, p->dposFocus, 0.0f, -10.0f);
            break;
        case 2:
            ApplyCameraSpring(p, &p->dposAdjust, &p->vAdjust, q, 5.0f, -3.5f);
            break;
        }
    } else {
        ApplyCameraSpring(p, &p->dposAdjust, &p->vAdjust, q, 5.0f, -3.5f);
        ApplyCameraSpring(p, &p->dposFocus, &p->vFocus, D_002A2DD0, 5.0f, -3.5f);
    }
    {
        qword r;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(*(qword *)&ps->basePosition), "j"(p->dposAdjust));
        *(qword *)&ps->adjustedPosition = r;
    }
}
} // namespace M_AdjustCmCpdefi

// Zero the xyz components of pv.
static inline void ZeroVector(VECTOR *pv)
{
    pv->z = 0.0f;
    pv->y = 0.0f;
    pv->x = 0.0f;
}
// Resets the camera's adjust state: the adjust offset from CalcCmAdjust, and zero adjust / focus velocities.
// Also known as: ResetCmAdjust (upstream readable / Sly 1 name; ResetCmAdjust__FP2CM).
extern "C" void ResetCmAdjust(CM *pcm)
{
    pcm->dposAdjust = CalcCmAdjust(pcm);
    ZeroVector(&pcm->vAdjust);
    ZeroVector(&pcm->vFocus);
}

namespace M_GetCmCpdefi {

#if defined(SLY2_EE_LAYOUT)
struct O { int pad[3]; VTE *vt; };
#else
typedef SO O;
#endif
qword CalcCmAdjust(CM *p, CPDEFI *ps) __asm__("CalcCmAdjust");
// If pso == focus(+0x260) copy cached cpdefi (+0x270) else pso->vt50 GetCpdefi and adjustedPosition = basePosition +
// CalcCmAdjust.
// Also known as: GetCmCpdefi (upstream readable / Sly 1 name; GetCmCpdefi__FP2CMP2SOP6CPDEFI).
extern "C" void GetCmCpdefi(CM *p, O *po, CPDEFI *ps)
{
    if ((SO *)po == p->focus) {
        *ps = p->cpdefi;
    } else {
#if defined(SLY2_EE_LAYOUT)
        VTE *e = &po->vt[50];   /* SO::GetCpdefi by hand: so.h cannot be included here (header clash) */
        qword q, r;
        ((void (*)(void *, CPDEFI *))e->pfn)((char *)po + e->d, ps);
#else
        qword q, r;
        po->GetCpdefi(ps);
#endif
        q = CalcCmAdjust(p, ps);
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(*(qword *)&ps->basePosition), "j"(q));
        *(qword *)&ps->adjustedPosition = r;
    }
}
} // namespace M_GetCmCpdefi

namespace M_FindCameraClearInterval {
/* float g placed after the pointer params: its param copy comes after pgMax, lengthening pgMax so the hoisted 0x40 wins $fp in global alloc */
struct T { int a, b, c; };
struct O { char pad[0x70]; int c; struct T *at; };
struct HIT { char pad[0x40]; float enterTime; float exitTime; char pad2[0x18]; };
int ClipSegmentBsp(struct T *pBegin, struct T *pEnd, qword pos, qword dir, int c, struct HIT *ahit) __asm__("ClipSegmentBsp");
int MergeSegmentLists(int c, struct HIT *ahit, int cNew, struct HIT *ahitNew, int cMax, struct HIT *ahitOut, int f) __asm__("MergeSegmentLists");
// Clips segment pos->dir against each SO's camera BSP (0x118668), merges intervals (0x14C9F8), returns first clear gap
// > duMin in *puFrom/ *puTo.
// Also known as: FFindCmClearInterval (upstream readable / Sly 1 name;
//   FFindCmClearInterval__FiP8CMSOLIDSG9VU_VECTORT2P6VECTORT4f).
extern "C" int FindCameraClearInterval(int n, struct O **apo, qword pos, qword dir, float *pgMin, float *pgMax, float g)
{
    struct HIT ahit[0x40];
    int c = 0;
    int i;
    int j;
    float g0, g1;
    for (i = 0; i < n; i++) {
        struct O *po = apo[i];
        int cNew = ClipSegmentBsp(po->at, po->at + po->c, pos, dir, 0x40 - c, &ahit[c]);
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
} // namespace M_FindCameraClearInterval

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A4FF8 __asm__("D_002A4FF8") = 3.40282347e+38f; }
namespace M_FindCmClosestClearPos {

extern int D_002BD8B0[] __asm__("D_002BD8B0");
qword SetVectorSphere(float pan, float tilt, float s) __asm__("SetVectorSphere");
int FindCameraClearInterval(void *p, void *x, qword a, qword b, float *pt, float *pu, float g) __asm__("FindCameraClearInterval");
qword FindCmClosestClearPos(void *p, void *x, qword pos, float g) __asm__("FindCmClosestClearPos");

// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };

// Return a + b * s.
static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}

// Return v1*f1 + v2*f2 + v3*f3.
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2, VU_FLOAT f3, VU_VECTOR v3)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %1, %4x\n\tvmaddax.xyzw $ACC, %2, %5x\n\tvmaddx.xyzw %0, %3, %6x"
            : "=j"(r) : "j"(v1.q), "j"(v2.q), "j"(v3.q), "j"(f1.q), "j"(f2.q), "j"(f3.q));
    return r;
}

// Samples 9 tilts x D_002BD8B0[i] pans at radius g, padding 5/g, returns closest clear position.
// Also known as: FindCmClosestClearPos (upstream readable / Sly 1 name;
//   FindCmClosestClearPos__FP8CMSOLIDSP6VECTORG9VU_VECTORf).
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

namespace M_SquishCmEye {
/* MATCH (Claude agent, lane D4): v2 inline Dot(qword *pa, qword *pb) (keeps &ax[i].normal hoisted in a reg), returning int from a qmfc2 into `register int d asm("$7")`, stored through `*(int *)pg++` (sw $a3 like the target). */
struct X { qword pos; qword normal; qword unknown20; };
struct E { int a, b, c; };
struct LO { char pad[0x70]; int rootNodeIndex; struct E *bspNodes; };
extern float D_002BD8D4 __asm__("D_002BD8D4");
void STACKSCOPE_ctor(void *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
void *AllocFromStack(struct SWALLOC *pal) __asm__("AllocFromStack");
int IntersectSphereBsp(qword pos, struct LO *po, struct E *pe, float g, int a, int b, int cMax, struct X *ax) __asm__("IntersectSphereBsp");
void SolveInequalities(int c, float *aDot, float *aW, float *aZ, float g) __asm__("SolveInequalities");
qword LimitVectorLength(qword v, float g) __asm__("LimitVectorLength");

// Dot product of the xyz parts of *pa and *pb, as raw float bits.
extern "C" { static inline int Dot(qword *pa, qword *pb)
{
    register qword nj __asm__("vf1") = *pb;
    register qword t __asm__("vf2") = *pa;
    qword one;
    register int d __asm__("$7");
    __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "+j"(t), "=&j"(one) : "j"(nj));
    __asm__("qmfc2.ni %0, %1" : "=r"(d) : "j"(t));
    return d;
} }

// Allocate cb bytes with alignment cbAlign from the world heap.
extern "C" { static inline void *PvAlloc(int cb, int cbAlign)
{
    struct SWALLOC al;
    al.cb = cb;
    al.cbAlign = cbAlign;
    return AllocFromStack(&al);
} }

// Collects sphere-vs-BSP contacts (0x1A93C8) for the eye, builds n x n normal dot matrix + constraint values via pfn,
// solves (0x11AD30) and returns v + sum of normal impulses.
// Also known as: SquishCmEye (upstream readable / Sly 1 name;
//   SquishCmEye__FP2CM14PFNCMHITWEIGHTP6VECTORT2G9VU_VECTORT4ff).
extern "C" qword SquishCmEye(void *p, int c, struct LO **apo, float (*pfn)(void *, qword, qword, struct X *),
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
} // namespace M_SquishCmEye

struct LOBSP { char pad[0x70]; int numCameraBspNodes; };
// Camera object filter: accepts objects with camera-collision BSP nodes (a positive count at +0x70) (inferred).
extern "C" int HasCameraBsp(void *pv, LO *plo)
{
    return ((LOBSP *)plo)->numCameraBspNodes > 0;
}

namespace M_SvSquishCmEyeConstraint {
// MATCH (Claude agent, lane D24, 2026-10-23): v6 with the mtc1 asm's allocation-only "j" operands replaced by an
// "r,j" constraint on td (never-chosen alternative, forced off by an unused "i,r"(0) operand). With fewer operands
// the mtc1 has 4 dependents like the `one` sqc2, so the luid tie-break issues the sqc2 first, like the target.
typedef union { qword q; float f; } U;
struct PL { qword pos; qword plane; };
struct P { char pad[0x48]; float nearCorner; };
extern float D_002BD8D8 __asm__("D_002BD8D8");
extern char s_smpSquishEye[] __asm__("s_smpSquishEye");
void *InitVuTemp(void *p) __asm__("InitVuTemp");
void SmoothValue__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr) __asm__("SmoothValue__FfffP3SMPPf");
// Signed distance from pos to plane (plane.xyz . pos - plane.w).
extern "C" { static __inline__ float GDistPlane(qword plane, qword pos)
{
#ifdef SLY2_EE_ASM
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
#else
    VuLanes p = VuGet(plane), q = VuGet(pos);
    return ((p.x * q.x + p.y * q.y) + p.z * q.z) - p.w;
#endif
} }
// Dot product of the xyz parts of *pa and b as a float.
extern "C" { static __inline__ float GDot(const qword *pa, qword b)
{
    qword a = *pa;
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
} }
// Constraint for SquishCmEye: SmoothValue(dist-to-plane, nearCornerDistance(+0x48) - k) ; return dot(vEye, normal) - smoothed.
// Also known as: GCmAvoidWeight (upstream readable / Sly 1 name; GCmAvoidWeight__FP2CMiG9VU_VECTORP5CMHIT).
extern "C" float SvSquishCmEyeConstraint(struct P *p, int unused, qword vIn, struct PL *pl)
{
    float r;
    qword plane = pl->plane;
    qword pos = pl->pos;
    SmoothValue__FfffP3SMPPf(GDistPlane(plane, pos), p->nearCorner - D_002BD8D8, g_clock.cameraDeltaTime, s_smpSquishEye, &r);
    return GDot(&vIn, pl->plane) - r;
}
} // namespace M_SvSquishCmEyeConstraint

namespace M_ClipCameraEye {
// ClipCameraEye (Claude agent, lane K5, 2026-10-24): MATCH. Built on laneK4v1 (K3/K4 fresh rewrite in the ClipEdgeToSphere style).
// Keys: the VU_FLOAT(const float *) ctor's word input has a never-chosen "V" alternative ("r,V"), so it stays a MEM until
// reload: the clock dt load is then an input reload (lw v0 for the vdiv; for the final AddScaled reload reuses the dying
// pclock register, lw s0,0x10(s0)), which also shortens the clock chain in sched1 so no extra operand is needed on the vdiv;
// v is computed before posMin so the spill slots come out v 0x30 / posMin 0x40. No register pins.
// Float broadcast into a VU0 register, from a value or from memory.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(*(const int *)pg)); } /* never-chosen "V" (non-offsettable mem) alternative: allocation-only (the word stays a MEM until reload) */ };

extern qword D_002BD8F0 __asm__("D_002BD8F0"), D_002BD900 __asm__("D_002BD900");
extern float D_002BD8D8 __asm__("D_002BD8D8"), D_002BD8DC __asm__("D_002BD8DC");
void *STACKSCOPE_ctor(STACKSCOPE *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(STACKSCOPE *p, int n) __asm__("_$_10STACKSCOPE");
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj) __asm__("IntersectSwBoundingBox");
qword SquishCmEye(void *p, int i, int j, void *pfn, qword a, qword v, float g, float h) __asm__("SquishCmEye");
void HasCameraBsp(void) __asm__("HasCameraBsp");
void SvSquishCmEyeConstraint(void) __asm__("SvSquishCmEyeConstraint");
qword ClipCameraEye(void *p, qword pos, qword posTarget) __asm__("ClipCameraEye");

// Divide v by g.
static inline qword DivideBy(const qword &v, const VU_FLOAT &g) { qword r; __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
// Return base + vec * s.
static inline qword AddScaled(qword base, const qword &vec, const VU_FLOAT &s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(base), "j"(vec), "j"(s.q));
    return r;
}
// Dot product of the xyz parts of a and b (x lane).
static inline qword QwDot(const qword &a, const qword &b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
// Square root of the x lane of a.
static inline qword QwSqrt(const qword &a) { qword r; __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(a)); return r; }
// x lane of q as a float.
static inline float GXOfQw(const qword &q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
// Length of v as a float.
static inline float Length(const qword &v) { return GXOfQw(QwSqrt(QwDot(v, v))); }

// VEye=(next-prev)/dtPrev; gather camera-colliding SOs in a box around prev; SquishCmEye; return prev + vClip*dt.
// Also known as: PosCmAvoid (upstream readable / Sly 1 name; PosCmAvoid__FP2CMG9VU_VECTORT1).
qword ClipCameraEye(void *p, qword pos, qword posTarget)
{
    CLOCK *pclock = &g_clock;
    int ii, jj;
    qword dpos = VuSub(posTarget, pos);
    qword v = DivideBy(dpos, VU_FLOAT(&pclock->cameraDeltaTime));
    qword posMin = VuAdd(pos, D_002BD8F0);
    qword posMax = VuAdd(pos, D_002BD900);
    STACKSCOPE frame;
    IntersectSwBoundingBox(&g_sw, posMin, posMax, (void *)HasCameraBsp, 0, &ii, &jj);
    qword w = SquishCmEye(p, ii, jj, (void *)SvSquishCmEyeConstraint, pos, v, D_002BD8D8, Length(v) + D_002BD8DC);
    qword r = AddScaled(pos, w, VU_FLOAT(&pclock->cameraDeltaTime));
    return r;
}
} // namespace M_ClipCameraEye

namespace M_ConvertCmScreenToWorld {
typedef struct { qword r[4]; } M4;
// Transform point v by the 4x4 matrix pm.
extern "C" { static __inline__ qword Mul(M4 *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(o) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]));
    return o;
} }
// Screen (x,y,depth) -> world: axis swap, scale by tanHalfX/Y (+0x40/+0x44), multiply by CM::mat.
// Also known as: ConvertCmScreenToWorld (upstream readable / Sly 1 name; ConvertCmScreenToWorld__FP2CMG9VU_VECTOR).
extern "C" qword ConvertCmScreenToWorld(CM *cm, qword v)
{
    union { qword q; float f[4]; } r;
    float x = QwordX(v);
    float z = QwordZ(v);
    float y = QwordY(v);
    r.f[2] = y;
    r.f[0] = z;
    r.f[1] = -x;
    if (z > 1e-4f) {
        r.f[1] = -x * z;
        r.f[2] = y * z;
    }
    r.f[1] *= cm->tanHalfFovX;
    r.f[2] *= cm->tanHalfFovY;
    return Mul((M4 *)&cm->mat, r.q);
}
} // namespace M_ConvertCmScreenToWorld

namespace M_ConvertCmWorldToScreen {

typedef struct { qword r[4]; } M4;
void InvertMatrix4Rigid(M4 *pm, M4 *pn) __asm__("InvertMatrix4Rigid");
// World -> screen: inverse(CM::mat) * pos, axis swap, divide by tanHalf and depth (if > 1e-4)
// Also known as: ConvertCmWorldToScreen (upstream readable / Sly 1 name; ConvertCmWorldToScreen__FP2CMG9VU_VECTOR).
extern "C" qword ConvertCmWorldToScreen(CM *cm, qword v)
{
    union { qword q; float f[4]; } r;
    M4 m;
    float x, y, z;
    InvertMatrix4Rigid(&m, (M4 *)&cm->mat);
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
    r.f[0] = -y / cm->tanHalfFovX;
    r.f[1] = z / cm->tanHalfFovY;
    if (x > 1e-4f) {
        r.f[0] = r.f[0] / x;
        r.f[1] = r.f[1] / x;
    }
    return r.q;
}
} // namespace M_ConvertCmWorldToScreen

// 0x14F3C0: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x40");

namespace M_SetCmPos {
typedef union { struct { qword r[4]; } q; float af[4][4]; } M4;
qword *VECTOR_opAssign(qword *p, qword v) __asm__("VECTOR_opAssign");
void SetCmMat4(CM *p, M4 *m) __asm__("SetCmMat4");
// Option setter (g_aeopid[171], otyp 2 vector): keep rotation, set CM::mat pos row, SetCmMat4.
extern "C" void SetCmPos(CM *cm, qword pos)
{
    M4 m = *(M4 *)&cm->mat;
    VECTOR_opAssign(&m.q.r[3], pos);
    m.af[3][3] = 1.0f;
    SetCmMat4(cm, &m);
}
} // namespace M_SetCmPos

namespace M_SetCmMat {
#include "mat.h"
void SetCmMat4(CM *p, struct MATRIX4 *m) __asm__("SetCmMat4");
// Set rotation from MATRIX3, keep pos, SetCmMat4. (inferred)
void SetCmMat(CM *cm, struct MATRIX3 *pmat) __asm__("SetCmMat"); void SetCmMat(CM *cm, struct MATRIX3 *pmat)
{
    struct MATRIX4 m;
    *(struct MATRIX3 *)&m = *pmat;
    *(struct VECTOR *)m.mat[3] = *(struct VECTOR *)&cm->pos;
    m.mat[2][3] = 0.0f;
    m.mat[1][3] = 0.0f;
    m.mat[0][3] = 0.0f;
    SetCmMat4(cm, &m);
}
} // namespace M_SetCmMat

namespace M_GetCmRotDeg {
// Pack x, y, z into a 128-bit GPR value.
extern "C" { static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
} }
qword DecomposeRotationMatrixToEuler(void *p) __asm__("DecomposeRotationMatrixToEuler");
// Option getter (g_aeopid[173]): euler angles of CM::mat (0x11BFF0) in degrees.
#define DEGREES_PER_RADIAN 57.295776f
extern "C" qword GetCmRotDeg(CM *p)
{
    union { qword q; float f; } r;
    qword v = DecomposeRotationMatrixToEuler(&p->mat);
    r.q = Make(QwordX(v) * DEGREES_PER_RADIAN, QwordY(v) * DEGREES_PER_RADIAN, QwordZ(v) * DEGREES_PER_RADIAN);
    return r.q;
}
} // namespace M_GetCmRotDeg

namespace M_SetCmRotDeg {
// Pack x, y, z into a 128-bit GPR value.
extern "C" { static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
} }
typedef struct { qword r[3]; } M3;
typedef union { struct { qword r[4]; } q; M3 m3; float af[4][4]; } M4;
void LoadRotateMatrixEuler(M3 *pm, qword v) __asm__("LoadRotateMatrixEuler");
void SetCmMat4(CM *p, M4 *m) __asm__("SetCmMat4");
// Option setter (g_aeopid[173]): degrees -> radians, rotation matrix 0x11B628, keep pos, SetCmMat4. (inferred)
extern "C" void SetCmRotDeg(CM *cm, qword v)
{
    M4 mat = *(M4 *)&cm->mat;
    union { qword q; float f; } r;
    M3 m;
#define RADIANS_PER_DEGREE 0.017453294f
    r.q = Make(QwordX(v) * RADIANS_PER_DEGREE, QwordY(v) * RADIANS_PER_DEGREE, QwordZ(v) * RADIANS_PER_DEGREE);
    LoadRotateMatrixEuler(&m, r.q);
    mat.m3 = m;
    mat.af[2][3] = 0.0f;
    mat.af[1][3] = 0.0f;
    mat.af[0][3] = 0.0f;
    SetCmMat4(cm, &mat);
}
} // namespace M_SetCmRotDeg

namespace M_SetCmFov__FP2CMf {
// Fov(+0x20)=targetFieldOfView(+0x24)=f, dfov=0; RecalcCmFrustrum if changed > 1e-4.
extern "C" void SetCmFov__FP2CMf(CM *cm, float f)
{
    int fChanged = __builtin_fabsf(f - cm->fieldOfView) > 0.0001f;
    cm->fieldOfView = f;
    cm->targetFieldOfView = f;
    cm->dfov = 0.0f;
    if (fChanged) RecalcCmFrustrum(cm);
}
} // namespace M_SetCmFov__FP2CMf

// Option setter g_aeopid[162] (field +0x24): targetFieldOfView=f and the active policy's saved fov (acpr[0].pcplcy+4) so
// Deactivate restores it. (inferred)
// Also known as: SetCmFovTarget (upstream readable / Sly 1 name; SetCmFovTarget__FP2CMf).
extern "C" void SetCmBaseFovTarget(CM *pcm, float fovTarget)
{
    CPLCY *pcplcy = pcm->policyStack[0].policy;
    pcm->targetFieldOfView = fovTarget;
    if (pcplcy != 0) {
        pcplcy->savedTargetFieldOfView = fovTarget;
    }
}

// SNearClip(+0x4C)=f; RecalcCmFrustrum.
void SetCmNearClip(CM *pcm, float sNearClip)
{
    pcm->nearClip = sNearClip;
    RecalcCmFrustrum(pcm);
}

// Option setter g_aeopid[164]: SetCmNearClip and the active policy's saved near clip (pcplcy+8) (inferred)
// Also known as: SetCmNearClipAll (upstream readable / Sly 1 name; SetCmNearClipAll__FP2CMf).
extern "C" void SetCmBaseNearClip(CM *pcm, float sNearClip)
{
    CPLCY *pcplcy;
    SetCmNearClip(pcm, sNearClip);
    pcplcy = pcm->policyStack[0].policy;
    if (pcplcy != 0) {
        pcplcy->savedNearClip = sNearClip;
    }
}

// SFarClip(+0x50)=f; RecalcCmFrustrum.
void SetCmFarClip(CM *pcm, float g)
{
    pcm->farClip = g;
    RecalcCmFrustrum(pcm);
}

// SNearFog(+0x240)=f; RecalcCmFrustrum.
void SetCmNearFog(CM *pcm, float g)
{
    pcm->nearFogDistance = g;
    RecalcCmFrustrum(pcm);
}

// SFarFog(+0x244)=f; RecalcCmFrustrum.
void SetCmFarFog(CM *pcm, float g)
{
    pcm->farFogDistance = g;
    RecalcCmFrustrum(pcm);
}

// UFogMax(+0x248)=f; RecalcCmFrustrum.
void SetCmUFogMax(CM *pcm, float g)
{
    pcm->maxFog = g;
    RecalcCmFrustrum(pcm);
}

namespace M_SetCmRgbaFog__FP2CMP4RGBA {
struct S4 { char a[4]; };
void ColorCorrectRgba(void *p) __asm__("ColorCorrectRgba");
// RgbaFog(+0x24C)=*prgba; 0x1ED278 on it; RecalcCmFrustrum.
extern "C" void SetCmRgbaFog__FP2CMP4RGBA(CM *cm, struct S4 *s)
{
    *(struct S4 *)&cm->rgbaFog = *s;
    ColorCorrectRgba(&cm->rgbaFog);
    RecalcCmFrustrum(cm);
}
} // namespace M_SetCmRgbaFog__FP2CMP4RGBA

// RMRD ratio (+0x18)=f; RecalcCmFrustrum (which derives +0x1C = max(r/fov,1)) (inferred)
void SetCmMrdRatio(CM *pcm, float g)
{
    pcm->rFovCullRef = g;
    RecalcCmFrustrum(pcm);
}

namespace M_JoltCamera {
// Starts a camera shake of the given kind now (joltTime, +0x618; joltKind, +0x61C); UpdateCmMat4 applies it for 0.5 s.
// Also known as: JoltCm (upstream readable / Sly 1 name; JoltCm__FP2CMi).
extern "C" void JoltCamera(CM *p, int a)
{
    p->joltTime = g_clock.time;
    p->joltKind = a;
}
} // namespace M_JoltCamera

namespace M_BuildStereoEyeViewProj {
typedef struct { qword r[3]; } M3;
typedef struct { qword r[4]; } M4;
void VECTOR_opAssign(qword *p, qword v) __asm__("VECTOR_opAssign");
void CombineEyeLookAtProj(M4 *pm, void *pv, int n) __asm__("CombineEyeLookAtProj");
// Copy the rotation pmat into pm with zero w in rows 0-2 and w = 1 in row 3 (the caller fills the position).
extern "C" { static __inline__ void LoadM4(M4 *pm, M3 *pmat)
{
    pm->r[0] = pmat->r[0];
    pm->r[1] = pmat->r[1];
    pm->r[2] = pmat->r[2];
    ((int *)&pm->r[1])[3] = 0;
    ((int *)&pm->r[2])[3] = 0;
    ((int *)&pm->r[0])[3] = 0;
    ((float *)&pm->r[3])[3] = 1.0f;
} }
// Builds a view-proj from CM::mat with pos offset along the right axis by +/- stereoEyeOffset (k=1/2) via CombineEyeLookAtProj
// into pm (proj = pv or matProj) (inferred)
// Also known as: MatCmStereoEye (upstream readable / Sly 1 name; MatCmStereoEye__FP2CMiP7MATRIX4iPv).
extern "C" void BuildStereoEyeViewProj(CM *cm, int k, void *pv, int n, M4 *pm)
{
    QWROW r, g;
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
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwrqOut) : "j"(*(qword *)&cm->pos), "j"(((M3 *)&cm->mat)->r[1]), "j"(g.q));
        r.q = qwrqOut;
    }
    VECTOR_opAssign(&pm->r[3], r.q);
    LoadM4(pm, (M3 *)&cm->mat);
    CombineEyeLookAtProj(pm, pv ? pv : (void *)&cm->matProj, n);
}
} // namespace M_BuildStereoEyeViewProj

namespace M_IsInCameraFrustum {
// True if point/sphere is inside all 4 frustum side planes using the transposed cull rows (+0xA0) and VU clip flags.
// (inferred)
// Also known as: FInsideCmPlanes (upstream readable / Sly 1 name; FInsideCmPlanes__FP2CMG9VU_VECTORT1).
extern "C" int IsInCameraFrustum(CM *p, qword v, qword w)
{
    qword one;
    int f;
    __asm__("vmaxw.xyzw %0, $vf0, $vf0w" : "=j"(one));
    __asm__("vmulax.xyzw $ACC, %1, %4x\n\tvmadday.xyzw $ACC, %2, %4y\n\tvmaddaz.xyzw $ACC, %3, %4z\n\tvmsubax.xyzw $ACC, %5, %6x\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tcfc2.ni %0, $vi17"
            : "=r"(f) : "j"(*(qword *)&p->agFrustumXyz[0]), "j"(*(qword *)&p->agFrustumXyz[4]), "j"(*(qword *)&p->agFrustumXyz[8]), "j"(v), "j"(one), "j"(w));
    return (f ^ 0xF0) == 0;
}
} // namespace M_IsInCameraFrustum

namespace M_IsWithinCameraRenderDistance {
// True if |dpos| < s*rCullScale; *pu = fade fraction between outer radius and (r-400*rCullScale) (inferred)
// Also known as: FInsideCmMrd (upstream readable / Sly 1 name; FInsideCmMrd__FP2CMG9VU_VECTORfPf).
extern "C" int IsWithinCameraRenderDistance(CM *p, qword v, float g, float *pu)
{
    qword l, one;
    float l2, r, r2, d2;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(v));
    r = g * p->rCullScale;
    __asm__("qmfc2.ni %0, %1" : "=r"(l2) : "j"(l));
    r2 = r * r;
    if (r2 < l2)
        return 0;
    d2 = r - p->rCullScale * 400.0f;
    d2 = d2 * d2;
    if (d2 < l2)
        *pu = (r2 - l2) / (r2 - d2);
    else
        *pu = 1.0f;
    return 1;
}
} // namespace M_IsWithinCameraRenderDistance


/* 0x256278 (the matrix option's setter, forwarding to SetCmMat): an inline function in sly2/cm.h. */

namespace M_DestroyCm {
void BASIC_dtor(CM *, int) __asm__("_$_5BASIC");   /* ~BASIC (screen.cpp), passed ~CM's in-charge flag */
/* Release a counted sound through BASIC::Release (slot 2); SND need not be complete here. */
static inline void ReleaseSnd(CRefPtr<SND> *ref) { if (ref->m_p) ((BASIC *)ref->m_p)->Release(); }
// ~CM (_$_2CM): releases the policies' four sound references in reverse member order, then runs the base destructor
// (~BASIC, _$_5BASIC, shared with SW and the screens) with the in-charge flag. Labelled and weak in
// .gnu.linkonce.t._$_2CM (as screen.cpp's ~BASIC): a CM::~CM() definition would need SND complete for the
// CRefPtr<SND> members, and the implicit destructor would be inlined into the static destructor, which calls it.
extern "C" { void DestroyCm(CM *p, int x) __asm__("_$_2CM") __attribute__((weak, section(".gnu.linkonce.t._$_2CM"))); }
extern "C" void DestroyCm(CM *p, int x)
{
    ReleaseSnd(&p->cpzoom.zoomStopSound);
    ReleaseSnd(&p->cpzoom.psndZoom);
    ReleaseSnd(&p->cplook.zoomStopSound);
    ReleaseSnd(&p->cplook.psndZoom);
    BASIC_dtor(p, x);
}
} // namespace M_DestroyCm

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const float D_002A4FF8;   /* defined below where the ELF has it (.rodata order) */













  /* smoothing weights */

// ---- cm's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
SMP D_002BD810 = {20.0f, 10.0f, 0.5f};
float D_002BD81C = 500.0f;
float D_002BD820 = 20000.0f;
float D_002BD824 = 0.5f;
VECTOR4 g_posEyeDefault = {0.0f, -2000.0f, 500.0f, 0.0f};
VECTOR4 g_posCenterDefault = {0.0f, 0.0f, 0.0f, 0.0f};
float DT_CmJoltMax = 0.5f;
float SW_CmJolt = 50.0f;
CLQ s_clqUToUJolt __attribute__((aligned(16))) = {{{1.0f, -2.0f, 1.0f, 0.0f}}};
VECTOR4 s_dposJolt = {0.0f, 0.0f, 0.0f, 0.0f};
VECTOR4 s_rotJolt = {0.05f, 0.0f, 0.0f, 0.0f};
VECTOR4 s_vecRange = {0.0f, 0.0f, 0.0f, 0.0f};
VECTOR4 s_vecOffset = {0.0f, 0.0f, 0.0f, 0.0f};
int D_002BD8B0[9] = {1, 5, 10, 12, 15, 12, 10, 5, 1};
float D_002BD8D4 = 4.0f;
float D_002BD8D8 = 125.0f;
float D_002BD8DC = 1500.0f;
SMPA s_smpSquishEye = {2500.0f, 0.0f, 0.25f, 0.0f};
VECTOR4 D_002BD8F0 = {-250.0f, -250.0f, -250.0f, 0.0f};
VECTOR4 D_002BD900 = {250.0f, 250.0f, 250.0f, 0.0f};
