// cplcy.cpp -- camera policies (CPLCY and the CPLOOK / CPZOOM / CPHANG / CPCAMBLEND / CPTURRET / CPTWOSHOT
// policies): constructors, activate / update / deactivate, the zoom policy's setters, and the counted sound
// pointer's linkonce operator=. TU 0x14FCA8..0x1520C8 (51 functions, address order) plus RcPtr_Assign (0x258040).
//
// Types: sly2/cm.h's policy classes and CM (g_cm is g_cm). The constructors store each class's vtable by
// hand (the virtuals still have C names). The m2c bodies are written on those types; the hand-written per-function
// files keep their views in namespaces named after each function (as jt.cpp and mark.cpp do).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_cplcy
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_cplcy

#include "sly2/basic.h"
#include "sly2/cm.h"
#include "sly2/act.h"
#include "sly2/lo.h"
#include "sly2/po.h"
#include "sly2/slotheap.h"
#include "sly2/sw.h"
#include "sly2/cref.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/joy.h"
#include "sly2/clock.h"
#include "sly2/save.h"
#include "sly2/vtables.h"

// 0x151F70 StaticInit_cplcy and GlobalConstructors_cplcy: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()


extern "C" {
extern int D_002DE254;      /* the active policy (compared by IsCameraPolicyActive) */
CPLCY *CPLCY_ctor(CPLCY *pcplcy);
void InitCplcy(CPLCY *pcplcy);
void UpdateCplcy(CPLCY *pcplcy, void *pv, JOY *pjoy);
void ActivateCplcy(CPLCY *pcplcy, void *pvHero, void *pv);
void DeactivateCplcy(CPLCY *pcplcy, void *pvHero, void *pv);
void SnapCpzoom(CPZOOM *pcpzoom);
extern "C++" { void RcPtr_Assign(CRefPtr<SND> *ppsnd, SND *psnd) __asm__("RcPtr_Assign"); }
void ReleaseAct(ACT1 **ppact);   /* releases the act and clears the pointer (HYPOTHESIS: by its use) */
}
void SetCmNearClip(CM *pcm, float sNearClip);
void SetCmFov(CM *pcm, float fov);
void CutCm(CM *pcm);

// CPLCY base constructor: vptr(+0xC) = 0x2CD570.
extern "C" CPLCY *CPLCY_ctor(CPLCY *pcplcy)
{
    SLY2_SET_VPTR(pcplcy, CPLCY);
    return pcplcy;
}

// Default (empty) CPLCY slot 1 = Init. (inferred)
void CPLCY::Init()   /* key function: CPLCY's vtable comes out here */
{
}

// Default slot 2 Activate(focus, pv): pvHero(+0)=focus; save g_cm targetFieldOfView (+4) and nearClip (+8)
// Also known as: Activate (upstream readable / Sly 1 name; Activate__5CPLCYP2LOPv).
extern "C" void ActivateCplcy(CPLCY *pcplcy, void *pvHero, void *pv)
{
    pcplcy->pvHero = pvHero;
    pcplcy->savedNearClip = g_cm.nearClip;
    pcplcy->savedTargetFieldOfView = g_cm.targetFieldOfView;
}

// Default slot 3 Deactivate: pvHero=0; SetCmNearClip(saved); targetFieldOfView=saved.
// Also known as: Deactivate (upstream readable / Sly 1 name; Deactivate__5CPLCYP2LOPv).
extern "C" void DeactivateCplcy(CPLCY *pcplcy, void *pvHero, void *pv)
{
    pcplcy->pvHero = 0;
    SetCmNearClip(&g_cm, pcplcy->savedNearClip);
    g_cm.targetFieldOfView = pcplcy->savedTargetFieldOfView;
}

// Default (empty) slot 4: called by SetCmPolicy with pv on every insert. (inferred)
extern "C" void SetCplcy()
{
}

// Default (empty) slot 5: called by RevokeCmPolicy for each removed entry. (inferred)
extern "C" void CPLCY_Revoke()
{
}

// Default (empty) slot 6 Update(pcpdefi, pjoy)
void UpdateCplcy() __asm__("UpdateCplcy"); void UpdateCplcy()
{
}

// Default (empty) slot 7. (inferred)
extern "C" void ResetCplcy()
{
}

// True if pcplcy is g_cm's current top policy (D_002DE254 = g_cm.acpr[0].pcplcy)
int IsCameraPolicyActive(CPLCY *pcplcy)
{
    return D_002DE254 == (int)pcplcy;
}

// Constructor of the default policy at CM+0x3A0 (vtable 0x2CD528) (inferred)
extern "C" CMPART1 *CMPART1_ctor(CMPART1 *pcmpart1)
{
    CPLCY_ctor(pcmpart1);
    SLY2_SET_VPTR(pcmpart1, CMPART1);
    return pcmpart1;
}

// Slot 1 of 0x2CD528: just calls CPLCY::Init. (inferred)
void CMPART1::Init()   /* key function */
{
    CPLCY::Init();
}

// Slot 6 of 0x2CD528: just calls CPLCY::Update (default policy does nothing)
extern "C" void UpdateCmpart1(CMPART1 *pcmpart1, void *pv, JOY *pjoy)
{
    UpdateCplcy(pcmpart1, pv, pjoy);
}

// CPLOOK ctor (vtable 0x2CD4E0): rZoomMax(+0x28)=7.0, psndZoom/zoomStopSound = 0.
extern "C" CPLOOK *CPLOOK_ctor(CPLOOK *pcplook)
{
    CPLCY_ctor(pcplook);
    SLY2_SET_VPTR(pcplook, CPLOOK);
    pcplook->rZoomMax = 7.0f;
    pcplook->psndZoom.m_p = 0;
    pcplook->zoomStopSound.m_p = 0;
    return pcplook;
}

// Resets the look policy's zoom: zoom amount and its speed to 0, and the camera field of view back to the policy's
// zoom field of view (zoomedFieldOfView).
// Also known as: ResetZoom (upstream readable / Sly 1 name).
extern "C" void ResetCplookZoom(CPLOOK *pcplook)
{
    pcplook->zoom = 0.0f;
    pcplook->svuZoom = 0.0f;
    SetCmFov(&g_cm, pcplook->zoomedFieldOfView);
}

namespace M_PosCplookAnchor {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
extern qword g_normalZ __asm__("g_normalZ");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int k) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// Look anchor: focus object position (0x1A62E8), +150 along up if it is a STEPPLAYER. (inferred)
// Also known as: PosFocus (upstream readable / Sly 1 name).
extern "C" qword PosCplookAnchor(CPLOOK *p)
{
    U s;
    U pos;
    pos.q = GetLoWorldPos(p->pvHero);
    if (IsBasicDerivedFrom__FP5BASIC3CID(p->pvHero, 0x34)) {
        register qword rs __asm__("vf2");
        register qword rp __asm__("vf3");
        register qword rd __asm__("vf1");
        rp = pos.q;
        rd = g_normalZ;
        __asm__("qmtc2.ni %1, %0" : "=j"(rs) : "r"(150.0f));
        s.q = rs;
        __asm__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %2, %1x" : "+j"(rp) : "j"(rs), "j"(rd));
        pos.q = rp;
    }
    return pos.q;
}
} // namespace M_PosCplookAnchor

namespace M_PosCplookEye {
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
extern qword D_002A5000 __asm__("D_002A5000");
void LoadRotateMatrixPanTiltP(M3 *pm, float x, float y) __asm__("LoadRotateMatrixPanTilt");
qword PosCplookAnchor(CPLOOK *p) __asm__("PosCplookAnchor");
// Eye = anchor + RotMatrix(panAngle +0x10, tiltAngle +0x14) * D_002A5000 offset. (inferred)
// Also known as: PosEye (upstream readable / Sly 1 name).
extern "C" qword PosCplookEye(CPLOOK *p)
{
    M3 m;
    QWROW v;
    qword pos;
    /* pinned $v0 for address and loaded value, like the target */
    { register qword *pa __asm__("$2") = &D_002A5000; register qword t __asm__("$2") = *pa; v.q = t; }
    LoadRotateMatrixPanTiltP(&m, p->panAngle, p->tiltAngle);
    pos = PosCplookAnchor(p);
    {
        register qword r1 __asm__("vf2") = m.r[1];
        register qword r0 __asm__("vf3") = m.r[0];
        register qword r2 __asm__("vf1") = m.r[2];
        register qword rv __asm__("vf4") = v.q;
        register qword rp __asm__("vf2");
        __asm__("vmulax.xyz $ACC, %1, %3x\n\tvmadday.xyz $ACC, %2, %3y\n\tvmaddz.xyz %0, %0, %3z"
                : "+j"(r2) : "j"(r0), "j"(r1), "j"(rv));
        rp = pos;
        __asm__("vadd.xyzw %0, %0, %1" : "+j"(rp) : "j"(r2));
        return rp;
    }
}
} // namespace M_PosCplookEye

