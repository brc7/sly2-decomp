// camera.cpp -- CAMERA (class cid 0x2C), a camera placed in the level by the designers (an XFM with an optional
// look-at target, a focal length and a field of view), and CPCAMERA, the camera policy that puts the camera manager at
// such a camera. A camera pushes the policy when it enters the world hierarchy (or at load, when lo.h's isInWorld bit is set) and revokes
// it when it leaves. Entry points: CAMERAMARKER::Init, PostCameraLoad, Add/RemoveCameraHierarchy, CPCAMERA::Activate,
// DeactivateCpcamera, UpdateCpcamera (VU0 asm). TU 0x12E600..0x12EB48 (13 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_camera
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_camera
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/classes.h"
#include "sly2/vtables.h"

// 0x12E9F0 StaticInit_camera and GlobalConstructors_camera: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef int qwordV __attribute__((mode(TI)));
typedef union { qwordV q; float f[4]; } U;
typedef struct { qwordV r[3]; } M3;
typedef struct { qwordV r[4]; } M4;

/* View of the camera object. */
typedef CAMERAMARKER CAMERA;   /* the camera object (classes.h CAMERAMARKER; was a whole-object view) */
extern U D_002A2DA0, g_normalZ;
extern "C" {
void InitXfm(CAMERA *p);
void PostLoLoad(CAMERA *p);
void AddLoHierarchy(CAMERA *p);
void RemoveLoHierarchy(CAMERA *p);
void SetCmPolicy(CM *a, int b, void *c, int d, void *e);
void RevokeCmPolicy(CM *a, int b, int c, void *d, int e, void *f);
void CPLCY_ctor(CPCAMERA *p);
void ActivateCplcy(CPCAMERA *t, void *b, CAMERA *s);
void DeactivateCplcy(CPCAMERA *t, int b, CAMERA *s);
void SetCmFov__FP2CMf(CM *a, float g);
void CutCm__FP2CM(CM *pcm);
qwordV GetLoWorldPos(CAMERA *pso);
M3 *GetLoWorldMatrix(CAMERA *pso);
void SetCameraLookAt(CM *pcm, qwordV a, qwordV b, float g, float h);
void VECTOR_opAssign(qwordV *p, qwordV v);
void SetCmMat4(CM *pcm, M4 *pm);
float atanf(float g);
}

// Initializes a camera object (class cid 0x2C, vtable slot 7): XFM init, then the two vectors at +0x100/+0x110.
void CAMERAMARKER::Init()
{
    CAMERA *camera = (CAMERA *)this;
    InitXfm(camera);
    camera->axis100 = D_002A2DA0.q;
    camera->axis110 = g_normalZ.q;
}

// Post-load (vtable slot 15): LO post-load, marks the camera as policy-bearing, and if it is already in the world (lo.h isInWorld) pushes
// the camera manager's CPCAMERA policy (g_cm+0x4A8, priority 10) with this camera right away.
extern "C" void PostCameraLoad(CAMERA *camera)
{
    PostLoLoad(camera);
    camera->hasPolicy = 1;
    if ((int)camera->isInWorld & 1)   /* lo.h isInWorld: bit 60 of the doubleword at +0x28 (the original read it so) */
        SetCmPolicy(&g_cm, 10, &g_cm.fixedCameraPolicy, 0, camera);
}

// Added to the world hierarchy (vtable slot 11): pushes the CPCAMERA policy with this camera.
// Also known as: EnableCmObject (upstream readable / Sly 1 name; EnableCmObject__FP2LO).
extern "C" void AddCameraHierarchy(CAMERA *camera)
{
    AddLoHierarchy(camera);
    if (camera->hasPolicy)
        SetCmPolicy(&g_cm, 10, &g_cm.fixedCameraPolicy, 0, camera);
}

// Removed from the world hierarchy (vtable slot 12): revokes the CPCAMERA policy it pushed.
extern "C" void RemoveCameraHierarchy(CAMERA *camera)
{
    RemoveLoHierarchy(camera);
    if (camera->hasPolicy)
        RevokeCmPolicy(&g_cm, 15, 10, &g_cm.fixedCameraPolicy, 0, camera);
}

// Vtable slot 43: returns the address of the focal length (lets the property/animation system drive it) (inferred).
extern "C" float *GetCameraFocalLengthField(CAMERA *camera)
{
    return &camera->focalLength;
}

// Vtable slot 44: one more request to derive the field of view from the focal length (see UpdateCpcamera) (inferred).
extern "C" void IncrementCameraFovRequests(CAMERA *camera)
{
    camera->fovRequests = camera->fovRequests + 1;
}