namespace M_ActivateCplook {
extern LM D_002BD950 __asm__("D_002BD950"), D_002BD958 __asm__("D_002BD958"), D_002BD960 __asm__("D_002BD960"), D_002BD968 __asm__("D_002BD968"), D_002BD970 __asm__("D_002BD970");
void ActivateCplcy(CPLOOK *p) __asm__("ActivateCplcy");
void *CreateSfxSound(int) __asm__("CreateSfxSound");
void SetSndSourceLo(void *, int) __asm__("SetSndSourceLo");
void RcPtr_Assign(CRefPtr<SND> *, void *) __asm__("RcPtr_Assign");
void PauseSnd(SND *) __asm__("PauseSnd");
void DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(void *, float *, float *) __asm__("DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1");
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void *GetLoWorldMatrix(void *) __asm__("GetLoWorldMatrix");
float ClampToLimits__FP2LMf(LM *, float) __asm__("ClampToLimits__FP2LMf");

// Binocucom/look mode enter: start zoom sounds 0x26/0x25, pan/tilt from current camera (or focus), pan/tilt limits,
// zoom=0, fov zoom range, near clip 49, numActivations++.
// Also known as: Activate (upstream readable / Sly 1 name; Activate__7CPRECONP2LOPv).
extern "C" void ActivateCplook(CPLOOK *p)
{
    CM *pcm;
    void *pv;
    float g;
    ActivateCplcy(p);
    pv = CreateSfxSound(0x26);
    SetSndSourceLo(pv, 0);
    RcPtr_Assign(&p->psndZoom, pv);
    {
        void *pv2 = CreateSfxSound(0x25);
        SetSndSourceLo(pv2, 0);
        RcPtr_Assign(&p->zoomStopSound, pv2);
    }
    PauseSnd(p->psndZoom.m_p);
    pcm = &g_cm;
    DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(&pcm->mat, &p->panAngle, &p->tiltAngle);
    if (IsBasicDerivedFrom__FP5BASIC3CID(p->pvHero, 0x34)) {
        p->lmPan = D_002BD950;
        p->lmTilt = D_002BD958;
    } else {
        p->lmPan = D_002BD968;
        p->lmTilt = D_002BD970;
        p->panStartAngle = p->panAngle;
        p->tiltStartAngle = p->tiltAngle;
        DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(GetLoWorldMatrix(p->pvHero), &p->panAngle, &p->tiltAngle);
        g = ClampToLimits__FP2LMf(&D_002BD960, p->tiltAngle);
        p->tiltAngle = g;
        p->lmPan.minValue += p->panAngle;
        p->lmPan.maxValue += p->panAngle;
        p->lmTilt.minValue += g;
        p->lmTilt.maxValue += g;
        p->lmTilt.minValue = ClampToLimits__FP2LMf(&D_002BD960, p->tiltAngle);
        p->lmTilt.maxValue = ClampToLimits__FP2LMf(&D_002BD960, p->tiltAngle);
    }
    p->swPan = 0.0f;
    p->swTilt = 0.0f;
    p->zoom = 0.0f;
    p->svuZoom = 0.0f;
    p->zoomedFieldOfView = g_cm.targetFieldOfView / p->rZoomMax;
    p->wideFieldOfView = g_cm.targetFieldOfView;
    SetCmNearClip(&g_cm, 49.0f);
    p->numActivations++;
    g_sw.tLookLast = g_clock.time;
}
} // namespace M_ActivateCplook

namespace M_UpdateCplook {
typedef int qword __attribute__((mode(TI)));
struct POLY { float c0, c1, c2; };
extern struct POLY D_002BD980 __asm__("D_002BD980");
extern float D_002BD990 __asm__("D_002BD990"), D_002BD994 __asm__("D_002BD994"), D_002BD998 __asm__("D_002BD998"), D_002BD99C __asm__("D_002BD99C");
extern char D_002BD9A0[] __asm__("D_002BD9A0");
extern struct LM D_002BD9B0 __asm__("D_002BD9B0");
extern char g_lmZeroOne[] __asm__("g_lmZeroOne"), g_lmNegOneOne[] __asm__("g_lmNegOneOne");
struct SND { char pad[0x4E]; short pauseCount; };   /* (inferred) > 0: paused */
void UpdateCplcy(CPLOOK *p, void *pv, struct JOY *pjoy) __asm__("UpdateCplcy");
float SmoothValue__FfffP3SMPPf(float gCur, float gTarget, float dt, void *psmp, float *pdgNext) __asm__("SmoothValue__FfffP3SMPPf");
float ClampToLimits__FP2LMf(void *p, float g) __asm__("ClampToLimits__FP2LMf");
void SetSndPitch(struct SND *p, float g) __asm__("SetSndPitch");
void SetSndVolume(struct SND *p, float g) __asm__("SetSndVolume");
void ResumeSnd(struct SND *p) __asm__("ResumeSnd");
void PauseSnd(struct SND *p) __asm__("PauseSnd");
void GetGlobalSaveVar(CRefData *, void *, void *) __asm__("GetGlobalSaveVar");
void Decref__4CRef(CRefData *) __asm__("Decref__4CRef");
float ClampAbs__Fff(float g, float gMax) __asm__("ClampAbs__Fff");
float NormalizeAngle__Ff(float g) __asm__("NormalizeAngle__Ff");
qword PosCplookEye(CPLOOK *p) __asm__("PosCplookEye");
qword PosCplookAnchor(CPLOOK *p) __asm__("PosCplookAnchor");
void SetCameraLookAt(CM *p, qword a, qword b, float c, float d) __asm__("SetCameraLookAt");


// Same idiom as sly2/util.h Clamp; kept local because the shared helper changes UpdateCplook (243/286 instructions off).
extern "C" { static __inline__ float ClampLocal(float g, float gMin, float gMax)
{
    if (g < gMin) g = gMin;
    else if (gMax < g) g = gMax;
    return g;
} }

// Look-mode update: zoom from joy (+0x1074) with sound pitch/volume, fov lerp, pan/tilt from sticks (invert option via
// splice 0x3D4A60), limits, SetCmLookAt(eye, anchor)
// Also known as: Update (upstream readable / Sly 1 name; Update__7CPRECONP6CPDEFIP5CMJOY).
extern "C" void UpdateCplook(CPLOOK *p, void *pv, struct JOY *pjoy)
{
    float x, u, gPan, gTilt, dt;
    int f;
    CRefData ref;
    UpdateCplcy(p, pv, pjoy);
    x = p->zoom;
    p->svuZoom = (D_002BD980.c0 + x * (D_002BD980.c1 + x * D_002BD980.c2)) * pjoy->dyRight * __builtin_fabsf(pjoy->dyRight);
    x += p->svuZoom * g_clock.deltaTime;
    p->zoom = x = ClampLocal(x, 0.0f, 1.0f);
    if (p->svuZoom != 0.0f && 0.0f < x && x < 1.0f) {
        struct SND *ps = (struct SND *)p->psndZoom.m_p;
        SetSndPitch(ps, ClampToLimits__FP2LMf(g_lmNegOneOne, __builtin_fabsf(p->svuZoom)));
        ps = (struct SND *)p->psndZoom.m_p;
        SetSndVolume(ps, ClampToLimits__FP2LMf(g_lmZeroOne, __builtin_fabsf(p->svuZoom) * 0.5f + 0.5f));
        if (((struct SND *)p->psndZoom.m_p)->pauseCount > 0)
            ResumeSnd((struct SND *)p->psndZoom.m_p);
    } else {
        if (((struct SND *)p->psndZoom.m_p)->pauseCount <= 0)
            PauseSnd((struct SND *)p->psndZoom.m_p);
    }
    u = 1.0f - p->zoom;
    SetCmFov(&g_cm, (1.0f - u) * p->zoomedFieldOfView + u * p->wideFieldOfView);
    gPan = -__builtin_fabsf(pjoy->leftStickX) * pjoy->leftStickX * D_002BD990;
    gTilt = -__builtin_fabsf(pjoy->leftStickY) * pjoy->leftStickY * D_002BD994;
    GetGlobalSaveVar(&ref, g_sw.psvpReverseLookAround, &g_gs);
    f = ref.u.n;
    Decref__4CRef(&ref);
    if (f)
        gTilt = -gTilt;
    gPan = p->swPan + ClampAbs__Fff(gPan - p->swPan, D_002BD998 * g_clock.deltaTime);
    gTilt = p->swTilt + ClampAbs__Fff(gTilt - p->swTilt, D_002BD99C * g_clock.deltaTime);
    dt = g_clock.deltaTime * 0.5f;
    {
        float gMin, gMax;
        SmoothValue__FfffP3SMPPf(p->panAngle, p->lmPan.minValue, dt, D_002BD9A0, &gMin);
        SmoothValue__FfffP3SMPPf(p->panAngle, p->lmPan.maxValue, dt, D_002BD9A0, &gMax);
        p->swPan = Clamp(gPan, gMin, gMax);
        SmoothValue__FfffP3SMPPf(p->tiltAngle, p->lmTilt.minValue, dt, D_002BD9A0, &gMin);
        SmoothValue__FfffP3SMPPf(p->tiltAngle, p->lmTilt.maxValue, dt, D_002BD9A0, &gMax);
        p->swTilt = Clamp(gTilt, gMin, gMax);
    }
    {
        float k = (1.0f - g_cm.fieldOfView) * D_002BD9B0.minValue + g_cm.fieldOfView * D_002BD9B0.maxValue;
        p->panAngle += k * p->swPan * g_clock.deltaTime;
        p->tiltAngle += k * p->swTilt * g_clock.deltaTime;
    }
    if (6.2831855f <= p->lmPan.maxValue - p->lmPan.minValue)
        p->panAngle = NormalizeAngle__Ff(p->panAngle);
    SetCameraLookAt(&g_cm, PosCplookEye(p), PosCplookAnchor(p), 0.0f, 0.0f);
    g_sw.tLookLast = g_clock.time;
}
} // namespace M_UpdateCplook

// ResetCplook (vtable slot 7, the policy reset; CPLCY's own slot 7 is the empty ResetCplcy): releases the two
// sound references zoomStopSound and psndZoom. (inferred)
void CPLOOK::ResetPolicy()   /* key function */
{
    CPLOOK *pcplook = this;
    RcPtr_Assign(&pcplook->zoomStopSound, 0);
    RcPtr_Assign(&pcplook->psndZoom, 0);
}

namespace M_DeactivateCplook {
void StopSnd(SND *p, float g) __asm__("StopSnd");
void RcPtr_Assign(CRefPtr<SND> *pp, int n) __asm__("RcPtr_Assign");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetCmCut(CM *p, float a, float b) __asm__("SetCmCut");
void DeactivateCplcy(CPLOOK *a, int n, void *b) __asm__("DeactivateCplcy");
// Stop the sound (fading over g) and drop the reference, if any.
extern "C" { static inline void Release(CRefPtr<SND> *pp, float g)
{
    if (pp && pp->m_p) {
        StopSnd(pp->m_p, g);
        RcPtr_Assign(pp, 0);
    }
} }
// Stop/release zoom sounds, SetCmCut with pan+PI/-tilt (camera continues behind), then CPLCY::Deactivate.
// Also known as: Deactivate (upstream readable / Sly 1 name; Deactivate__7CPRECONP2LOPv).
extern "C" void DeactivateCplook(CPLOOK *a, int n, void *b)
{
    Release(&a->psndZoom, 0.0f);
    Release(&a->zoomStopSound, 0.5f);
    if (IsBasicDerivedFrom__FP5BASIC3CID(a->pvHero, 0x34))
        SetCmCut(&g_cm, a->panAngle + 3.1415927f, -a->tiltAngle);
    else
        SetCmCut(&g_cm, a->panStartAngle + 3.1415927f, -a->tiltStartAngle);
    DeactivateCplcy(a, n, b);
}
} // namespace M_DeactivateCplook

// CPZOOM ctor (vtable 0x2CD498): rZoomMax(+0xB4)=7.0, zoomTarget(+0x18)=-1, sounds 0.
extern "C" CPZOOM *CPZOOM_ctor(CPZOOM *pcpzoom)
{
    CPLCY_ctor(pcpzoom);
    SLY2_SET_VPTR(pcpzoom, CPZOOM);
    pcpzoom->rZoomMax = 7.0f;
    pcpzoom->zoomTarget = -1.0f;
    pcpzoom->psndZoom.m_p = 0;
    pcpzoom->zoomStopSound.m_p = 0;
    return pcpzoom;
}

namespace M_PosCpzoomCenter {
typedef int qword __attribute__((mode(TI)));
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void *GetCurrentPo(void *p) __asm__("GetCurrentPo");
// Center: ploOwner(+0x10) pos, else player (0x1C4A48) pos +150 z, else camera pos. (inferred)
// Also known as: PosFocus (upstream readable / Sly 1 name).
extern "C" qword PosCpzoomCenter(CPZOOM *p)
{
    QWROW v;
    if (p->ploOwner) {
        v.q = GetLoWorldPos(p->ploOwner);
    } else {
        void *o = GetCurrentPo(&g_sw);
        if (o) {
            v.q = GetLoWorldPos(o);
            v.f[2] += 150.0f;
        } else {
            v.q = g_cm.matq.pos;
        }
    }
    return v.q;
}
} // namespace M_PosCpzoomCenter

namespace M_PosCpzoomEye {
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
extern qword D_002A5000 __asm__("D_002A5000");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword PosCpzoomCenter(CPZOOM *p) __asm__("PosCpzoomCenter");
float atan2f(float y, float x) __asm__("atan2f");
void LoadRotateMatrixPanTilt(M3 *pm, float a, float b) __asm__("LoadRotateMatrixPanTilt");
// Length of the xyz part of v (x lane).
extern "C" { static inline qword Len(qword v)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v), "j"(v));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(d) : "j"(d));
    return d;
} }
// Unit vector along v.
extern "C" { static inline qword Normalize(qword v)
{
    qword l = Len(v), r;
    __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
} }
// Return v scaled by g.
extern "C" { static inline qword VecMul(qword v, float g)
{
    qword r;
    QWROW s;
    qword t;
    __asm__("qmtc2.ni %1, %0" : "=j"(t) : "r"(g));
    s.q = t;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
    return r;
} }
// Rotate v by the 3x3 matrix pm.
extern "C" { static inline qword MulM3(M3 *pm, qword v)
{
    qword r;
    __asm__("vmulax.xyz ACC, %2, %1x\n\tvmadday.xyz ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z" : "=j"(r) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]));
    return r;
} }
// Eye: if ploFocus(+0x14), behind center looking at focus (updates panAngle +0xA4); else center +
// Rot(panAngle,tiltAngle)*offset. (inferred)
// Also known as: PosEye (upstream readable / Sly 1 name).
extern "C" qword PosCpzoomEye(CPZOOM *p)
{
    qword r;
    if (p->ploFocus) {
        QWROW n;
        qword a = GetLoWorldPos(p->ploFocus);
        n.q = Normalize(VuSub(a, PosCpzoomCenter(p)));
        p->panAngle = atan2f(n.f[1], n.f[0]);
        r = VuAdd(PosCpzoomCenter(p), VecMul(n.q, -VuToFloat(Len(D_002A5000))));
    } else {
        M3 m;
        qword v = D_002A5000;
        LoadRotateMatrixPanTilt(&m, p->panAngle, p->tiltAngle);
        r = VuAdd(PosCpzoomCenter(p), MulM3(&m, v));
    }
    return r;
}
} // namespace M_PosCpzoomEye

namespace M_ActivateCpzoom {
void ActivateCplcy(CPZOOM *a, int n) __asm__("ActivateCplcy");
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
void RcPtr_Assign(CRefPtr<SND> *pp, void *p) __asm__("RcPtr_Assign");
void PauseSnd(SND *p) __asm__("PauseSnd");
void DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(void *pmat, float *pa, float *pb) __asm__("DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1");
// Zoom-cam enter: zoom sounds 0x26/0x25, pan/tilt from current camera mat, fov range, near clip 49, SnapCpzoom, shares
// numActivations counter (g_cm+0x4A4)
// Also known as: Activate (upstream readable / Sly 1 name; Activate__6CPFREEP2LOPv).
extern "C" void ActivateCpzoom(CPZOOM *a, int n)
{
    void *p;
    CM *pg;
    ActivateCplcy(a, n);
    p = CreateSfxSound(0x26);
    SetSndSourceLo(p, 0);
    RcPtr_Assign(&a->psndZoom, p);
    p = CreateSfxSound(0x25);
    SetSndSourceLo(p, 0);
    RcPtr_Assign(&a->zoomStopSound, p);
    PauseSnd(a->psndZoom.m_p);
    __asm__ __volatile__("" : : : "$6"); /* a2 is dead here: delays the a2 setup like the original */
    pg = &g_cm;
    DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(&pg->mat, &a->panAngle, &a->tiltAngle);
    a->tiltAngle = 0;
    a->zoom = 0.0f;
    a->svuZoom = 0.0f;
    a->zoomedFieldOfView = pg->targetFieldOfView / a->rZoomMax;
    a->wideFieldOfView = pg->targetFieldOfView;
    SetCmNearClip(pg, 49.0f);
    SnapCpzoom(a);
    pg->cplook.numActivations++;
    g_sw.tLookLast = g_clock.time;
}
} // namespace M_ActivateCpzoom