// Vtable slot 45: drops one such request (inferred).
extern "C" void DecrementCameraFovRequests(CAMERA *camera)
{
    camera->fovRequests = camera->fovRequests - 1;
}

// Constructs the CPLCY-derived camera policy embedded in this class (__5CPLCY 0x14FCA8 then vptr) (inferred)
extern "C" CPCAMERA *CPCAMERA_ctor(CPCAMERA *policy)
{
    CPLCY_ctor(policy);
    SLY2_SET_VPTR(policy, CPCAMERA);   /* the vtable pointer, after CPLCYDATA */
    return policy;
}

// Activates the policy for a camera object: base activation, remembers the camera and applies its field of view if
// it has one.
// Also known as: Activate (upstream readable / Sly 1 name; Activate__8CPLOOKATP2LOPv).
void CPCAMERA::Activate(void *pvHero, void *pv)   /* key function: CPCAMERA's vtable comes out here */
{
    CPCAMERA *policy = this;
    void *hero = pvHero;
    CAMERA *camera = (CAMERA *)pv;
    ActivateCplcy(policy, hero, camera);
    policy->pcamera = (CAMERAMARKER *)camera;
    if (camera->fov != 0.0f)
        SetCmFov__FP2CMf(&g_cm, camera->fov);
}

// Deactivates the policy for a camera object: if it is the active camera, cuts (no blend) when the camera asks for it
// and forgets it; then base deactivation.
// Also known as: Deactivate (upstream readable / Sly 1 name; Deactivate__8CPLOOKATP2LOPv).
extern "C" void DeactivateCpcamera(CPCAMERA *policy, int hero, CAMERA *camera)
{
    if (policy->pcamera == (CAMERAMARKER *)camera) {
        if (camera->cutOnEnd != 0)
            CutCm__FP2CM(&g_cm);
        policy->pcamera = 0;
    }
    DeactivateCplcy(policy, hero, camera);
}

// Copies the three rotation rows of a 3x3 matrix into a 4x4 one.
static __inline__ void LoadM4(M4 *pm, M3 *pmat)
{
    pm->r[0] = pmat->r[0];
    pm->r[1] = pmat->r[1];
    pm->r[2] = pmat->r[2];
}
// (Claude agent) vmulx outputs into temps, then stores g, mT.r[0], mT.r[1] in that order.
// Per-frame update: places the camera at the camera object, looking at its target when it has one, otherwise along
// the object's own axes (the game's camera space swaps them: forward = -x row, up = row 1). When field-of-view
// requests are active, sets the fov from the focal length: 2*atan(18/focalLength), clamped to 0.01..3 rad.
// Also known as: Update (upstream readable / Sly 1 name; Update__8CPLOOKATP6CPDEFIP5CMJOY).
extern "C" void UpdateCpcamera(CPCAMERA *policy)
{
    CAMERA *camera = (CAMERA *)policy->pcamera;
    if (camera->lookAtTarget) {
        qwordV eye = GetLoWorldPos(camera);
        qwordV target = GetLoWorldPos(camera->lookAtTarget);
        SetCameraLookAt(&g_cm, eye, target, 0.0f, 0.0f);
    } else {
        M3 m = *GetLoWorldMatrix(camera);
        M3 mT;
        M4 m4;
        U g;
        float f = -1.0f;
        M4 *pm = &m4;
        qwordV pos;
        {
        register qwordV gq __asm__("vf2");
        qwordV t1, t0;
        gq = VuFromFloat<qwordV>(f);
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(t1) : "j"(m.r[0]), "j"(gq));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(t0) : "j"(m.r[2]), "j"(gq));
        g.q = gq;
        mT.r[0] = t0;
        mT.r[1] = t1;
        }
        mT.r[2] = m.r[1];
        pos = GetLoWorldPos(camera);
        VECTOR_opAssign(&m4.r[3], pos);
        LoadM4(pm, &mT);
        ((int *)&m4.r[2])[3] = 0;
        ((int *)&m4.r[1])[3] = 0;
        ((int *)&m4.r[0])[3] = 0;
        ((float *)&pm->r[3])[3] = 1.0f;
        SetCmMat4(&g_cm, pm);
    }
    if (camera->fovRequests > 0) {
        float g = atanf(17.995901107788086f / camera->focalLength);
        g = Clamp(g + g, 0.01f, 3.0f);
        SetCmFov__FP2CMf(&g_cm, g);
    }
}