namespace M_UpdateCpzoom {
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef union { struct { qword r[4]; } q; M3 m3; float af[4][4]; } M4;
extern char D_002BD9B8[] __asm__("D_002BD9B8"), D_002BD9C8[] __asm__("D_002BD9C8"), D_002BD9D8[] __asm__("D_002BD9D8"), D_002BD9E8[] __asm__("D_002BD9E8"), D_002BD9F8[] __asm__("D_002BD9F8");
extern char g_lmZeroOne[] __asm__("g_lmZeroOne"), g_lmNegOneOne[] __asm__("g_lmNegOneOne");
struct SND { char pad[0x4E]; short pauseCount; };   /* (inferred) > 0: paused */
void UpdateCplcy(CPZOOM *p) __asm__("UpdateCplcy");
float SmoothValue__FfffP3SMPPf(void *p, float *pdg, float g, float gTarget, float dt) __asm__("SmoothValue__FfffP3SMPPf");
float ClampToLimits__FP2LMf(void *p, float g) __asm__("ClampToLimits__FP2LMf");
void SetSndPitch(struct SND *p, float g) __asm__("SetSndPitch");
void SetSndVolume(struct SND *p, float g) __asm__("SetSndVolume");
void ResumeSnd(struct SND *p) __asm__("ResumeSnd");
void PauseSnd(struct SND *p) __asm__("PauseSnd");
qword PosSmooth(qword a, qword b, float g, void *p, int n) __asm__("PosSmooth");
void LoadRotateMatrixEuler(M3 *pm, qword v) __asm__("LoadRotateMatrixEuler");
void SmoothMatrix(M3 *a, M3 *b, void *p, M3 *out, int n, float g) __asm__("SmoothMatrix");
qword DecomposeRotationMatrixToEuler(M3 *pm) __asm__("DecomposeRotationMatrixToEuler");
float SmoothAngle__FfffP3SMPPf(void *p, int n, float a, float b, float c) __asm__("SmoothAngle__FfffP3SMPPf");
qword *VECTOR_opAssign(qword *p, qword v) __asm__("VECTOR_opAssign");
void SetCmMat4(CM *p, M4 *m) __asm__("SetCmMat4");
void DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(M4 *pm, float *pa, float *pb) __asm__("DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1");
qword PosCpzoomEye(CPZOOM *p) __asm__("PosCpzoomEye");
qword PosCpzoomCenter(CPZOOM *p) __asm__("PosCpzoomCenter");
void SetCameraLookAt(CM *p, qword a, qword b, float c, float d) __asm__("SetCameraLookAt");


// Fraction of g between a and b ((g - a) / (b - a)); 0 when a == b.
extern "C" { static __inline__ float GFrac(float g, float a, float b)
{
    if (a == b) return 0.0f;
    return (g - a) / (b - a);
} }

// Smooth zoom toward zoomTarget with sounds; if posed (hasPose +0x40) blend pos/eul/fov to target pose, else smooth
// center/eye and SetCmLookAt.
// Also known as: Update (upstream readable / Sly 1 name; Update__6CPFREEP6CPDEFIP5CMJOY).
extern "C" void UpdateCpzoom(CPZOOM *p)
{
    UpdateCplcy(p);
    if (g_clock.deltaTime == 0.0f)
        return;
    if (0.0f <= p->zoomTarget) {
        float g = Clamp(SmoothValue__FfffP3SMPPf(D_002BD9B8, 0, p->zoom, p->zoomTarget, g_clock.deltaTime), 0.0f, 1.0f);
        p->svuZoom = (g - p->zoom) / g_clock.deltaTime;
        p->zoom = g;
    }
    if (p->svuZoom != 0.0f) {
        struct SND *ps = (struct SND *)p->psndZoom.m_p;
        SetSndPitch(ps, ClampToLimits__FP2LMf(g_lmNegOneOne, __builtin_fabsf(p->svuZoom)));
        ps = (struct SND *)p->psndZoom.m_p;
        SetSndVolume(ps, ClampToLimits__FP2LMf(g_lmZeroOne, __builtin_fabsf(p->svuZoom) * 0.5f + 0.5f));
        if (((struct SND *)p->psndZoom.m_p)->pauseCount > 0)
            ResumeSnd((struct SND *)p->psndZoom.m_p);
    } else {
        if (((struct SND *)p->psndZoom.m_p)->pauseCount <= 0)
            PauseSnd((struct SND *)p->psndZoom.m_p);
    }
    if (p->hasPose) {
        M3 m0, m30, m60, m90;
        M4 m4;
        float u;
        *(qword *)&p->pos = PosSmooth(*(qword *)&p->pos, *(qword *)&p->targetEyePosition, g_clock.deltaTime, D_002BD9C8, 0);
        LoadRotateMatrixEuler(&m90, *(qword *)&p->eul);
        m0 = m90;
        LoadRotateMatrixEuler(&m90, *(qword *)&p->eulTarget);
        m30 = m90;
        SmoothMatrix(&m0, &m30, D_002BD9D8, &m60, 0, g_clock.deltaTime);
        *(qword *)&p->eul = DecomposeRotationMatrixToEuler(&m60);
        p->fieldOfView = SmoothAngle__FfffP3SMPPf(D_002BD9E8, 0, p->fieldOfView, p->targetFieldOfView, g_clock.deltaTime);
        {
        M4 *pm4 = &m4;
        VECTOR_opAssign(&pm4->q.r[3], *(qword *)&p->pos);
        pm4->q.r[0] = m60.r[0];
        pm4->q.r[1] = m60.r[1];
        pm4->q.r[2] = m60.r[2];
        m4.af[2][3] = 0.0f;
        m4.af[1][3] = 0.0f;
        m4.af[0][3] = 0.0f;
        pm4->af[3][3] = 1.0f;
        SetCmMat4(&g_cm, pm4);
        SetCmFov(&g_cm, p->fieldOfView);
        DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(pm4, &p->panAngle, &p->tiltAngle);
        }
        p->zoom = ClampToLimits__FP2LMf(g_lmZeroOne, GFrac(p->fieldOfView, p->zoomedFieldOfView, p->wideFieldOfView));
    } else {
        float u = 1.0f - p->zoom;
        SetCmFov(&g_cm, (1.0f - u) * p->zoomedFieldOfView + u * p->wideFieldOfView);
        {
            qword a = *(qword *)&p->smoothedEyePosition;
            *(qword *)&p->smoothedEyePosition = PosSmooth(a, PosCpzoomEye(p), g_clock.deltaTime, D_002BD9F8, 0);
        }
        {
            qword a = *(qword *)&p->smoothedFocusPosition;
            *(qword *)&p->smoothedFocusPosition = PosSmooth(a, PosCpzoomCenter(p), g_clock.deltaTime, D_002BD9F8, 0);
        }
        SetCameraLookAt(&g_cm, *(qword *)&p->smoothedEyePosition, *(qword *)&p->smoothedFocusPosition, 0.0f, 0.0f);
    }
    g_sw.tLookLast = g_clock.time;
}
} // namespace M_UpdateCpzoom

// ResetCpzoom (vtable slot 7, the policy reset; CPLCY's own slot 7 is the empty ResetCplcy): releases the two
// sound references zoomStopSound (+0xC4) and psndZoom (+0xC0). (inferred)
void CPZOOM::ResetPolicy()   /* key function */
{
    CPZOOM *pcpzoom = this;
    RcPtr_Assign(&pcpzoom->zoomStopSound, 0);
    RcPtr_Assign(&pcpzoom->psndZoom, 0);
}

// PloOwner(+0x10)=plo; if posed, SnapCpzoom; hasPose=0. (inferred)
// Also known as: SetFocusObject (upstream readable / Sly 1 name; SetFocusObject__6CPFREEP2LO).
extern "C" void SetCpzoomOwner(CPZOOM *pcpzoom, LO *plo)
{
    pcpzoom->ploOwner = plo;
    if (pcpzoom->hasPose != 0) {
        SnapCpzoom(pcpzoom);
    }
    pcpzoom->hasPose = 0;
}

// PloFocus(+0x14)=plo; if posed, SnapCpzoom; hasPose=0. (inferred)
extern "C" void SetCpzoomFocus(CPZOOM *pcpzoom, LO *plo)
{
    pcpzoom->ploFocus = plo;
    if (pcpzoom->hasPose != 0) {
        SnapCpzoom(pcpzoom);
    }
    pcpzoom->hasPose = 0;
}

// PosTarget(+0x80), eulTarget(+0x90), targetFieldOfView(+0xA0); if not posed SnapCpzoom; hasPose=1. (inferred)
// Also known as: SetTargets (upstream readable / Sly 1 name; SetTargets__6CPFREEG9VU_VECTORT1f).
extern "C" void SetCpzoomPose(CPZOOM *pcpzoom, qword pos, qword eul, float radFov)
{
    *(qword *)&pcpzoom->targetEyePosition = pos;
    *(qword *)&pcpzoom->eulTarget = eul;
    pcpzoom->targetFieldOfView = radFov;
    if (pcpzoom->hasPose == 0) {
        SnapCpzoom(pcpzoom);
    }
    pcpzoom->hasPose = 1;
}

// UZoomTarget(+0x18)=g.
extern "C" void SetCpzoomUZoomTarget(CPZOOM *pcpzoom, float uZoomTarget)
{
    pcpzoom->zoomTarget = uZoomTarget;
}

namespace M_SnapCpzoom {
typedef int qword __attribute__((mode(TI)));
qword PosCpzoomCenter(CPZOOM *p) __asm__("PosCpzoomCenter");
qword PosCpzoomEye(CPZOOM *p) __asm__("PosCpzoomEye");
// Reset smoothed center/eye to current targets, zoom=zoomTarget (if >=0), pose = target pose.
// Also known as: Snap (upstream readable / Sly 1 name).
extern "C" void SnapCpzoom(CPZOOM *p)
{
    *(qword *)&p->smoothedFocusPosition = PosCpzoomCenter(p);
    *(qword *)&p->smoothedEyePosition = PosCpzoomEye(p);
    if (0.0f <= p->zoomTarget)
        p->zoom = p->zoomTarget;
    *(qword *)&p->pos = *(qword *)&p->targetEyePosition;
    *(qword *)&p->eul = *(qword *)&p->eulTarget;
    p->fieldOfView = p->targetFieldOfView;
}
} // namespace M_SnapCpzoom

namespace M_HasZoomCenterArrived {
typedef int qword __attribute__((mode(TI)));
qword PosCpzoomCenter(CPZOOM *p) __asm__("PosCpzoomCenter");
int AreFloatsNear__Ffff(float a, float b, float eps) __asm__("AreFloatsNear__Ffff");
// True when a and b are within 0.0001.
extern "C" { static __inline__ int FNear(float a, float b) { return AreFloatsNear__Ffff(a, b, 0.0001f); } }
// Smoothed center (+0x20) == PosCpzoomCenter within 1e-4 per axis. (inferred)
extern "C" int HasZoomCenterArrived(CPZOOM *p)
{
    int f = 0;
    qword a = *(qword *)&p->smoothedFocusPosition;
    qword b = PosCpzoomCenter(p);
    if (FNear(QwordX(a), QwordX(b)))
        if (FNear(QwordY(a), QwordY(b)))
            if (FNear(QwordZ(a), QwordZ(b)))
                f = 1;
    return f;
}
} // namespace M_HasZoomCenterArrived

namespace M_HasZoomEyeArrived {
typedef int qword __attribute__((mode(TI)));
qword PosCpzoomEye(CPZOOM *p) __asm__("PosCpzoomEye");
int AreFloatsNear__Ffff(float a, float b, float eps) __asm__("AreFloatsNear__Ffff");
// True when a and b are within 0.0001.
extern "C" { static __inline__ int FNear(float a, float b) { return AreFloatsNear__Ffff(a, b, 0.0001f); } }
// Smoothed eye (+0x30) == PosCpzoomEye within 1e-4. (inferred)
extern "C" int HasZoomEyeArrived(CPZOOM *p)
{
    int f = 0;
    qword a = *(qword *)&p->smoothedEyePosition;
    qword b = PosCpzoomEye(p);
    if (FNear(QwordX(a), QwordX(b)))
        if (FNear(QwordY(a), QwordY(b)))
            if (FNear(QwordZ(a), QwordZ(b)))
                f = 1;
    return f;
}
} // namespace M_HasZoomEyeArrived

namespace M_HasZoomReachedTarget {
int AreFloatsNear__Ffff(float a, float b, float eps) __asm__("AreFloatsNear__Ffff");
// UZoomTarget < 0 or zoom ~= zoomTarget (0.01) (inferred)
extern "C" int HasZoomReachedTarget(CPZOOM *p)
{
    int f = 0;
    if (p->zoomTarget < 0.0f || AreFloatsNear__Ffff(p->zoom, p->zoomTarget, 0.01f))
        f = 1;
    return f;
}
} // namespace M_HasZoomReachedTarget

namespace M_DeactivateCpzoom {
void StopSnd(SND *p, float g) __asm__("StopSnd");
void RcPtr_Assign(CRefPtr<SND> *pp, int n) __asm__("RcPtr_Assign");
void DeactivateCplcy(CPZOOM *a, int n, void *b) __asm__("DeactivateCplcy");
// Stop the sound (fading over g) and drop the reference, if any.
extern "C" { static inline void Release(CRefPtr<SND> *pp, float g)
{
    if (pp && pp->m_p) {
        StopSnd(pp->m_p, g);
        RcPtr_Assign(pp, 0);
    }
} }
// Release sounds, clear owner/focus/pose, zoomTarget=-1, CutCm, CPLCY::Deactivate.
// Also known as: Deactivate (upstream readable / Sly 1 name; Deactivate__6CPFREEP2LOPv).
extern "C" void DeactivateCpzoom(CPZOOM *a, int n, void *b)
{
    Release(&a->psndZoom, 0.0f);
    Release(&a->zoomStopSound, 0.5f);
    a->ploOwner = 0;
    a->ploFocus = 0;
    a->hasPose = 0;
    a->zoomTarget = -1.0f;
    CutCm(&g_cm);
    DeactivateCplcy(a, n, b);
}
} // namespace M_DeactivateCpzoom

// CPHANG ctor (vtable 0x2CD450)
extern "C" CPHANG *CPHANG_ctor(CPHANG *pcphang)
{
    CPLCY_ctor(pcphang);
    SLY2_SET_VPTR(pcphang, CPHANG);
    return pcphang;
}

namespace M_ActivateCphang {
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void ActivateCplcy(CPHANG *a, int n, void *b) __asm__("ActivateCplcy");
ACT1 *NewPositionAct(void *pvHero, XF *pxf, int a, int b, int c, qword q) __asm__("NewPositionAct");
// Store pv (+0x10), near clip 25, fov 1.15, +0x14 = 0x12DC18(owner, pv, 1, 8, 3, 0), nextSwayTime=-1. (inferred)
// Also known as: Activate (upstream readable / Sly 1 name; Activate__10CPACTUATEDP2LOPv).
extern "C" void ActivateCphang(CPHANG *a, int n, void *b)
{
    CM *pg;
    ActivateCplcy(a, n, b);
    a->pxfCam = (XF *)b;
    pg = &g_cm;
    SetCmNearClip(pg, 25.0f);
    SetCmFov(pg, 1.15f);
    a->pact1 = NewPositionAct(a->pvHero, a->pxfCam, 1, 8, 3, D_002A2DD0);
    a->nextSwayTime = -1.0f;
}
} // namespace M_ActivateCphang

// 0x12E460(+0x14), CutCm, CPLCY::Deactivate. (inferred)
// Also known as: Deactivate (upstream readable / Sly 1 name; Deactivate__10CPACTUATEDP2LOPv).
void CPHANG::Deactivate(void *pvHero, void *pv)   /* key function */
{
    CPHANG *pcphang = this;
    ReleaseAct(&pcphang->pact1);
    CutCm(&g_cm);
    DeactivateCplcy(pcphang, pvHero, pv);
}

namespace M_UpdateCphang {
// UpdateCphang (Lilly's agent, T1649): joystick / random-target camera spring. MATCH.
// Keys: the sibling helper style of UpdateCpcamblend / FindCmClosestClearPos (by-value VU_FLOAT temps -> the sqc2 0(sp) after each qmtc2,
// VU_VECTOR(const qword &) wrappers, VecCombo(f1, v1, f2, v2)); Sub taking VU_VECTORs makes both p14->pos reads plain MEMs,
// so cse2 keeps the target's two lqc2 0x20(v0); k is read through a VU_FLOAT(const float *) ctor whose word input has a
// never-chosen "V" alternative (the ClipCameraEye trick: lw instead of lwc1+mfc1).
typedef int qword __attribute__((mode(TI)));
struct CLQ { float g0, g1, g2, pad; };
extern CLQ D_002BDA10 __asm__("D_002BDA10");
extern LM D_002BDA20 __asm__("D_002BDA20");
extern CLQ D_002BDA30 __asm__("D_002BDA30");
extern LM D_002BDA40 __asm__("D_002BDA40");
extern LM D_002BDA48 __asm__("D_002BDA48"), D_002BDA50 __asm__("D_002BDA50"), D_002BDA58 __asm__("D_002BDA58");
extern float D_002BDA60 __asm__("D_002BDA60"), D_002BDA64 __asm__("D_002BDA64");
struct GS { char pad[0x4CC]; int unknown4CC; };
struct OBJ { char pad[0x40]; int matrix; };   /* the camera XF: its matrix at +0x40 */

void UpdateCplcy(CPHANG *p, void *pv, JOY *pjoy) __asm__("UpdateCplcy");
float RandomFloatInRange__Fff(float gMin, float gMax) __asm__("RandomFloatInRange__Fff");
void SetActPositionGoal(ACT1 *p, qword pos) __asm__("SetActPositionGoal");
void SetCmMat4(CM *p, int *pm) __asm__("SetCmMat4");
void UpdateCphang(CPHANG *p, void *pv, JOY *pjoy) __asm__("UpdateCphang");

// Evaluate the quadratic g0 + g1*g + g2*g^2.
static inline float GFromClq(const CLQ &c, float g) { return c.g0 + g * (c.g1 + g * c.g2); }
// Random float in the range [minValue, maxValue].
static inline float GRand(const LM &lm) { return RandomFloatInRange__Fff(lm.minValue, lm.maxValue); }
// Pack x, y, z into a 128-bit GPR value.
static inline qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__ __volatile__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
// Zero vector.
static inline qword Zero() { qword q; __asm__ __volatile__("vsub.xyzw %0, $vf0, $vf0" : "=j"(q)); return q; }
// Float broadcast into a VU0 register, from a value or from memory.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j,j"(q) : "r,V"(*(const int *)pg)); } /* never-chosen "V" alternative: allocation-only */ };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
// Return a - b.
static inline qword Sub(VU_VECTOR a, VU_VECTOR b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a.q), "j"(b.q)); return d; }
// Return v1*f1 + v2*f2.
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
// Return a + b * s.
static inline qword AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(s.q));
    return r;
}
// Return a + (b + c) * s.
static inline qword AddScaled2(VU_VECTOR a, VU_VECTOR b, VU_VECTOR c, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %4x\n\tvmaddx.xyzw %0, %3, %4x" : "=j"(r) : "j"(a.q), "j"(b.q), "j"(c.q), "j"(s.q));
    return r;
}

// Sway spring: stick deflection or, in state 9, random targets every few seconds; moves target XF (0x12D108) and
// SetCmMat4 from the camera transform.
// Also known as: Update (upstream readable / Sly 1 name; Update__10CPACTUATEDP6CPDEFIP5CMJOY).
void UpdateCphang(CPHANG *p, void *pv, JOY *pjoy)
{
    qword pos, v;
    UpdateCplcy(p, pv, pjoy);
    if (0.0f < pjoy->rightStickDeflection) {
        float sx, sy;
        sx = 1.0f;
        if (0.0f < pjoy->dxRight)
            sx = -1.0f;
        sy = -1.0f;
        if (0.0f < pjoy->dyRight)
            sy = 1.0f;
        float ax = sx * Clamp(GFromClq(D_002BDA10, __builtin_fabsf(pjoy->dxRight)), D_002BDA20.minValue, D_002BDA20.maxValue);
        float ay = Clamp(GFromClq(D_002BDA30, __builtin_fabsf(pjoy->dyRight)), D_002BDA40.minValue, D_002BDA40.maxValue);
        float y = sy * ay;
        v = Zero();
        union { qword q; } u;
        u.q = Make(0.0f, y, ax);
        pos = u.q;
    } else if (((GS *)p->pvHero)->unknown4CC != 9) {
        qword zero = Zero();
        pos = zero;
        v = pos;
    } else {
        if (p->nextSwayTime < g_clock.time) {
            float y = GRand(D_002BDA48);
            float z = GRand(D_002BDA50);
            float *pf = &p->swayOffset.x;
            pf[0] = 0.0f;
            pf[1] = y;
            pf[2] = z;
            float t = g_clock.time;
            p->nextSwayTime = t + GRand(D_002BDA58);
        }
        float dt = g_clock.deltaTime;
        qword accel = VecCombo(VU_FLOAT(&D_002BDA60), Sub(*(qword *)&p->swayOffset, p->pact1->goalValue), VU_FLOAT(-D_002BDA64), *(qword *)&p->vecVel);
        v = AddScaled(*(qword *)&p->vecVel, accel, VU_FLOAT(dt));
        pos = AddScaled2(p->pact1->goalValue, *(qword *)&p->vecVel, v, VU_FLOAT(dt * 0.5f));
    }
    SetActPositionGoal(p->pact1, pos);
    *(qword *)&p->vecVel = v;
    SetCmMat4(&g_cm, &((OBJ *)p->pxfCam)->matrix);
}
} // namespace M_UpdateCphang

// CPCAMBLEND ctor (vtable 0x2CD408) (inferred)
extern "C" CPCAMBLEND *CPCAMBLEND_ctor(CPCAMBLEND *pcpcamblend)
{
    CPLCY_ctor(pcpcamblend);
    SLY2_SET_VPTR(pcpcamblend, CPCAMBLEND);
    return pcpcamblend;
}

namespace M_ActivateCpcamblend {
void ActivateCplcy(CPCAMBLEND *a, int n, CAMMOVE *b) __asm__("ActivateCplcy");
} // namespace M_ActivateCpcamblend
// CPLCY::Activate; pcammove(+0x10)=pv; pevent->AddRef() (begin event). (inferred)
// Also known as: Activate (upstream readable / Sly 1 name; Activate__7CPTIMEDP2LOPv).
void CPCAMBLEND::Activate(void *pvHero, void *pv)   /* key function (the namespace's views) */
{
    using namespace M_ActivateCpcamblend;
    CPCAMBLEND *a = this;
    int n = (int)pvHero;
    CAMMOVE *b = (CAMMOVE *)pv;
    M_ActivateCpcamblend::ActivateCplcy(a, n, b);
    a->pcammove = b;
    ((BASIC *)b->pevent)->AddRef();
}

namespace M_DeactivateCpcamblend {
void DeactivateCplcy(CPCAMBLEND *a, int n, CAMMOVE *b) __asm__("DeactivateCplcy");
// CPLCY::Deactivate; CutCm if fCutOnLeave(+0xC); pevent->Release() (end event); pcammove=0. (inferred)
// Also known as: Deactivate (upstream readable / Sly 1 name; Deactivate__7CPTIMEDP2LOPv).
extern "C" void DeactivateCpcamblend(CPCAMBLEND *a, int n, CAMMOVE *b)
{
    DeactivateCplcy(a, n, b);
    if (b->fCutOnLeave)
        CutCm(&g_cm);
    ((BASIC *)a->pcammove->pevent)->Release();
    a->pcammove = 0;
}
} // namespace M_DeactivateCpcamblend

namespace M_UpdateCpcamblend {
// UpdateCpcamblend (Claude agent): camera path keyframe lerp. VU_FLOAT is a qmtc2 asm fed by a gcc mfc1, and Dot ends
// with separate qmfc2/mtc1 asms, so the float t lives only in an FPR and sub.s schedules like the original.

typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef union { struct { qword r[4]; } q; M3 m3; float af[4][4]; } M4;
extern qword D_002BDA70[] __asm__("D_002BDA70");
void UpdateCplcy(CPCAMBLEND *p) __asm__("UpdateCplcy");
void LoadRotateMatrixEuler(M3 *pm, qword v) __asm__("LoadRotateMatrixEuler");
qword *VECTOR_opAssign(qword *p, qword v) __asm__("VECTOR_opAssign");
void SetCmMat4(CM *p, M4 *m) __asm__("SetCmMat4");


// Dot product of the xyz parts of a and b as a float.
extern "C" { static __inline__ float Dot(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    int r;
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(r));
    return g;
} }

// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
// Return v1*f1 + v2*f2.
extern "C" { static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
} }


// T=(clock.time - modeStartTime)/dt; curve weight dot((t^3,t^2,t), D_002BDA70[iCurve]); lerp pos/eul/fov between the two
// CAMPOSEs; SetCmMat4 + SetCmFov.
// Also known as: Update (upstream readable / Sly 1 name; Update__7CPTIMEDP6CPDEFIP5CMJOY).
extern "C" void UpdateCpcamblend(CPCAMBLEND *p)
{
    float u, t, t1, fov;
    QWROW pos, rot;
    M4 m4;
    QWROW c;
    CAMMOVE *ppath;
    UpdateCplcy(p);
    u = g_clock.time - g_cm.modeStartTime;
    t = Clamp(u / p->pcammove->dt, 0.0f, 1.0f);
    u = t;
    c.f[1] = u * u;
    c.f[0] = u * u * u;
    c.f[2] = u;
    ppath = p->pcammove;
    t = Dot(c.q, D_002BDA70[ppath->iCurve]);
    t1 = 1.0f - t;
    pos.q = VecCombo(VU_FLOAT(t1), VU_VECTOR(*(qword *)&ppath->pposeFrom->pos), VU_FLOAT(t), VU_VECTOR(*(qword *)&ppath->pposeTo->pos));
    rot.q = VecCombo(VU_FLOAT(t1), VU_VECTOR(*(qword *)&ppath->pposeFrom->eul), VU_FLOAT(t), VU_VECTOR(*(qword *)&ppath->pposeTo->eul));
    fov = t1 * ppath->pposeFrom->radFov + t * ppath->pposeTo->radFov;
    {
        M3 m3;
        M4 *pm4;
        LoadRotateMatrixEuler(&m3, rot.q);
        pm4 = &m4;
        VECTOR_opAssign(&pm4->q.r[3], pos.q);
        pm4->q.r[0] = m3.r[0];
        pm4->q.r[1] = m3.r[1];
        pm4->q.r[2] = m3.r[2];
        m4.af[2][3] = 0.0f;
        m4.af[1][3] = 0.0f;
        m4.af[0][3] = 0.0f;
        pm4->af[3][3] = 1.0f;
        SetCmMat4(&g_cm, pm4);
    }
    SetCmFov(&g_cm, fov);
}
} // namespace M_UpdateCpcamblend

// CPTURRET ctor (vtable 0x2CD3C0) (inferred)
extern "C" CPTURRET *CPTURRET_ctor(CPTURRET *pcpturret)
{
    CPLCY_ctor(pcpturret);
    SLY2_SET_VPTR(pcpturret, CPTURRET);
    return pcpturret;
}

namespace M_ActivateCpturret {
/* The turret (classes.h TURRET, a PO; fields as turret.cpp's TURRETV): +0x1BC is what TURRETV calls strength. */
struct B : PO { void *target; int aim, follow; float pitch, yaw, vpitch, vyaw; LM yawRange, pitchRange, range; float blend, velocity; float turretFov; };
void ActivateCplcy(void *a, struct B *b) __asm__("ActivateCplcy");
} // namespace M_ActivateCpturret
// CPLCY::Activate; near clip 49; targetFieldOfView = turret +0x1BC. (inferred)
// Also known as: Activate (upstream readable / Sly 1 name; Activate__8CPATTACHP2LOPv).
void CPTURRET::Activate(void *pvHero, void *pv)   /* key function (the namespace's views) */
{
    using namespace M_ActivateCpturret;
    void *a = this;
    struct B *b = (struct B *)pvHero;
    CM *pg;
    M_ActivateCpturret::ActivateCplcy(a, b);
    pg = &g_cm;
    SetCmNearClip(pg, 49.0f);
    pg->targetFieldOfView = b->turretFov;
}

namespace M_UpdateCpturret {
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
union M4 { struct { qword r[4]; } q; M3 m3; float af[4][4]; };
extern char D_002BDAB0[] __asm__("D_002BDAB0"), D_002BDAC0[] __asm__("D_002BDAC0");
struct Y { char pad[0x1BC]; float turretFov; char pad1[0x1D0 - 0x1C0]; LO *plo; int isSmoothing; };

void UpdateCplcy(CPTURRET *p) __asm__("UpdateCplcy");
M3 MulMatrix3Matrix3(M4 *pm, M3 *pm2) __asm__("MulMatrix3Matrix3");
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(M3 *pm, float *pg, qword *pv) __asm__("DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR");
float SmoothValue__FfffP3SMPPf(float gCur, float gTarget, float dt, void *psmp, float *pdgNext) __asm__("SmoothValue__FfffP3SMPPf"); /* SmoothValue */
M3 BuildAxisAngleRotationMatrix(qword v, float g) __asm__("BuildAxisAngleRotationMatrix");
qword *VECTOR_opAssign(qword *p, qword v) __asm__("VECTOR_opAssign");
void SetCmMat4(CM *p, M4 *m) __asm__("SetCmMat4");
void UpdateCpturret(CPTURRET *p) __asm__("UpdateCpturret");

// Return the 3x3 matrix product of a and b.
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
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Length of the xyz part of a as a float.
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
// Return a + b * s.
static inline qword AddScaled(qword a, qword b, VU_FLOAT s)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(b), "j"(s.q));
    return r;
}

// Camera follows turret object's world matrix (plo->vt31 GetWorldMatrix), smoothing rotation/position unless cut; fov from
// turret +0x1BC. (inferred)
// Also known as: Update (upstream readable / Sly 1 name; Update__8CPATTACHP6CPDEFIP5CMJOY).
void UpdateCpturret(CPTURRET *p)
{
    UpdateCplcy(p);
    Y *py = (Y *)p->pvHero;
    LO *plo = py->plo;
    M4 m = *(M4 *)plo->GetWorldMatrix();
    if (g_cm.nCut == 0 && py->isSmoothing != 0) {
        M3 mRel;
        M3 mNew;
        union { qword q; } axis;
        float rad;
        mRel = MulMatrix3Matrix3(&m, (M3 *)&g_cm.mat);
        DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&mRel, &rad, &axis.q);
        if (rad < 0.0001f) {
            mNew = m.m3;
        } else {
            float g = SmoothValue__FfffP3SMPPf(rad, 0.0f, g_clock.deltaTime, D_002BDAB0, 0);
            mNew = Mul(BuildAxisAngleRotationMatrix(axis.q, g), m.m3);
        }
        qword d = VuSub<qword>(g_cm.matq.pos, m.q.r[3]);
        float dist = Length(d);
        qword pos;
        if (dist < 0.0001f)
            pos = m.q.r[3];
        else
            pos = AddScaled(m.q.r[3], d, VU_FLOAT(SmoothValue__FfffP3SMPPf(dist, 0.0f, g_clock.deltaTime, D_002BDAC0, 0) / dist));
        VECTOR_opAssign(&m.q.r[3], pos);
        m.m3 = mNew;
        m.af[3][3] = 1.0f;
        m.af[2][3] = 0.0f;
        m.af[1][3] = 0.0f;
        m.af[0][3] = 0.0f;
    }
    SetCmMat4(&g_cm, &m);
    if (g_cm.targetFieldOfView != g_cm.fieldOfView)
        g_cm.targetFieldOfView = py->turretFov;
    else
        SetCmFov(&g_cm, py->turretFov);
}
} // namespace M_UpdateCpturret

// CPLCY::Activate; +0x14=focus arg, +0x10=pv arg. (inferred)
// Also known as: Activate (upstream readable / Sly 1 name; Activate__6CPPAIRP2LOPv).
void CPTWOSHOT::Activate(void *pvHero, void *pv)   /* key function */
{
    CPTWOSHOT *pcptwoshot = this;
    LO *ploHero = (LO *)pvHero, *ploOwner = (LO *)pv;
    ActivateCplcy(pcptwoshot, ploHero, ploOwner);
    pcptwoshot->ploHero = ploHero;
    pcptwoshot->ploOwner = ploOwner;
}

namespace M_UpdateCptwoshot {
// UpdateCptwoshot (Claude agent, lane D12): matched from the v7 draft. Normalize's sqrt and divide are two asms (sqrt into a
// separate length temp), which lengthens the VU chain so the scheduler orders the block like the target; Cross lists `a`
// twice (allocation-only operand) so the VF numbering matches.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct M3 { qword r[3]; };
union M4 { struct { qword r[4]; } q; M3 m3; float af[4][4]; };
struct CLQ { float g0, g1, g2, pad; };
struct LM { float minValue, maxValue; int pad[2]; };
extern float D_002BDACC __asm__("D_002BDACC");
extern CLQ D_002BDAD0 __asm__("D_002BDAD0"), D_002BDAF0 __asm__("D_002BDAF0"), D_002BDB10 __asm__("D_002BDB10"), D_002BDB30 __asm__("D_002BDB30");
extern LM D_002BDAE0 __asm__("D_002BDAE0"), D_002BDB00 __asm__("D_002BDB00"), D_002BDB20 __asm__("D_002BDB20"), D_002BDB40 __asm__("D_002BDB40");
extern qword g_normalZ __asm__("g_normalZ");

void UpdateCplcy(CPTWOSHOT *p) __asm__("UpdateCplcy");
qword GetLoWorldPos(LO *p) __asm__("GetLoWorldPos");
M3 *LoadLookAtMatrix(M3 *pm, qword v) __asm__("LoadLookAtMatrix");
qword *VECTOR_opAssign(qword *p, qword v) __asm__("VECTOR_opAssign");
void SetCmMat4(CM *p, M4 *m) __asm__("SetCmMat4");
void UpdateCptwoshot(CPTWOSHOT *p) __asm__("UpdateCptwoshot");

// Evaluate the quadratic g0 + g1*g + g2*g^2.
static inline float GFromClq(const CLQ &c, float g) { return c.g0 + g * (c.g1 + g * c.g2); }
// Float carried in a GPR qword (mfc1), or loaded from memory.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); } VU_FLOAT(const float &f, int) { __asm__("lw %0, %1" : "=r"(q) : "m"(f)); } };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
// Return v1*f1 + v2*f2.
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q), "j"(f1.q), "j"(f2.q)); /* extra unused operands: allocation only */
    return r;
}
// Length of the xyz part of a as a float.
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
// Cross product a x b.
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b), "j"(a)); /* extra operand: allocation only */
    return r;
}
// Unit vector along a.
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
// Float broadcast into a VU0 register on construction.
struct VU_FLOATJ { qword q; VU_FLOATJ(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Return a scaled by s.
static inline qword Scale(qword a, VU_FLOATJ s) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(a), "j"(s.q)); return r; }

// Set the main camera's matrix.
static inline void Cm1(M4 *pm) { SetCmMat4(&g_cm, pm); }
// Set the main camera's field of view.
static inline void Cm2(float g) { SetCmFov(&g_cm, g); }
// Frame two objects: focus = weighted mix at z=150, eye offset sideways (cross with up) and up by distance curves,
// look-at matrix, fov from distance curve. (inferred)
// Also known as: Update (upstream readable / Sly 1 name; Update__6CPPAIRP6CPDEFIP5CMJOY).
void UpdateCptwoshot(CPTWOSHOT *p)
{
    U d;
    UpdateCplcy(p);
    d.q = VuSub(GetLoWorldPos(p->ploOwner), GetLoWorldPos(p->ploHero));
    d.f[2] = 0.0f;
    float dist = Length(d.q);
    U focus;
    focus.q = VecCombo(VU_FLOAT(D_002BDACC, 0), VU_VECTOR(GetLoWorldPos(p->ploOwner)), VU_FLOAT(1.0f - D_002BDACC), VU_VECTOR(GetLoWorldPos(p->ploHero)));
    focus.f[2] = 150.0f;
    U pos;
    pos.q = focus.q;
    pos.f[2] += Clamp(GFromClq(D_002BDAD0, dist), D_002BDAE0.minValue, D_002BDAE0.maxValue);
    qword n = Normalize(Cross(d.q, g_normalZ));
    float g = Clamp(GFromClq(D_002BDAF0, dist), D_002BDB00.minValue, D_002BDB00.maxValue);
    U pos2;
    pos2.q = VuAdd(focus.q, Scale(n, VU_FLOATJ(g)));
    pos2.f[2] += Clamp(GFromClq(D_002BDB10, dist), D_002BDB20.minValue, D_002BDB20.maxValue);
    {
        M4 m4;
        M3 m3;
        LoadLookAtMatrix(&m3, VuSub(pos.q, pos2.q));
        M4 *pm4 = &m4;
        VECTOR_opAssign(&pm4->q.r[3], pos2.q);
        pm4->q.r[0] = m3.r[0];
        pm4->q.r[1] = m3.r[1];
        pm4->q.r[2] = m3.r[2];
        m4.af[2][3] = 0.0f;
        m4.af[1][3] = 0.0f;
        m4.af[0][3] = 0.0f;
        pm4->af[3][3] = 1.0f;
        Cm1( pm4);
    }
    Cm2( Clamp(GFromClq(D_002BDB30, dist), D_002BDB40.minValue, D_002BDB40.maxValue));
}
} // namespace M_UpdateCptwoshot

namespace M_RcPtr_Assign {
typedef struct { int pad[3]; void *vptr; } O;   /* a SND (BASIC: AddRef/Release) */
// Reference-counted pointer assignment: add a reference to o, release the old target, store o.
extern "C" { inline void RcPtr_Assign(O **pp, O *o)
{
    if (o) ((BASIC *)o)->AddRef();
    if (*pp) ((BASIC *)*pp)->Release();
    *pp = o;
} }
} // namespace M_RcPtr_Assign

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const VECTOR4 D_002A5000 = {-50.0f, 0.0f, 0.0f, 0.0f};






































int D_002DE254;

// ---- cplcy's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
LM D_002BD950 = {-10.0f, 10.0f};
LM D_002BD958 = {-1.0f, 1.0f};
LM D_002BD960 = {-1.5f, 1.5f};
LM D_002BD968 = {-1.0f, 1.0f};
LM D_002BD970 = {-1.0f, 1.0f};
CLQ D_002BD980 __attribute__((aligned(16))) = {{{2.0f, 0.0f, -1.0f, 0.0f}}};
float D_002BD990 = 0.9f;
float D_002BD994 = 0.8f;
float D_002BD998 = 10.0f;
float D_002BD99C = 10.0f;
SMPA D_002BD9A0 = {1.0f, 0.0f, 0.2f, 0.0f};
LM D_002BD9B0 = {0.0f, 1.3f};
SMPA D_002BD9B8 = {1.0f, 0.4f, 1.5f, 0.0f};
SMPA D_002BD9C8 = {300.0f, 100.0f, 0.5f, 0.0f};
SMPA D_002BD9D8 = {1.0f, 0.5f, 0.5f, 0.0f};
SMPA D_002BD9E8 = {1.0f, 0.4f, 1.5f, 0.0f};
SMPA D_002BD9F8 = {100.0f, 40.0f, 1.5f, 0.0f};
CLQ D_002BDA10 __attribute__((aligned(16))) = {{{0.0f, 1.1f, -0.3f, 0.0f}}};
LM D_002BDA20 = {0.0f, 0.5f};
CLQ D_002BDA30 __attribute__((aligned(16))) = {{{0.0f, 0.5f, -0.1f, 0.0f}}};
LM D_002BDA40 = {0.0f, 0.4f};
LM D_002BDA48 = {-0.01f, 0.01f};
LM D_002BDA50 = {-0.04f, 0.04f};
LM D_002BDA58 = {1.5f, 2.0f};
float D_002BDA60 = 5.0f;
float D_002BDA64 = 1.0f;
VECTOR4 D_002BDA70[4] = {{0.0f, 0.0f, 1.0f, 0.0f}, {-1.0f, 2.0f, 0.0f, 0.0f}, {-1.0f, -1.0f, 1.0f, 0.0f}, {-2.0f, 3.0f, 0.0f, 0.0f}};
SMPA D_002BDAB0 = {5.0f, 0.0f, 0.5f, 0.0f};
SMP D_002BDAC0 = {2000.0f, 0.0f, 1.0f};
float D_002BDACC = 0.45f;
CLQ D_002BDAD0 = {{{200.0f, 0.22f, 0.0f, 0.0f}}};
LM D_002BDAE0 = {225.0f, 650.0f};
CLQ D_002BDAF0 __attribute__((aligned(16))) = {{{600.0f, 0.33f, 0.0f, 0.0f}}};
LM D_002BDB00 = {600.0f, 1500.0f};
CLQ D_002BDB10 __attribute__((aligned(16))) = {{{200.0f, 0.225f, 0.0f, 0.0f}}};
LM D_002BDB20 = {0.0f, 500.0f};
CLQ D_002BDB30 __attribute__((aligned(16))) = {{{0.6f, 0.00045f, 0.0f, 0.0f}}};
LM D_002BDB40 = {0.7f, 1.2f};
