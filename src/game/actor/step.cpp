// step.cpp -- STEP (a PO that walks on legs: the guards and the player characters derive from it).
// Role: the walking body's physics on top of SO/PO: a "phys" collision part that the body part smoothly follows,
// steering toward a target heading, a cached floor contact (base) found by a ground probe, contact adjustments
// (friction and closing speed on the floor), terrain fitting with a two-triangle footprint, the behaviour stack
// (BHVSTK) accessors and transform matching. Entry points: STEP::Init, LoadStepFromBrx, PostStepLoad, UpdateStep
// (slot 16), PresetStepAccel (slot 70), the contact slots 54-60, SetStepTargetHeading / FaceStepToward,
// PushStepBehavior / FindStepBehavior.
// TU 0x203948..0x205E70 (45 functions, address order); the linkonce setter 0x259D18 is an inline function in
// sly2/step.h.
//
// Types: step.h STEP/BHVSTK, po.h PO/POCTL, so.h SO/XF; the 18 m2c bodies are written on them. The other functions
// keep the per-function views they were matched with, each in a namespace named after it (as vec.cpp and mark.cpp
// do).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_step
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_step
#include "sly2/basic.h"
#include "sly2/so.h"
#include "sly2/act.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/slotheap.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"
#include "sly2/sndmgr.h"
#include "sly2/sw.h"
#include "sly2/xp.h"

// The real classes behind the per-function views below, for their virtual calls.
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }
static inline ::STEP *AsStep(void *pstep) { return (::STEP *)pstep; }

// 0x205D18 StaticInit_step and GlobalConstructors_step: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* A contact's side-1 object (engine/physics/mark.cpp's XP: axps[1].pso at +0x48). */
#define PSO_XP(pxp) (*(SO **)((char *)(pxp) + 0x48))

extern "C" {
extern qword D_002A2DD0;    /* zero vector */
void LoadPoFromBrx(PO *ppo, CBinaryInputStream *pbis);
void ClonePo(PO *ppo, PO *ppoBase);
void CloneBhvstk(BHVSTK *pbhvstk, STEP *pstep);
void RenderSoSelf(SO *pso, void *pcm, void *pro);
qword ConvertPosBetweenLos(SO *psoFrom, SO *psoTo, qword pos);
int EnsureSoPhysRegionRecord(STEP *pstep);   /* its int result decides the scheduling around the call (the v0 rule) */
SOPART *FindSoPart(SO *pso, OID oid);
void SetStepPhys(STEP *pstep, SOPART *ppart, int n);
void TranslateSoToPos(SO *pso, qword pos);
void RotateSoToMat(SO *pso, MATRIX3 *pmat);
void SetStepTargetHeading(STEP *pstep, float rad);
float GetStepHeading(STEP *pstep);
float atan2f(float y, float x);
void OnSoContactFreed(SO *pso, SO *psoOther);
BHV *PushBhvstk(BHVSTK *pbhvstk);
BHV *FindBhvInBhvstk(BHVSTK *pbhvstk, CID cid);
}

/* STEP +0x1A4: how the body turns (inferred from UpdateStep / PresetStepAccel): toward the target heading with the
   spin smoothing, by a fixed angular velocity (+0x1E0), or not at all. */
enum STEERING { STEERING_NONE = -1, STEERING_HEADING = 0, STEERING_ANGULAR_VELOCITY = 1 };
/* Ground-probe flags (slot 85, GetStepGroundProbeFlags; read by AddStepCustomXps). */
enum
{
    PROBE_ENABLED = 1,          /* cast the floor ray at all */
    PROBE_FROM_TOP = 2,         /* cast to the top helper point instead of the base point */
    PROBE_FLAG4 = 4,            /* the floor contact takes part in the contact solver (solve.cpp tests it) */
    PROBE_KEEP_NORMAL = 8,      /* always use the hit normal (no snapping to straight up) */
    PROBE_DEFAULT = PROBE_ENABLED | PROBE_FROM_TOP | PROBE_FLAG4
};

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_InitStep {
void InitPo(PO *ppo) __asm__("InitPo");
void ScaleStepSpinSpeed(STEP *pstep, float r) __asm__("ScaleStepSpinSpeed");
void InitBhvstk(BHVSTK *pbhvstk, STEP *pstep) __asm__("InitBhvstk");
} // namespace M_InitStep
// Slot 7 (Init): InitPo, then walking defaults: gravity ratio 1, spin ratio 1 (ScaleStepSpinSpeed), target heading
// 0, base radius 30, step-down height -50, and the behaviour stack.
// Also known as: InitStep (upstream readable / Sly 1 name; InitStep__FP4STEP).
void STEP::Init()
{
    using namespace M_InitStep;
    InitPo(this);
    oidPhys = -1;
    gravityRatio = 1.0f;
    unknown200 = 1.0f;
    ScaleStepSpinSpeed(this, 1.0f);
    SetStepTargetHeading(this, 0.0f);
    baseRadius = 30.0f;
    stepDownHeight = -50.0f;
    InitBhvstk(&behaviors, this);
}

// Slot 14 (LoadFromBrx): LoadPoFromBrx, caches the base position in parent space (+0x240) and ensures the physics
// region record. Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadPoFromBrx (upstream readable / Sly 1 name; LoadPoFromBrx__FP2POP18CBinaryInputStream).
extern "C" void LoadStepFromBrx(STEP *step, CBinaryInputStream *stream)
{
    LoadPoFromBrx(step, stream);
    step->pivotOffset = ConvertPosBetweenLos(step->parent, step, D_002A2DD0);
    EnsureSoPhysRegionRecord(step);
}

// Slot 41 (Clone): ClonePo, then points the behaviour stack's owner at the clone.
extern "C" void CloneStep(STEP *step, STEP *pstepBase)
{
    ClonePo(step, pstepBase);
    CloneBhvstk(&step->behaviors, step);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PostStepLoad {
void PostSoLoad(SO *pso) __asm__("PostSoLoad");
void *NewSampledShape(int a, int b) __asm__("NewSampledShape");
void SetXfRole(SO *pso, XF *pxf, int f) __asm__("SetXfRole");
void BuildStepPhysPart(STEP *pstep, SOPART *ppart) __asm__("BuildStepPhysPart");
void HideSoPart(SO *pso, SOPART *ppart) __asm__("HideSoPart");
void PostBhvstkLoad(BHVSTK *pbhvstk) __asm__("PostBhvstkLoad");
// Slot 15 (PostLoad): PostSoLoad, allocates the step's sampled shape, gives its two child transforms role 1 and
// builds the collision ("phys") part from the mesh part named by oidPhys (BuildStepPhysPart).
// Also known as: PostStepLoad (upstream readable / Sly 1 name; PostStepLoad__FP4STEP).
extern "C" void PostStepLoad(STEP *step)
{
    PostSoLoad(step);
    step->unknown294 = NewSampledShape(0x2C9, 0x10);
    SetXfRole(step, step->positionXf, 1);
    SetXfRole(step, step->rotationXf, 1);
    if (step->oidPhys != -1) {
        step->ppartPhysMesh = FindSoPart(step, (OID)step->oidPhys);
        if (step->ppartPhysMesh) {
            BuildStepPhysPart(step, step->ppartPhysMesh);
            HideSoPart(step, step->ppartPhysMesh);
        }
    }
    PostBhvstkLoad(&step->behaviors);
}
} // namespace M_PostStepLoad

// Slot 19 (RenderSelf): just RenderSoSelf.
// Also known as: RenderStepSelf (upstream readable / Sly 1 name; RenderStepSelf__FP4STEPP2CMP2RO).
extern "C" void RenderStepSelf(STEP *step, void *pcm, void *pro)
{
    RenderSoSelf(step, pcm, pro);
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A85F0 __asm__("D_002A85F0") = 3.40282347e+38f; }
// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00203AF0 {
/* (Claude agent) matched: mesh->meshLocal.positions loaded into a $2 pointer after qmtc2, vmadd fed the VF3 scalar directly, slot = firstHelper*16 + (int)dst computed before the u.q store. */
typedef union { qword q; float f; } U;
struct E { float x; float y; float z; float w; };
extern const float D_002A85F0 __asm__("D_002A85F0");   /* defined below where the ELF has it (.rodata order) */
extern qword g_normalZ __asm__("g_normalZ");
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void RemoveSoParts(STEP *p) __asm__("RemoveSoParts");
void AddSoParts(STEP *p) __asm__("AddSoParts");
int IsBasicDerivedFrom__FP5BASIC3CID(STEP *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void CloneSoPartGeomInserting(SOPART *m, void *p, int k, SOPART *q) __asm__("CloneSoPartGeomInserting");
void EnsureSoPartMesh(SOPART *q) __asm__("EnsureSoPartMesh");
void UpdateGeomWorld(void *a, void *b, void *firstHelper) __asm__("UpdateGeomWorld");

// Builds the step's collision part from a mesh part: appends a new SOPART, copies the geometry, records the lowest
// point as the base height (+0x210) and adds two helper points. (inferred)
// Also known as: MakeStepCustomEntry (upstream readable / Sly 1 name; MakeStepCustomEntry__FP3ALOP3SOE).
extern "C" void BuildStepPhysPart(STEP *step, SOPART *m)
{
    int n, allPinned, i, firstHelper;
    float lowestZ;
    U u;
    qword r;
    {
        bool fInWorld = step->isInWorld;   /* through a bool: g++ shifts the bit out (dsrl32; andi 1) */
        if (fInWorld)
            RemoveSoParts(step);
    }
    n = step->numParts;
    step->numParts = n + 1;
    allPinned = 0;
    step->psopartBody = &step->parts[n];
    if (!IsBasicDerivedFrom__FP5BASIC3CID(step, 0x34))
        allPinned = m->unknown1C == m->meshLocal.numPositions;
    CloneSoPartGeomInserting(m, step->rootTransform, !allPinned ? 2 : 3, step->psopartBody);
    EnsureSoPartMesh(step->psopartBody);
    lowestZ = D_002A85F0;
    for (i = 0; i < step->psopartBody->unknown1C; i++) {
        float e = ((struct E *)step->psopartBody->meshLocal.positions)[i].z;
        lowestZ = (e > lowestZ) ? lowestZ : e;
    }
    step->unknown210 = lowestZ;
    firstHelper = step->psopartBody->unknown1C;
    step->psopartBody->unknown1C = firstHelper + 2;
    {register SOPART *mesh __asm__("$4")=step->psopartBody;
     register qword point __asm__("$3")=step->pivotOffset;
     ((qword *)mesh->meshLocal.positions)[firstHelper]=point;}
    {register int word __asm__("$2");
     register SOPART *mesh __asm__("$3");
     register qword *dst __asm__("$2");
     __asm__("lw %0,%2\n\tlw %1,%3":"=&r"(word),"=r"(mesh):"m"(step->stepDownHeight),"m"(step->psopartBody));
     {register qword value __asm__("vf3");__asm__("qmtc2.ni %1,%0":"=j"(value):"r"(word));
     dst=(qword *)mesh->meshLocal.positions;
     __asm__ __volatile__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(step->pivotOffset),"j"(g_normalZ),"j"(value));
     {qword *slot=(qword *)(firstHelper*16+(sptr)dst);
     u.q=value;
     slot[1]=r;}}}
    if (allPinned)
        ((qword *)step->psopartBody->meshLocal.positions)[firstHelper + 2] = D_002A2DD0;
    UpdateGeomWorld(&step->psopartBody->transform->worldMatrix, &step->psopartBody->meshLocal, &step->psopartBody->meshWorld);
    step->unknown218 = (::qword *)((char *)step->psopartBody->meshWorld.positions + firstHelper * 16);
    step->unknown21C = (::qword *)((char *)step->psopartBody->meshWorld.positions + (firstHelper * 16 + 0x10));
    {
        bool fInWorld = step->isInWorld;
        if (fInWorld)
            AddSoParts(step);
    }
}
} // namespace M_func_00203AF0

// Looks up a part by oid and makes it the current phys shape (SetStepPhys).
extern "C" void SetStepPhysByOid(STEP *step, OID oid, int n)
{
    SOPART *ppart;
    ppart = FindSoPart(step, oid);
    if (ppart != 0) {
        SetStepPhys(step, ppart, n);
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SetStepPhys {
typedef struct { qword a[4]; } MATRIX4;   /* qword rows (TU-private; XF's matrices are converted at the call) */
/* The C file assigned the struct result (hidden pointer = mi); as C++ that adds a temporary, so the result is
   passed by hand. This form exists to reproduce the original codegen. */
void InvertMatrix4RigidP(MATRIX4 *pd, MATRIX4 *ps) __asm__("InvertMatrix4Rigid");
void __ml__FRC7MATRIX4T0(MATRIX4 *pd, MATRIX4 *pa, MATRIX4 *pb) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
void *func_002333B4(void *p, int c, int n) __asm__("memset");
// Sets the current phys part (psopartPhys). With fForceSnap the body part (psopartBody) takes its points at once (in the body
// part's frame) and the per-point velocities are cleared.
// Also known as: SetStepMesh (upstream readable / Sly 1 name; SetStepMesh__FP3ALOPvi).
extern "C" void SetStepPhys(STEP *step, SOPART *pm, int f)
{
    step->psopartPhys = pm;
    if (f) {
        MATRIX4 m, mm, mi;
        MATRIX4 *pmm = &mm;
        MATRIX4 *pmi = &mi;
        int i;
        qword *ps, *pd;
        InvertMatrix4RigidP(&mi, (MATRIX4 *)&step->psopartBody->transform->worldMatrix);
        __ml__FRC7MATRIX4T0(pmm, pmi, (MATRIX4 *)&step->psopartPhys->transform->worldMatrix);
        m = mm;
        ps = (qword *)step->psopartPhys->meshLocal.positions;
        pd = (qword *)step->psopartBody->meshLocal.positions;
        i = step->psopartPhys->meshLocal.numPositions;
        while (--i >= 0) {
            qword r;
            __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
                    : "=j"(r) : "0"(m.a[0]), "j"(m.a[1]), "j"(m.a[2]), "j"(m.a[3]), "j"(*ps));
            *pd = r;
            ps++;
            pd++;
        }
        func_002333B4(step->psopartBody->plvo->mpiposvLocal, 0, step->psopartBody->meshLocal.numPositions * 16);
        InvalidateSoAaox(step);
    }
}
} // namespace M_SetStepPhys

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateStepPhys {
typedef struct {qword a[4];} M4;
union V {qword q;float f[4];};
struct X {char pad40[0x40];M4 m;};
struct B {union V *v;};
struct M {struct X *px;char pad4[0xC];struct B *pb;char pad14[8];int count;char pad20[0x5C];union V *points;};
struct S : PO { char pad258[0x258 - 0x180]; struct M *pm0,*pm1;};
/* step's .data starts here: the contact side signs (medium: gadget and murray read them too) and the point
 * smoothing SMP; the rest is defined in the per-function namespaces below, in address order. */
extern float D_002C3C60[2] __asm__("D_002C3C60");
extern float D_002C3C68[3] __asm__("D_002C3C68");
M4 *InvertMatrix4Rigid(M4 *,const M4 *) __asm__("InvertMatrix4Rigid");
M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
float SmoothValue__FfffP3SMPPf(float,float,float,float *,float *) __asm__("SmoothValue__FfffP3SMPPf");
// Copy a 4x4 matrix through four GPR quadword loads/stores.
extern "C" { static __inline__ void CopyMatrix(M4 *destination,const M4 *source) {
    register qword row0 __asm__("$2");
    register qword row1 __asm__("$3");
    register qword row2 __asm__("$4");
    register qword row3 __asm__("$5");
    __asm__("lq %2,%6\n\tlq %1,%5\n\tlq %3,%7\n\tlq %0,%4"
      :"=r"(row0),"=r"(row1),"=r"(row2),"=r"(row3)
      :"m"(source->a[0]),"m"(source->a[1]),"m"(source->a[2]),"m"(source->a[3]));
    destination->a[1]=row1;destination->a[2]=row2;
    destination->a[3]=row3;destination->a[0]=row0;
} }
// Per frame: moves each body-part point toward the phys part's point with SmoothValue and stores the point's velocity
// ((new - old) / dt), so shape changes (crouch, stand) blend instead of popping.
// Also known as: UpdateStepMesh (upstream readable / Sly 1 name; UpdateStepMesh__FP3ALO).
extern "C" void UpdateStepPhys(struct S *input)
{
    void *context=input;
    M4 m;
    union V v;
    M4 mm,mi;
    register M4 *pmi __asm__("$16");
    register M4 *pmm __asm__("$17");
    register struct B *velocityContainer __asm__("$2");
    register struct X *firstBasis __asm__("$5");
    union V *old;
    register union V *velocity __asm__("$20");
    int i,j;
    register struct M *firstSource __asm__("$3");
    register struct M *secondSource __asm__("$4");
    CLOCK *clock;
    float *pv;
    float *pt,*ps,*pd;
    __asm__("lw %0,%1":"=r"(firstSource):"m"(((struct S*)context)->pm0));
    if(!firstSource)return;
    __asm__("lw %0,%1":"=r"(secondSource):"m"(((struct S*)context)->pm1));
    pmi=&mi;
    if(!secondSource)return;
    __asm__("lw %0,%2\n\tlw %1,%3":"=r"(velocityContainer),"=r"(firstBasis):"m"(firstSource->pb),"m"(firstSource->px));
    pmm=&mm;
    {register float *nextPoints __asm__("$18");
     register union V *oldPoints __asm__("$19");
     __asm__("lw %0,%4\n\tlw %1,%5\n\taddiu %2,%2,64\n\tlw %3,%6"
       :"=r"(nextPoints),"=r"(oldPoints),"+r"(firstBasis),"=r"(velocity)
       :"m"(secondSource->points),"m"(firstSource->points),"m"(velocityContainer->v));
     pd=nextPoints;old=oldPoints;}
    InvertMatrix4Rigid(pmi,(M4 *)firstBasis);
    {register struct M *currentSource __asm__("$2");
     register M4 *destination __asm__("$4");
     register M4 *inverse __asm__("$5");
     register struct X *basis __asm__("$6");
     __asm__("lw %0,%1":"=r"(currentSource):"m"(((struct S*)context)->pm1));
     __asm__("daddu %0,%3,$zero\n\tdaddu %1,%4,$zero\n\tlw %2,%5"
       :"=&r"(destination),"=&r"(inverse),"=r"(basis)
       :"r"(pmm),"r"(pmi),"m"(currentSource->px));
     __ml__FRC7MATRIX4T0(destination,inverse,&basis->m);}

    CopyMatrix(&m,&mm);
    i=((struct S*)context)->pm1->count;
    if(--i>=0){
    pv=v.f;
    clock=&g_clock;
    do{
        register float *nextGoal __asm__("$22");
        int nextIndex=i-1;
        context=old+1;
        __asm__("addiu %0,%1,16":"=r"(nextGoal):"r"(pd));
        qword vq;
        __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(vq) : "0"(m.a[0]),"j"(m.a[1]),"j"(m.a[2]),"j"(m.a[3]),"j"(*(qword*)pd));
        v.q = vq;
        pd=velocity->f;
        __asm__("addiu %0,%0,16":"+r"(velocity));
        pt=pv;ps=old->f;
        j=3;
        while(--j>=0){
            float next=SmoothValue__FfffP3SMPPf(*ps,*pt,clock->deltaTime,D_002C3C68,0);
            *pd=(next-*ps)/clock->deltaTime;
            ++pt;++ps;++pd;
        }
        pd=nextGoal;old=(__typeof__(old))context;
        i=nextIndex;
    }while(i>=0);
    }
}
} // namespace M_UpdateStepPhys

// Sets the "no base" option (+0x204); turning it on drops the cached floor contact (+0x208).
extern "C" void SetStepNoBase(STEP *step, int n)
{
    if (step->unknown204 != n) {
        step->unknown204 = n;
        if (n != 0) {
            step->floorContact = 0;
        }
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateStep {
struct S {
    char pad0[0xC]; void *vptr;
    char pad1[0x180 - 0x10]; float turnSpeedFactor;
    char pad2[0x1A4 - 0x184]; int steeringMode; float targetHeading;
    char pad3[0x208 - 0x1AC]; int floorContact; float lastFloorContactTime;
};
extern float D_002C3C80[3] __asm__("D_002C3C80");   /* turn-rate speed factor polynomial */
extern float g_lmZeroOne[2] __asm__("g_lmZeroOne");
void UpdateSo(struct S *p) __asm__("UpdateSo");
void UpdateStepPhys(struct S *p) __asm__("UpdateStepPhys");
float atan2f(float y, float x) __asm__("atan2f");
float NormalizeAngle__Ff(float a) __asm__("NormalizeAngle__Ff");

// Slot 16 (Update): UpdateSo, UpdateStepPhys, then the turn-rate speed factor (+0x180) from how far the heading is
// from the target heading.
// Also known as: UpdateStep (upstream readable / Sly 1 name; UpdateStep__FP4STEPf).
extern "C" void UpdateStep(struct S *step)
{
    UpdateSo(step);
    UpdateStepPhys(step);
    switch (step->steeringMode) {
    case STEERING_HEADING: {
        float *a = (float *)AsStep(step)->GetWorldMatrix();
        float *b = (float *)AsStep(step)->GetWorldMatrix();
        float d = NormalizeAngle__Ff(step->targetHeading - atan2f(a[1], b[0]));
        float f;
        d = __builtin_fabsf(d);
        f = D_002C3C80[0] + d * (D_002C3C80[1] + d * D_002C3C80[2]);
        {
            float lo = g_lmZeroOne[0], hi = g_lmZeroOne[1];
            if (f < lo)
                f = lo;
            else if (hi < f)
                f = hi;
        }
        step->turnSpeedFactor = f;
        break;
    }
    case STEERING_NONE:
    case STEERING_ANGULAR_VELOCITY:
        step->turnSpeedFactor = 1.0f;
        break;
    }
    if (step->floorContact)
        step->lastFloorContactTime = g_clock.time;
}
} // namespace M_UpdateStep

// Slot 85: ground-probe flags for AddStepCustomXps: 0 with the no-base option, else 7 (probe, use top point, ...).
extern "C" int GetStepGroundProbeFlags(STEP *step)
{
    return (step->unknown204 != 0) ? 0 : PROBE_DEFAULT;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PresetStepAccel {
/* (Claude agent) clock dt word loaded via ClockPage/ClockWord asm (same form as src/game/engine/fx/rip/ProjectRipTransform.cpp, lw into the lui register); volatile on the lw keeps it after the targetAngularVelocity lqc2. */
typedef union { qword q; float f; } U;
typedef struct { float x,y,z; } VECTOR3;

void PresetSoAccel(struct STEP *p) __asm__("PresetSoAccel");
// Read the frame clock's delta-time word (g_clock) given its high address page.
extern "C" { static inline int ClockWord(char*page){int word;__asm__ volatile("lw %0,%%lo(%2)(%1)":"=r"(word):"r"(page),"i"(&g_clock.deltaTimeBits),"m"(*(const int*)(page-0x212C)));return word;} }
// High address page (lui) of the frame clock g_clock.
extern "C" { static inline char*ClockPage(){register char*page __asm__("$2");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&g_clock.deltaTimeBits));return page;} }
void AccelSoTowardMatBounded(struct STEP *,MATRIX3Q *,VECTOR3 *) __asm__("AccelSoTowardMatBounded");
// Broadcast int bits into vf4 and store them to *value.
extern "C" { static inline void InitScalar(U *value,int bits) {
    register qword scalar __asm__("vf4");
    __asm__("qmtc2.ni %1,%0" : "=j"(scalar) : "r"(bits));
    value->q=scalar;
} }
// Slot 70: PresetSoAccel, then steering: mode 0 accelerates the rotation toward the target heading matrix with the
// spin smoothing (AccelSoTowardMatBounded); mode 1 fixes the angular velocity directly (FixStepAngularVelocity).
// Also known as: UpdateStepPhysMotion (upstream readable / Sly 1 name; UpdateStepPhysMotion__FP3ALO).
extern "C" void PresetStepAccel(struct STEP *step)
{
    PresetSoAccel(step);
    switch (step->unknown1A4) {
    case STEERING_HEADING: {
        VECTOR3 v;
        float s = step->unknown200;
        v.z = step->vSpin.z;
        v.x = step->vSpin.x * s;
        v.y = step->vSpin.y * s;
        AccelSoTowardMatBounded(step, &step->matqTarget, &v);
        break;
    }
    case STEERING_ANGULAR_VELOCITY: {
        register struct XF *po __asm__("$3") = step->rotationXf;
        U u;
        register qword d __asm__("vf3");         register qword vel __asm__("vf2");
        char*page=ClockPage();
        int word;
        d=step->unknown1E0;
        word=ClockWord(page);
        vel = po->acceleration;
        __asm__("vsub.xyzw %0,%0,%1" : "+j"(d) : "j"(po->velq.q));
        InitScalar(&u,word);
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %0, $Q" : "+j"(d) : "j"(u.q));
        __asm__("vadd.xyzw %0,%0,%1" : "+j"(vel) : "j"(d));
        po->acceleration=vel;
        break;
    }
    case STEERING_NONE:
        break;
    }
}
} // namespace M_PresetStepAccel

// Slot 29: TranslateSoToPos, then refreshes the world base position (+0x230) from the local one (+0x240).
// Also known as: SetStepPos (upstream readable / Sly 1 name; SetStepPos__FP4STEPG9VU_VECTOR).
extern "C" void TranslateStepToPos(STEP *step, qword pos)
{
    TranslateSoToPos(step, pos);
    step->unknown230 = ConvertPosBetweenLos(step, 0, step->pivotOffset);
}

// Slot 30: RotateSoToMat, then takes the new heading as target heading and stores it in the controller.
// Also known as: SetStepRotation (upstream readable / Sly 1 name; SetStepRotation__FP4STEPP7MATRIX3).
extern "C" void RotateStepToMat(STEP *step, MATRIX3 *matrix)
{
    POCTL *pctl;
    RotateSoToMat(step, matrix);
    SetStepTargetHeading(step, GetStepHeading(step));
    pctl = step->controller;
    if (pctl != 0) {
        pctl->leftStickAngle = step->targetHeading;
    }
}

// Slot 63 (so.h GetExtraPartCount): a STEP has one extra part, the phys part built by BuildStepPhysPart; the SO
// default (GetSoExtraPartCount) answers 0 (inferred from the slot name).
extern "C" int GetStepExtraPartCount()
{
    return 1;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AdjustStepNewXp {
/* Prepare a new contact: reset its friction/response and set the limits for kind 14; for the
   side-facing kinds add a horizontal response contact (kind 10) through AddXp. */
struct ENDPOINT { void *shape, *owner, *object, *lo; qword pos; qword local; int pad[4]; };
struct XP { struct ENDPOINT ae[2]; QWROW normal; float distance, friction, response; int kind; float lower, upper; int tag; struct XP *next; };
struct STATE { char pad[0xD0]; float mass; };
struct P : LO { char padE8[0xE8 - 0x50]; struct STATE *state; char padEC[0x258 - 0xEC]; void *mesh; };
extern qword D_002A2DA0 __asm__("D_002A2DA0");
extern float D_002C3C60[] __asm__("D_002C3C60");
extern float D_002C3C90 __asm__("D_002C3C90");   /* base contact friction */
void AdjustSoNewXp(void) __asm__("AdjustSoNewXp");
void AddXp(void *pw, void *a, void *b, qword pos, qword dir, int k, int x, int y, struct XP **out, float g) __asm__("AddXp");

/* The endpoint positions are passed by address (like C++ const VECTOR & operands): the address
   then comes out as offset + x, and the second read of the other position is a reload. */
extern "C" { static inline qword Sub(const qword *pa, const qword *pb)
{
    qword a = *pa, b = *pb, r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
} }
// Return a scaled by g.
extern "C" { static inline qword Scale(qword a, float g)
{
    qword k;
    QWROW u;
    __asm__("qmtc2.ni %1,%0" : "=j"(k) : "r"(g));
    u.q = k;
    __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(a) : "j"(a), "j"(k));
    return a;
} }
// Dot product of the xyz parts of a and b as a float.
extern "C" { static inline float Dot(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0,%1" : "=r"(g) : "j"(d));
    return g;
} }
// Normalize *v in place, or set it to fallback when its length is below 1e-4 (length pinned to vf1).
extern "C" { static inline void Normal(QWROW *v, qword fallback)
{
    qword a = v->q;
    register qword d __asm__("vf1"); /* the one pin: the length/result lives in vf1 (as in TakeHackDamage) */
    qword one;
    float len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(d), "=&j"(one) : "j"(a));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(len) : "j"(d));
    if (len < 0.0001f)
        d = fallback;
    else
        __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(d) : "j"(d), "j"(a));
    v->q = d;
} }
// Return *pa + b * g.
extern "C" { static inline qword AddScaled(const qword *pa, qword b, float g)
{
    qword a = *pa, k;
    QWROW u;
    __asm__("qmtc2.ni %1,%0" : "=j"(k) : "r"(g));
    u.q = k;
    __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x" : "=j"(a) : "j"(b), "j"(k), "0"(a));
    return a;
} }

// Slot 55: sets up a new contact: resets friction and response; a floor contact (kind 14) with an upward normal
// (z > 0.7) gets the step's floor friction and a force limit from its weight.
// Also known as: OnStepXpNew (upstream readable / Sly 1 name; OnStepXpNew__FP3ALOP2XPi).
extern "C" void AdjustStepNewXp(struct P *step, struct XP *x, int side)
{
    QWROW dir, diff, point;
    float d;
    float sign;
    int kind;
    AdjustSoNewXp();
    kind = x->kind;
    x->friction = 0.0f;
    x->response = 0.0f;
    if (kind == 14) {
        if (x->normal.f[2] > 0.7f) {
            x->friction = D_002C3C90;
            x->lower = step->state->mass * -4500.0f;
            x->upper = step->state->mass * 9000.0f;
        } else
            x->upper = step->state->mass * 9000.0f;
        return;
    }
    if (kind != 3) {
        if (side != 1)
            return;
        if (kind != side && kind != 4 && kind != 5 && kind != 6)
            return;
    }
    sign = D_002C3C60[side];
    if (sign * x->normal.f[2] > 0.0f) {
        /* horizontal direction of the (side-signed) normal */
        dir.q = Scale(x->normal.q, sign);
        dir.f[2] = 0.0f;
        Normal(&dir, D_002A2DA0);
        /* project this endpoint onto the other along that direction */
        diff.q = Sub(&x->ae[side].pos, &x->ae[1 - side].pos);
        d = Dot(diff.q, dir.q);
        point.q = AddScaled(&x->ae[1 - side].pos, dir.q, d);
        AddXp(&g_sw, step->mesh, x->ae[1 - side].shape, point.q, dir.q, 10, 0, 0, &x->next, d);
    }
}
} // namespace M_AdjustStepNewXp

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AdjustStepXpVelocity {
// AdjustStepXpVelocity: a STEP's contact-velocity adjustment (vtable slot 56; Sly 1 has AdjustStepXpVelocity(STEP *, XP *, int)).
// After the base version (AdjustSoXpVelocity), for a kind-0xE contact: the closing speed along the contact normal is smoothed
// with SmoothValue (SmoothValue) toward two limits (SMPs D_002C3C98 / D_002C3CA8), the change is limited to dt * 9000 up and
// dt * -4500 down, and this side's velocity gets normal * -speed plus (matTarget * unknown190 without its part along the
// normal) * unknown180. Finally the contact distance moves by unknown210 - gUnk248.
// Matched with one asm statement per VU0 operation, no register pins: the local pmat gives the target's pointer/row registers,
// the two smoothing outputs are separate address-taken floats (frame slots after the VU_FLOAT home at 0($sp)).

struct VU_FLOAT
{
    qword qw;
};

struct M3
{
    qword mat[3];
};

/* one side of a contact: 0x40 bytes, its velocity at +0x30 */


struct STEP : PO
{
    float unknown180;
    char pad184[0xC];
    qword unknown190;
    char pad1a0[0x10];
    struct M3 matTarget;
    char pad1e0[0x30];
    float unknown210;
    char pad214[0x34];
    float gUnk248;
};


extern float D_002C3C98[3] __asm__("D_002C3C98");   /* the two closing-speed SMPs */
extern float D_002C3CA8[3] __asm__("D_002C3CA8");

void AdjustSoXpVelocity(struct STEP *pstep, struct XP *pxp, int ixpd) __asm__("AdjustSoXpVelocity");
float SmoothValue__FfffP3SMPPf(float gCur, float gTarget, float dt, float *psmp, float *pdgNext) __asm__("SmoothValue__FfffP3SMPPf");




// Rotate v by the 3x3 matrix pmat.
static inline qword QwRotate(struct M3 *pmat, qword v)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qw) : "j"(v), "j"(pmat->mat[0]), "j"(pmat->mat[1]), "j"(pmat->mat[2]));
    return qw;
}


// Broadcast a float into a VU0 register.
static inline struct VU_FLOAT VuFloat(float g)
{
    struct VU_FLOAT vu;
    __asm__("qmtc2.ni %1, %0" : "=j"(vu.qw) : "r"(g));
    return vu;
}

// Broadcast the float word n into a VU0 register.
static inline struct VU_FLOAT VuFloatFromMemWord(int n)
{
    struct VU_FLOAT vu;
    __asm__("qmtc2.ni %1, %0" : "=j"(vu.qw) : "rV"(n));
    return vu;
}

// Return v + a*sa + b*sb.
static inline qword QwAddScaled2(qword v, qword a, qword sa, qword b, qword sb)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %3x\n\tvmaddx.xyzw %0, %4, %5x"
            : "=j"(qw) : "j"(v), "j"(a), "j"(sa), "j"(b), "j"(sb));
    return qw;
}

// Slot 57: for floor contacts (kind 14) smooths the closing speed with SmoothValue, limited to dt * 9000, so the step
// settles onto the floor without bouncing.
// Also known as: OnStepXpUpdate (upstream readable / Sly 1 name; OnStepXpUpdate__FP3ALOP2XPi).
extern "C" void AdjustStepXpVelocity(struct STEP *step, struct XP *pxp, int ixpd)
{
    AdjustSoXpVelocity(step, pxp, ixpd);
    if (pxp->kind != 0xE)
        return;

    float gVn = QwordX(VuDot(VuSub(pxp->axps[0].v, pxp->axps[1].v), pxp->normal));
    float gVnMin;
    float gVnTarget;
    SmoothValue__FfffP3SMPPf(pxp->g, step->unknown210, g_clock.deltaTime, D_002C3C98, &gVnMin);
    SmoothValue__FfffP3SMPPf(pxp->g, 0.0f, g_clock.deltaTime, D_002C3CA8, &gVnTarget);
    gVn = gVn > gVnMin ? gVn : gVnMin;
    if (gVn < gVnTarget)
    {
        float dg = gVnTarget - gVn;
        float dgMax = g_clock.deltaTime * 9000.0f;
        gVn += dg < dgMax ? dg : dgMax;
    }
    else
    {
        float dg = gVnTarget - gVn;
        float dgMin = g_clock.deltaTime * -4500.0f;
        gVn += dg > dgMin ? dg : dgMin;
    }

    struct XPS *pxpd = &pxp->axps[ixpd];
    struct M3 *pmat = &step->matTarget;
    qword v = QwRotate(pmat, step->unknown190);
    qword normal = pxp->normal;
    qword vT = VuSub(v, VuScaleByX(normal, VuDot(v, normal)));
    struct VU_FLOAT vuVn = VuFloat(-gVn);
    struct VU_FLOAT vuK = VuFloatFromMemWord(*(int *)&step->unknown180);
    pxpd->v = QwAddScaled2(pxpd->v, normal, vuVn.qw, vT, vuK.qw);
    pxp->g += step->unknown210 - step->gUnk248;
}
} // namespace M_AdjustStepXpVelocity

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00204660 {

extern qword g_normalZ __asm__("g_normalZ");
MATRIX3 BuildAxisAngleRotationMatrix(qword v, float g) __asm__("BuildAxisAngleRotationMatrix");
struct S : PO { char pad[0x1A8 - 0x180]; float g; int pad2; MATRIX3 m; };
// Sets the target heading (radians, +0x1A8) and rebuilds the target rotation matrix (+0x1B0) about Z.
// Also known as: SetStepPhysHeading (upstream readable / Sly 1 name; SetStepPhysHeading__FP3ALOf).
void SetStepTargetHeading(struct S *p, float g) __asm__("SetStepTargetHeading"); void SetStepTargetHeading(struct S *p, float g) { p->g = g; p->m = BuildAxisAngleRotationMatrix(g_normalZ, g); }
} // namespace M_func_00204660

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SetStepRadTargetVec {
float atan2f(float y, float x) __asm__("atan2f");
void SetStepTargetHeading(void *p, float g) __asm__("SetStepTargetHeading");
// Sets the target heading toward a direction vector (atan2 of its y and x).
extern "C" void SetStepTargetHeadingFromVector(void *p, qword v) { SetStepTargetHeading(p, atan2f(QwordY(v), QwordX(v))); }
} // namespace M_SetStepRadTargetVec

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FaceStepToward {
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void SetStepTargetHeadingFromVector(void *p, qword v) __asm__("SetStepTargetHeadingFromVector");
// Turns the target heading toward another object's world position.
// Also known as: FaceObject (upstream readable / Sly 1 name; FaceObject__FP5ACTORP2LO).
extern "C" void FaceStepToward(void *step, void *q)
{
    qword a, b, d;
    a = GetLoWorldPos(q);
    b = GetLoWorldPos(step);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    SetStepTargetHeadingFromVector(step, d);
}
} // namespace M_FaceStepToward

/* The heading of the step's root transform (atan2 of its world x axis). */
// Returns the current heading: atan2 of the rotation matrix's first row (y, x).
// Also known as: RadStepHeading (upstream readable / Sly 1 name; RadStepHeading__FP4STEP).
extern "C" float GetStepHeading(STEP *step)
{
    XF *pxf = step->rootTransform;
    return atan2f(pxf->worldMatrix.mat[0][1], pxf->worldMatrix.mat[0][0]);
}

namespace RO_ORDER { extern const float r_D_002A85F4 __asm__("D_002A85F4") = 3.40282347e+38f; }
// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00204768 {
struct P;
struct Endpoint { char pad[8]; struct P *owner,*other; char pad10[0x30]; };
struct N { struct Endpoint e[2]; qword q80; float value; char pad94[8]; int kind; char padA0[0xC]; struct N *next; };
struct L { struct N *first; struct L *next; struct N **back; };
struct G { char pad[0x64]; struct L *groups; struct N *def; };
struct P : LO { char pad[0xEC - 0x50]; struct G *g; char padF0[0x118]; struct N *best; float t; char pad210[0x20]; qword position; };
extern const float D_002A85F4 __asm__("D_002A85F4");   /* defined below where the ELF has it (.rodata order) */
void AdjustSoXps(struct P *) __asm__("AdjustSoXps");
void FreeSwXpList__FP2SWP2XP(struct SW *,struct N *) __asm__("FreeSwXpList__FP2SWP2XP");
// The other endpoint of contact n, seen from p.
extern "C" { static inline struct P *Other(struct N*n,struct P*p){unsigned different=(unsigned)p^(unsigned)n->e[0].owner;int i=different>0;return ((struct Endpoint *)((char *)n+(i<<6)))->other;} }
// Slot 58: base AdjustSoXps, then keeps only the best floor contact (kind 14, lowest distance) as the cached base
// contact (+0x208) with its stamp (+0x20C) and position (+0x230).
// Also known as: FindStepFloor (upstream readable / Sly 1 name; FindStepFloor__FP3ALO).
extern "C" void AdjustStepXps(struct P *step)
{
    struct N *best;
    struct L *group;
    AdjustSoXps(step);
    {
        float value=D_002A85F4;
        group=step->g->groups;
        best=0;
        for(;;){
            struct N *node=group?group->first:step->g->def;
            for(;node;node=node->next){
                if(node->kind==14 && node->value<value){value=node->value;best=node;}
            }
            if(!group)break;
            group=group->next;
        }
    }
    step->best=best;
    if(best){step->t=g_clock.time;step->position=*(qword *)((char *)best+0x50);}
    {
        struct N *removed=0;
        group=step->g->groups;
        for(;;){
            struct N **link=group?&group->first:&step->g->def;
            struct N *node;
            while((node=*link)!=0){
                if(Other(node,step)==step && node->kind==14 && node!=best){
                    *link=node->next;
                    node->next=removed;
                    removed=node;
                }else link=&node->next;
            }
            if(!group)break;
            *group->back=group->first;
            group=group->next;
        }
        FreeSwXpList__FP2SWP2XP(&g_sw,removed);
    }
}
} // namespace M_func_00204768

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_002048C8 {
// HasStepFloorNearby (Claude agent, 2026-10-17): breadth-first polygon adjacency search. MATCH.
typedef unsigned short ushort;
union V {qword q;float f[4];};
struct E {ushort vertices[2],faces[2];struct E *next0,*next1;};
struct POLY {float normal[3];int unknown;struct E *head0,*head1;int flags,pad;};
struct GEOM {char pad94[0x94];qword *positions;int unknown;struct POLY *polygons;};
struct P : PO { char pad210[0x210 - 0x180]; float height;char pad214[0x34];float previous;};
void FindClosestPointOnLineSegment(qword,qword,qword,qword *,float *,float *) __asm__("FindClosestPointOnLineSegment");
// Returns 1 when, starting from a hit mesh polygon, a breadth-first walk over adjacent polygons (max 32) reaches
// one whose height is inside the step-height window: there is floor nearby to step onto. (inferred)
// Also known as: FStepFloorNear (upstream readable / Sly 1 name; FStepFloorNear__FP3ALOG9VU_VECTORP3SOEP4POLY).
extern "C" int HasStepFloorNearby(struct P *step,qword position,struct GEOM *g,struct POLY *first)
{
    struct POLY *queue[32];
    union V closest;
    float distance;
    int count=1,current=0;
    float delta=step->height-step->previous;
    float limit=delta*1.45f;
    float maximumNormal=0.7f;
    queue[0]=first;
    do {
        struct POLY *poly=queue[current];
        int side;
        for(side=0;side<2;side++){
            struct E *edge;
            if(!side)edge=poly->head0;else edge=poly->head1;
            while(edge){
                float z=QwordZ(position);
                FindClosestPointOnLineSegment(position,g->positions[edge->vertices[0]],g->positions[edge->vertices[1]],&closest.q,0,&distance);
                if(limit<distance)goto next;
                if(z+delta<closest.f[2])goto next;
                {
                    struct POLY *neighbor=&g->polygons[edge->faces[1-side]];
                    int j;struct POLY **pp;
                    if(maximumNormal<neighbor->normal[2])return 1;
                    for(j=0,pp=queue;;pp++,j++){
                        if(j>=count){
                            if((unsigned)count<32)queue[count++]=neighbor;
                            break;
                        }
                        if(neighbor==*pp)break;
                    }
                }
            next:
                if(!side)edge=edge->next0;else edge=edge->next1;
            }
        }
    }while(++current<count);
    return 0;
}
} // namespace M_func_002048C8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AddStepCustomXps {
struct EC { char pad[0x60]; void *rootSo; };
struct X4 : LO { char pad[0xEC - 0x50]; struct EC *tree; };
struct X { int pad; struct X4 *so; };
struct HIT { qword pos; qword unknown10; qword normal; qword unknown30; float t; int unknown44; int unknown48; int tag; char pad[0x10]; };
extern qword g_normalZ __asm__("g_normalZ");
void AddSoCustomXps(void) __asm__("AddSoCustomXps");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
int ClipEdgeToObjectPruned(struct X *x, BSP *pbsp, qword pos, qword v, int n, struct HIT *phit) __asm__("ClipEdgeToObjectPruned");
int HasStepFloorNearby(STEP *p, qword v, struct X *x, int n) __asm__("HasStepFloorNearby");
void AddXp(void *g, void *p, struct X *x, qword a, qword b, int k, int c, int d, XP **ppxpFirst, float f) __asm__("AddXp");

// Slot 60: if the ground-probe flags allow it, casts a ray down from the base point (ClipEdgeToObjectPruned) and
// adds a floor contact (kind 14) at the hit.
// Also known as: AddStepFloorXps (upstream readable / Sly 1 name; AddStepFloorXps__FP3ALOP3SOEiiii).
extern "C" void AddStepCustomXps(STEP *step, struct X *x, int cbsp, BSP *absp, BSP *pbsp, XP **ppxpFirst)
{
    unsigned int fl;
    struct HIT hit;
    qword pos, n, d, tmp;
    float f;
    AddSoCustomXps();
    fl = AsStep(step)->GetGroundProbeFlags();
    if ((fl & PROBE_ENABLED) == 0)
        return;
    if (IsBasicDerivedFrom__FP5BASIC3CID(x->so->tree->rootSo, 0x33))
        return;
    pos = GetLoWorldPos(step);
    if (ClipEdgeToObjectPruned(x, pbsp, pos, *((fl & PROBE_FROM_TOP) ? step->unknown21C : step->unknown218), 1, &hit) <= 0)
        return;
    if (0.0f < hit.t) {
        if (fl & PROBE_KEEP_NORMAL)
            n = hit.normal;
        else if (0.7f < ((float *)&hit.normal)[2])   /* a floor-like slope (normal z above 0.7): straight up */
            n = g_normalZ;
        else if (hit.unknown48)
            n = hit.normal;
        else if (HasStepFloorNearby(step, *step->unknown218, x, hit.tag))
            n = g_normalZ;
        else
            n = hit.normal;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(*step->unknown218), "j"(hit.pos));
        __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=&j"(tmp) : "j"(n));
        __asm__("mtc1 %1, %0" : "=f"(f) : "r"(d));
        AddXp(&g_sw, step->psopartBody, x, *step->unknown218, n, 0xE, 0, 0, ppxpFirst, f);   /* kind 14: the floor contact */
    }
}
} // namespace M_AddStepCustomXps

// Slot 54: a contact is being freed; drops the cached floor contact if it is that one.
// Also known as: SetStepOverlap (upstream readable / Sly 1 name; SetStepOverlap__FP3ALOT0).
extern "C" void OnStepSoContactFreed(STEP *step, SO *object)
{
    XP *pxp;
    OnSoContactFreed(step, object);
    pxp = step->floorContact;
    if ((pxp != 0) && (PSO_XP(pxp) == object)) {
        step->floorContact = 0;
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FixStepAngularVelocity {
struct B { char pad[0xB0]; qword v; };
struct S { char pad[0xC]; void *vptr; char pad2[0x5C - 0x10]; struct B *pb; char pad3[0x1A8 - 0x60]; float g; char pad4[0x1F4 - 0x1AC]; int x; };
float atan2f(float y, float x) __asm__("atan2f");
void SmoothAngle__FfffP3SMPPf(float a, float b, float c, int *p, float *pf) __asm__("SmoothAngle__FfffP3SMPPf");
// Smooths the heading toward the target heading (SmoothAngle with the spin parameters) and sets the resulting
// angular velocity about Z.
// Also known as: TurnActorNow (upstream readable / Sly 1 name; TurnActorNow__FP5ACTOR).
extern "C" void FixStepAngularVelocity(struct S *step)
{
    QWROW v;
    float *a, *b;
    v.q = step->pb->v;
    a = (float *)AsStep(step)->GetWorldMatrix();
    b = (float *)AsStep(step)->GetWorldMatrix();
    SmoothAngle__FfffP3SMPPf(atan2f(a[1], b[0]), step->g, 0.0f, &step->x, &v.f[2]);
    AsStep(step)->SetAngularVelocity(v.q);
}
} // namespace M_FixStepAngularVelocity

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00204D48 {
extern VECTOR D_002C3CB8 __asm__("D_002C3CB8");   /* the default spin smoothing */
// Scales the spin smoothing: fast/slow speeds times r, fast time divided by sqrt(r); stores r (rSpin).
// Also known as: SetStepPhysScale (upstream readable / Sly 1 name; SetStepPhysScale__FP3ALOf).
extern "C" void ScaleStepSpinSpeed(STEP *step, float g)
{
    step->vSpin = D_002C3CB8;
    step->vSpin.x *= g;
    step->vSpin.y *= g;
    step->vSpin.z *= 1.0f / SqrtFloat(g);
    step->rSpin = g;
}
} // namespace M_func_00204D48

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00204DA8 {
/* SetStepGravityRatio (Sly 1 name): scale the step's gravity to rGravity (at least 0.1) and its mass so the weight stays the same.
   Ported from Lilly's personal decomp (T0981). Each VU float is a one-qword struct so it gets the stack home the dead sqc2 go to;
   the gravity is read as a struct member, and the old ratio goes into vf3 straight from memory ("rV"). No register pins. */
#include "sly2/step.h"
struct VU_FLOAT { qword qw; };
extern void SetSoMass(struct STEP *pso, float m) __asm__("SetSoMass");   /* SetSoMass */

// Sets the gravity ratio (clamped to >= 0.1): rescales the mass and the gravity vector by the ratio change and
// stores it (+0x260).
// Also known as: SetStepPhysSize (upstream readable / Sly 1 name; SetStepPhysSize__FP3ALOf).
extern "C" void SetStepGravityRatio(struct STEP *step, float rGravity)
{
    float r = 0.1f;
    qword qwScaled;
    r = rGravity > r ? rGravity : r;
    SetSoMass(step, step->physics->m * step->gravityRatio / r);
    {
        struct VU_FLOAT vuR;
        qword qwGrav = *(qword *)&step->physics->gravity;
        __asm__("qmtc2 %1, %0" : "=j"(vuR.qw) : "r"(r));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qwScaled) : "j"(qwGrav), "j"(vuR.qw));
    }
    {
        struct VU_FLOAT vuOld;
        qword qwResult;
        __asm__("qmtc2 %1, %0" : "=j"(vuOld.qw) : "rV"(*(int *)&step->gravityRatio));
        __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(qwResult) : "j"(qwScaled), "j"(vuOld.qw));
        *(qword *)&step->physics->gravity = qwResult;
    }
    step->gravityRatio = r;
}
} // namespace M_func_00204DA8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00204E38 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
CHN *FindAsegChannel(ASEG *paseg, void *owner, int kind, int check) __asm__("FindAsegChannel");
// If the step's animation (ASEGA) is live, evaluates its translation channel for this object; else zero.
// Used for root motion (BHVANIM events, Murray, Sly's acceleration preset, dancers).
// Also known as: VelActorAnimRoot (upstream readable / Sly 1 name; VelActorAnimRoot__FP5ACTORPvf).
extern "C" qword GetStepAsegaTranslation(void *step, ASEGA *q, float g)
{
    qword r = D_002A2DD0;
    if (q && !IsAsegaRetracted(q)) {
        CHN *x = FindAsegChannel(q->animation, step, 1, 1);
        if (x)
            x->pac->Evaluate(g, q->speed, 0, &r);
    }
    return r;
}
} // namespace M_func_00204E38

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00204ED8 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
CHN *FindAsegChannel(ASEG *paseg, void *owner, int kind, int check) __asm__("FindAsegChannel");
// Same as GetStepAsegaTranslation for the rotation channel.
extern "C" qword GetStepAsegaRotation(void *step, ASEGA *q, float g)
{
    qword r = D_002A2DD0;
    if (q && !IsAsegaRetracted(q)) {
        CHN *x = FindAsegChannel(q->animation, step, 6, 1);
        if (x)
            x->pac->Evaluate(g, q->speed, 0, &r);
    }
    return r;
}
} // namespace M_func_00204ED8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00204F78 {
struct O { char pad[0x10]; struct O *parent; char p2[0xEC - 0x14]; void *tree; };
void *GetSoFromLo(void *pv) __asm__("GetSoFromLo");
void UpdateSwPox(SW *pg, void *a, void *b, int n) __asm__("UpdateSwPox");
void InvalidateSoAaox(struct O *p) __asm__("InvalidateSoAaox");
// Finds the SO up plo's parent chain that has an attachment record and links (f=1) or unlinks the step's
// attachment pair through UpdateSwPox. (inferred)
// Also known as: PairActorWith (upstream readable / Sly 1 name; PairActorWith__FP5ACTORP2LOi).
extern "C" void LinkStepAttachment(struct O *step, void *pv, int f)
{
    struct O *o = (struct O *)GetSoFromLo(pv);
    while (o && !o->tree) o = o->parent;
    if (step->tree && o && o->tree) UpdateSwPox(&g_sw, step->tree, o->tree, f ? 1 : 2);
    InvalidateSoAaox(step);
}
} // namespace M_func_00204F78

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AddStepAxisXps {
// STEP::AddAxisXps (0x205008, KB name VUnk81__4STEPiP2LOUxUxUx): add contacts (AddXp 0x1A7C18) for a point
// against the three axes of the frame BuildOrthonormalMatrixZ(direction, g_normalZ) builds. The contacts go
// to the link list entry that joins this object to the zap target of plo (or of its nearest attached
// ancestor); with no target they go to this object's own list.
struct M3 { qword r[3]; };
struct XP;
struct SOPART;
struct SO;
struct LO;
struct LINK { XP *pxpFirst; LINK *plinkNext; XP **ppxpOut; SO *pso; };
struct ATT { char pad0[0x34]; SOPART *psopart; char pad38[0x60 - 0x38]; SO *pso; LINK *plinkFirst; XP *pxpFirst; };
struct SO { char pad0[0x10]; SO *psoParent; char pad14[0xEC - 0x14]; ATT *patt; char padF0[0x258 - 0xF0]; SOPART *psopart; };
extern qword g_normalZ __asm__("g_normalZ");   // g_normalZ

SO *GetSoFromLo(LO *) __asm__("GetSoFromLo");                 // PsoZapTarget
M3 BuildOrthonormalMatrixZ(qword, qword) __asm__("BuildOrthonormalMatrixZ");          // BuildOrthonormalMatrixZ
void AddXp(void *, SOPART *, SOPART *, qword, qword, float, int, void *, void *, XP **) __asm__("AddXp");  // AddXp

// Return a - b.
static inline qword Sub(const qword &a, const qword &b) {
    qword r;
    __asm__ __volatile__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
// Transform v into the frame of m (one dot product per row); rows 1 and 2 are loaded by the asm itself.
static inline qword Rotate(const M3 &m, qword v) {
    qword one, a, b, c, r;
    __asm__("lqc2 $vf2,0x20(%7)\n\tlqc2 $vf1,0x10(%7)\n\tvmul.xyzw %1,%5,%6\n\tvmaxw.xyzw %0,$vf0,$vf0w\n\tvmul.xyzw %2,$vf1,%6\n\tvmul.xyzw %3,$vf2,%6\n\tvadday.x ACC,%1,%1y\n\tvmaddz.x %4,%0,%1z\n\tvaddax.y ACC,%2,%2x\n\tvmaddz.y %4,%0,%2z\n\tvaddax.z ACC,%3,%3x\n\tvmaddy.z %4,%0,%3y"
        : "=&j"(one), "=&j"(a), "=&j"(b), "=&j"(c), "=&j"(r) : "j"(m.r[0]), "j"(v), "r"(&m) : "vf1", "vf2");
    return r;
}

// Slot 82: adds up to three contacts at a point along the axes of an orthonormal frame built around dir, one per
// grf bit.
// Also known as: AddActorLineXps (upstream readable / Sly 1 name; AddActorLineXps__FP5ACTORiP6VECTORG9VU_VECTORN23).
extern "C" void AddStepAxisXps(SO *object, int flags, LO *plo, qword posOther, qword direction, qword pos)
{
    XP **ppxpFirst, **ppxpOut;
    SOPART *psopartOther;
    SO *psoTarget = GetSoFromLo(plo);
    ATT *patt, *pattThis;
    if (psoTarget) {
        patt = psoTarget->patt;
        pattThis = object->patt;
        if (!patt) {
            while ((psoTarget = psoTarget->psoParent) != 0) {
                patt = psoTarget->patt;
                if (patt) break;
            }
        }
    }
    if (psoTarget) {
        SO *psoOwner = patt->pso;
        LINK *plink = pattThis->plinkFirst;
        while (plink && plink->pso != psoOwner) plink = plink->plinkNext;
        ppxpFirst = &plink->pxpFirst;
        ppxpOut = plink->ppxpOut;
        psopartOther = psoOwner->patt->psopart;
    } else {
        psopartOther = 0;
        ppxpFirst = ppxpOut = &object->patt->pxpFirst;
    }
    M3 mat = BuildOrthonormalMatrixZ(direction, g_normalZ);
    qword posLocal = Rotate(mat, Sub(pos, posOther));
    if ((flags & 1) || ((flags & 8) && QwordX(posLocal) < 0.0001f))
        AddXp(&g_sw, object->psopart, psopartOther, pos, mat.r[0], QwordX(posLocal), (flags & 1) ? 11 : 10, 0, 0, ppxpFirst);
    if ((flags & 2) || ((flags & 0x10) && QwordY(posLocal) < 0.0001f))
        AddXp(&g_sw, object->psopart, psopartOther, pos, mat.r[1], QwordY(posLocal), (flags & 2) ? 11 : 10, 0, 0, ppxpFirst);
    if ((flags & 4) || ((flags & 0x20) && QwordZ(posLocal) < 0.0001f))
        AddXp(&g_sw, object->psopart, psopartOther, pos, mat.r[2], QwordZ(posLocal), (flags & 4) ? 11 : 14, 0, 0, ppxpFirst);
    *ppxpOut = *ppxpFirst;
}
} // namespace M_AddStepAxisXps

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_MatchStepXf {
// Set the parent matrix, position and inherited motion.
typedef union U { qword q; float f; } U;

typedef struct M4 { qword row[4]; } M4;
typedef struct B {int a,b,c; VTE *vt;} B;
typedef struct P {
    B base;
    char pad[0x208 - 0x10]; int count;
    char pad2[0x240 - 0x20C]; qword offset;
    char pad3[0x1048 - 0x250]; FLOATWORD amount;
    char pad4[0x1060 - 0x104C]; qword adjusted;
} P;
extern qword D_002A2DD0 __asm__("D_002A2DD0"), g_normalZ __asm__("g_normalZ");
extern int IsBasicDerivedFrom__FP5BASIC3CID(B *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
extern void MatchSoXf(P *, B *) __asm__("MatchSoXf");
// Transform v by the matrix rows with `last` as the translation, storing it to *published; returns the result through
// a GPR.
extern "C" { static inline qword Transform(const M4*m,qword v,qword last,qword*published){
 register qword result __asm__("vf7"),argument __asm__("$5"),origin __asm__("vf1")=last;
 __asm__ volatile("vmulax.xyz $ACC,%3,%7x\n\tvmadday.xyz $ACC,%4,%7y\n\tvmaddaz.xyz $ACC,%5,%7z\n\tvmaddw.xyz %0,%6,$vf0w\n\tqmfc2.ni %1,%0\n\tsqc2 %6,%2"
  :"=&j"(result),"=&r"(argument),"=m"(*published):"j"(m->row[0]),"j"(m->row[1]),"j"(m->row[2]),"j"(origin),"j"(v));
 return argument;
} }
// Broadcast a float into vf6.
extern "C" { static inline qword Broadcast(float f){register qword r __asm__("vf6");__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));return r;} }
// Return v scaled by s.x (vf2).
extern "C" { static inline qword MulX(qword v,qword s){register qword r __asm__("vf2");__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(s));return r;} }
// Slot 38: copies another object's pose (its world matrix, including the base offset +0x240) and resets the
// velocities; non-STEP sources fall back to MatchSoXf.
// Also known as: TeleportActor (upstream readable / Sly 1 name; TeleportActor__FP2LOT0).
extern "C" { extern void MatchStepXf(P *step, B *other)
{
    if (!other) return;
    if (IsBasicDerivedFrom__FP5BASIC3CID(other, 3)) {
        M4 m;
        U scale;
        register qword v __asm__("vf2");
        register VTE *set __asm__("a2");
        const M4 *source = (const M4 *)AsLo(other)->GetWorldMatrix();
        m.row[0] = source->row[0];
        set = step->base.vt;
        {qword sc = Broadcast(-1.0f);
        m.row[1] = source->row[1];
        v = MulX(step->offset, sc);
        m.row[2] = source->row[2];
        {qword t = Transform(&m,v,source->row[3],&m.row[3]);
        P *self = (P *)((char *)step+set[29].d);
        register void (*positionFn)(P*,qword) __asm__("v1")=(void(*)(P*,qword))set[29].pfn;
        scale.q = sc;
        positionFn(self, t);}}
        AsStep(step)->RotateToMat((MATRIX3 *)&m);
        if (IsBasicDerivedFrom__FP5BASIC3CID(&step->base,0x34)) {
            qword r;
            qword scq;
            __asm__("qmtc2.ni %1,%0" : "=j"(scq) : "r"(step->amount.bits));
            scale.q = scq;
            r = m.row[3];
            __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x" : "+j"(r) : "j"(g_normalZ), "j"(scale.q));
            step->adjusted = r;
        }
        AsStep(step)->SetVelocity(D_002A2DD0);
        AsStep(step)->SetAngularVelocity(D_002A2DD0);
        step->count = 0;
    } else {
        MatchSoXf(step,other);
    }
} }

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_scalar[(sizeof(U)==16)?1:-1];

typedef char layout_matrix[(sizeof(M4)==0x40)?1:-1];
typedef char layout_matrix_0[((unsigned)&((M4*)0)->row[0]==0)?1:-1];
typedef char layout_matrix_1[((unsigned)&((M4*)0)->row[1]==0x10)?1:-1];
typedef char layout_matrix_2[((unsigned)&((M4*)0)->row[2]==0x20)?1:-1];
typedef char layout_matrix_3[((unsigned)&((M4*)0)->row[3]==0x30)?1:-1];
typedef char layout_base[(sizeof(B)==0x10)?1:-1];
typedef char layout_vtable[((unsigned)&((B*)0)->vt==0xC)?1:-1];
typedef char layout_count[((unsigned)&((P*)0)->count==0x208)?1:-1];
typedef char layout_offset[((unsigned)&((P*)0)->offset==0x240)?1:-1];
typedef char layout_amount[((unsigned)&((P*)0)->amount==0x1048)?1:-1];
typedef char layout_adjusted[((unsigned)&((P*)0)->adjusted==0x1060)?1:-1];
} // namespace M_MatchStepXf

// Pushes a new behaviour onto the step's behaviour stack and initializes it (the class id passes through to
// PushBhvstk in a1).
extern "C" BHV *PushStepBehavior(STEP *step)
{
    return PushBhvstk(&step->behaviors);
}

// Finds the behaviour of class cid (or derived from it) on the step's behaviour stack.
extern "C" BHV *FindStepBehavior(STEP *step, CID cid)
{
    return FindBhvInBhvstk(&step->behaviors, cid);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00205478 {
int GetDialogRoleSmOid(void *p, int i) __asm__("GetDialogRoleSmOid");
void *FindSwObject__FP2SWi3OIDP2LO(void *p, int a, int b, STEP *ps) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void *FindSmaForObject__FP2SMP3ALO(void *x, STEP *ps) __asm__("FindSmaForObject__FP2SMP3ALO");
void ApplySmGetSma(void *x, STEP *ps, int a, int b, void **ppv) __asm__("ApplySmGetSma");
// Slot 83: for each set bit (< 6) of the mask increments that dialog role's refcount (acrefDialogSm); the first reference
// ensures a state-machine instance (SMA) of the role's dialog SM. (inferred)
extern "C" void AddStepDialogSmRefs(STEP *step, int mask)
{
    int i;
    for (i = 0; i < 6; i++) {
        int f = (unsigned int)i < 6;
        int bit = 1 << i;
        if (!f) bit = 0;
        if (mask & bit) {
            if (++step->acrefDialogSm[i] < 2) {
                int id = GetDialogRoleSmOid(&g_sndmgr, i);
                void *x = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, id, step);
                if (x) {
                    void *y = FindSmaForObject__FP2SMP3ALO(x, step);
                    if (!y)
                        ApplySmGetSma(x, step, -1, 9, &y);
                }
            }
        }
    }
}
} // namespace M_func_00205478

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00205588 {
int GetDialogRoleSmOid(void *p, int i) __asm__("GetDialogRoleSmOid");
void *FindSwObject__FP2SWi3OIDP2LO(void *p, int a, int b, STEP *ps) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void *FindSmaForObject__FP2SMP3ALO(void *x, STEP *ps) __asm__("FindSmaForObject__FP2SMP3ALO");
void RetractSma(void *y) __asm__("RetractSma");
// Slot 84: the inverse of AddStepDialogSmRefs; at refcount 0 the role's SMA is retracted. (inferred)
extern "C" void RemoveStepDialogSmRefs(STEP *step, int mask)
{
    int i;
    for (i = 0; i < 6; i++) {
        int f = (unsigned int)i < 6;
        int bit = 1 << i;
        if (!f) bit = 0;
        if (mask & bit) {
            if (step->acrefDialogSm[i] != 0) {
                if (--step->acrefDialogSm[i] <= 0) {
                    int id = GetDialogRoleSmOid(&g_sndmgr, i);
                    void *x = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, id, step);
                    if (x) {
                        void *y = FindSmaForObject__FP2SMP3ALO(x, step);
                        if (y)
                            RetractSma(y);
                    }
                }
            }
        }
    }
}
} // namespace M_func_00205588

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00205670 {
extern qword g_normalZ __asm__("g_normalZ");
void BuildStepFootprint(STEP*,QWROW*) __asm__("BuildStepFootprint");
int IntersectRayTriangle(qword,qword,qword,qword,qword,float*,float*) __asm__("IntersectRayTriangle");
// Return *a - *b (vf3).
extern "C" { static inline qword Sub(const qword*a,const qword*b){qword av=*a,bv=*b;register qword r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(av),"j"(bv));return r;} }
// Return a scaled by f (broadcast through k).
extern "C" { static inline qword Scale(qword a,float f,QWROW *k){qword q,r;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));k->q=q;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(k->q));return r;} }
// Normalize *v in place (or set it to *fallback when its length is below 1e-4); returns the length.
extern "C" { static inline float Normal(QWROW*v,const qword*fallback){qword a=v->q;register qword d __asm__("vf1");register qword one __asm__("vf2");float g,r;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(one):"j"(d));
__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(one));
if(g<0.0001f)v->q=*fallback;
else {register qword normalized __asm__("vf1");__asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"=&j"(normalized):"j"(a),"j"(one));v->q=normalized;}
__asm__("mtc1 %1,%0":"=f"(r):"r"(one));return r;} }
// When the step has a floor contact, intersects the segment from->to with its two footprint triangles; returns 1
// and the hit point on a hit.
// Also known as: HitStepSupport (upstream readable / Sly 1 name; HitStepSupport__FP3ALOG9VU_VECTORT1P6VECTOR).
extern "C" int IntersectStepFootprint(STEP*step,qword from,qword to,qword*output)
{
 QWROW tri[6],d,k;
 float distance,length;
 register qword *out __asm__("s4")=output;
 register QWROW *t __asm__("s1");
 register QWROW *third __asm__("s0");
 register QWROW *end __asm__("s2");
 register float *distanceOut __asm__("s3");
 if(!step->floorContact)return 0;
 BuildStepFootprint(step,tri);
 d.q=Sub(&to,&from);
 length=Normal(&d,&g_normalZ);
 distanceOut=&distance;third=&tri[2];t=tri;end=tri+6;
 do{
   int hit=IntersectRayTriangle(d.q,from,*(qword*)((int)third-32),*(qword*)((int)third-16),third->q,distanceOut,0);
   if(distance<=length && (hit&1)){
      *out=VuAdd(from,Scale(d.q,distance,&k));return 1;
   }
   t+=3;third+=3;
 }while(t<end);
 return 0;
}
} // namespace M_func_00205670

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_002057E8 {
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct M3 { qword r[3]; };

M3 *GetLoWorldMatrix(LO *p) __asm__("GetLoWorldMatrix");
qword GetLoWorldPos(LO *p) __asm__("GetLoWorldPos");
extern int D_002C3CC8[2][3] __asm__("D_002C3CC8");

// Return v scaled by g.
static inline qword MulX(qword v, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
// Load a vector into a VU0 register.
static inline qword LoadVf(qword *p) { qword r; __asm__("lqc2 %0, %1" : "=j"(r) : "m"(*p)); return r; }
// Builds the two footprint triangles: a diamond of radius baseRadius around the base point (*unknown218), oriented by the
// world matrix.
extern "C" void BuildStepFootprint(STEP *step, qword out[2][3])
{
    qword point[5];
    qword origin = *step->unknown218;
    qword right = GetLoWorldMatrix(step)->r[0];
    qword forward = LoadVf(&GetLoWorldMatrix(step)->r[1]);
    point[0] = GetLoWorldPos(step);
    float radius = step->baseRadius;
    point[1] = VuAdd(origin, MulX(right, VU_FLOAT(radius)));
    point[2] = VuAdd(origin, MulX(forward, VU_FLOAT(radius)));
    point[3] = VuAdd(origin, MulX(right, VU_FLOAT(-radius)));
    point[4] = VuAdd(origin, MulX(forward, VU_FLOAT(-radius)));
    for (unsigned i = 0; i < 2; i++) {
        int *indices = D_002C3CC8[i];
        qword *dest = out[i];
        for (unsigned j = 0; j < 3; j++)
            dest[j] = *(qword *)((indices[j] << 4) + (unsigned int)point);
    }
}
} // namespace M_func_002057E8

namespace RO_ORDER { extern const float r_D_002A85F8 __asm__("D_002A85F8") = -3.40282347e+38f; }
// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00205910 {
/* FitStepToTerrain (Lilly / Claude agent, T1593): terrain-fit of the crawl probes (our name UpdateCrawlCollide, 0x205910; see
 * re/symbols: the 3-probe floor follower of the vent crawl, called from UpdateJtGrab case 8).  MATCH, no register pins.
 * What it does: the actor's orientation is turned about the up axis (g_normalZ) by its heading (p+0x1A8) into a position+rotation
 * matrix matCur (copied to matOrig); the largest probe radius (length of the local probe points, start -FLT_MAX D_002A85F8) is the
 * sphere for the nearby-objects query (IntersectSwBoundingSphere, filter ShouldKeepSoNotSelfSlot62 with p+0xEC as context).  Then D_002C3CE4 passes: every
 * probe point is placed with matCur and snapped to the floor by the line test SnapToGround (height->max / height->min); a probe
 * that finds no floor is dropped by D_002C3CE0 along matCur's up row.  The plane through the three probes gives a normal (cross of
 * the two edges, g_normalZ if shorter than 0.0001), BuildRotateVectorsMatrix builds the rotation from matOrig's up row to it, DecomposeRotateMatrixRad
 * measures its angle, which is limited to D_002C3CE8 (BuildAxisAngleRotationMatrix rebuilds the rotation about the same axis with the limit),
 * and matCur = adjustment * matOrig.  The final rotation is stored at p+0x1B0.
 * How it matches (everything came from the personal decomp's recipes, nothing from register pins):
 *  - M3 * M3 is the game's inline NRV operator* (three dead VU_VECTOR4 homes at +0x140), `pmatOrig` is a pointer declared in the loop
 *    so rows 1/2 of matOrig are read through a hoisted pointer register while row 0 is `0x40(sp)`.
 *  - the first rotation is an NRV wrapper over BuildAxisAngleRotationMatrix (hidden pointer = plain `addiu a0,sp,0x80`); the loop's calls return
 *    the matrix by value (hidden pointer = a hoisted pseudo, s7).  `adjustment`/`angle` are declared after the probe loop so their
 *    frame slots come after the VU_FLOAT home of the drop scale (0xD0).
 *  - normalise-or-fallback = QwLengthUntied form (the fallback is a pointer computed before the cross product, `lq` after it);
 *    QwAddScaled adds in place ("+j" on the base) so the point/scale vf numbers match.
 *  - the frame object has its constructor/dtor called explicitly through a `frame` pointer: the RAII form loses the frame-address
 *    pseudo that the game spills at sp+0x188 (frame 0x250 instead of 0x260).
 * NOT natural: GCSE_PAD.  The spill slots 0x188/0x18C/0x190 (frame, &angle, &adjustment: gcse PRE pseudos) come out in the game's
 * order only when gcse's expression hash table has 155..173 buckets (bucket = (13259 + frame offset) mod (insns/2 | 1)); the plain
 * source has 109 (218 insns).  110 empty `asm volatile` after the last call (nothing is scheduled behind them) bring it to 165
 * without changing a single instruction.  The game's source has ~100 more insns at that point that are gone before the final
 * code; I could not find which (all-separate VU asms only add ~20).  Without the pad: 5/254 (only those slot numbers).
 */
struct MATRIX4 {float mat[4][4];} __attribute__((aligned(16)));
struct VU_VECTOR4 {qword qw;};
union U {qword q;float f[4];int i[4];};
struct VU_FLOAT {qword qw;};
// Empty scope object (its constructor and destructor only shape the generated code).
struct FRAME {FRAME(){}~FRAME(){}};

struct HEIGHT {float min,max;};
extern qword g_normalZ __asm__("g_normalZ");
extern const float D_002A85F8 __asm__("D_002A85F8");   /* defined below where the ELF has it (.rodata order) */
extern float D_002C3CE0 __asm__("D_002C3CE0");   /* the probe drop along the up row */
extern int D_002C3CE4 __asm__("D_002C3CE4");
extern float D_002C3CE8 __asm__("D_002C3CE8");    /* the angle limit */

MATRIX3 BuildAxisAngleRotationMatrix(qword,float) __asm__("BuildAxisAngleRotationMatrix");
MATRIX3 *func_0011B500r(MATRIX3 *,qword,float) __asm__("BuildAxisAngleRotationMatrix");
MATRIX3 BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(qword,qword) SLY2_SRET_LABEL("BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3", "BuildRotateVectorsMatrixValue");
qword GetLoWorldPos(STEP *) __asm__("GetLoWorldPos");
qword *VECTOR_opAssign(qword *,qword) __asm__("VECTOR_opAssign");
FRAME *STACKSCOPE_ctor(FRAME *) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(FRAME *,int) __asm__("_$_10STACKSCOPE");
void IntersectSwBoundingSphere(SW *,qword,float,int (*)(void *,void *),void *,int *,void ***) __asm__("IntersectSwBoundingSphere");
int ShouldKeepSoNotSelfSlot62(void *,void *) __asm__("ShouldKeepSoNotSelfSlot62");
int SnapToGround(qword *,float,float,int,void *) __asm__("SnapToGround");
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(MATRIX3 *,float *,qword *) __asm__("DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR");

// Length of v as a float.
static inline float GLength(qword v){return VuToFloat(VuLength(v));}
// Length of v (x lane), with the squared length kept in its own register.
static inline qword QwLengthUntied(qword v){qword qwSq,vfOne,qwLen;__asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z":"=j"(qwSq),"=j"(vfOne):"j"(v));__asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q":"=j"(qwLen):"j"(qwSq));return qwLen;}
// Transform point qwPos by the 4x4 matrix pmat.
static inline qword QwTransformPos(MATRIX4 *pmat,qword qwPos)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w":"=j"(r):"j"(*(qword *)pmat->mat[0]),"j"(*(qword *)pmat->mat[1]),"j"(*(qword *)pmat->mat[2]),"j"(*(qword *)pmat->mat[3]),"j"(qwPos));
    return r;
}
// Broadcast the float word n into a VU0 register.
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2 %1, %0":"=j"(v.qw):"rV"(n));return v;}
// Return base + vec * k.
static inline qword QwAddScaled(qword base,qword vec,VU_FLOAT k){__asm__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x":"+j"(base):"j"(vec),"j"(k.qw));return base;}
// 3x3 matrix product matL * matR.
static inline MATRIX3 operator*(const MATRIX3 &matL,const MATRIX3 &matR) return mat
{
    qword qwL0=*(qword *)matL.mat[0];
    qword qwL1=*(qword *)matL.mat[1];
    qword qwL2=*(qword *)matL.mat[2];
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z":"=j"(v0.qw):"j"(qwL0),"j"(qwL1),"j"(qwL2),"j"(((VU_VECTOR4 *)matR.mat[0])->qw));
    *(VU_VECTOR4 *)mat.mat[0]=v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z":"=j"(v1.qw):"j"(qwL0),"j"(qwL1),"j"(qwL2),"j"(((VU_VECTOR4 *)matR.mat[1])->qw));
    *(VU_VECTOR4 *)mat.mat[1]=v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z":"=j"(v2.qw):"j"(qwL0),"j"(qwL1),"j"(qwL2),"j"(((VU_VECTOR4 *)matR.mat[2])->qw));
    *(VU_VECTOR4 *)mat.mat[2]=v2;
}
// Rotation matrix about axis by angle.
static inline MATRIX3 Rotation(qword axis,float angle) return r {func_0011B500r(&r,axis,angle);}
// Build a 4x4 matrix from rotation rot and translation pos.
static inline void InitMat(MATRIX4 *pm,const MATRIX3 &rot,qword pos)
{
    VECTOR_opAssign((qword *)pm->mat[3],pos);
    *(MATRIX3 *)pm=rot;
    pm->mat[3][3]=1.0f;
    pm->mat[2][3]=0;pm->mat[1][3]=0;pm->mat[0][3]=0;
}
// 10 empty asm volatile (see the header: gcse hash-table size)
#define GCSE_PAD10 __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile("");
// Fits the step's orientation to the floor under three probe points (bounding-sphere query and line tests), with
// the tilt limited to a maximum angle. (inferred)
// Also known as: FitStepToFloor (upstream readable / Sly 1 name; FitStepToFloor__FP3ALOP6VECTORiPG9VU_VECTOR).
extern "C" void FitStepToTerrain(STEP *step,HEIGHT *height,int pointCount,const qword *localPoints)
{
    MATRIX4 matCur,matOrig;
    InitMat(&matCur,Rotation(g_normalZ,step->targetHeading),GetLoWorldPos(step));
    matOrig=matCur;
    float radius=D_002A85F8;
    for(int i=0;i<pointCount;i++){float length=GLength(localPoints[i]);radius=(radius>length)?radius:length;}
    FRAME storage;
    FRAME *frame=&storage;
    STACKSCOPE_ctor(frame);
    int nearbyCount;void **nearby;
    IntersectSwBoundingSphere(&g_sw,*(qword *)matCur.mat[3],radius,ShouldKeepSoNotSelfSlot62,step->physicsTree,&nearbyCount,&nearby);
    U points[3];
    U point;
    for(int pass=0;pass<D_002C3CE4;pass++){
        qword up=*(qword *)matCur.mat[2];
        for(int i=0;i<pointCount;i++){
            point.q=QwTransformPos(&matCur,localPoints[i]);
            if(SnapToGround(&point.q,height->max,height->min,0,0))points[i].q=point.q;
            else points[i].q=QwAddScaled(point.q,up,VuFloatFromMemWord(*(int *)&D_002C3CE0));
        }
        MATRIX3 adjustment;
        float angle;
        const qword *pFallback=&g_normalZ;
        qword origin=points[0].q;
        qword first=VuSub(points[1].q,origin);
        qword second=VuSub(points[2].q,origin);
        qword qwCross=VuCross(first,second);
        qword qwFallback=*pFallback;
        qword qwLen=QwLengthUntied(qwCross);
        qword normal;
        if(VuToFloat(qwLen)<0.0001f)normal=qwFallback;
        else{__asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq": :"j"(qwLen));__asm__("vmulq.xyzw %0, %1, $Q":"=j"(normal):"j"(qwCross));}
        adjustment=BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(*(qword *)matOrig.mat[2],normal);
        DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&adjustment,&angle,&point.q);
        if(D_002C3CE8<angle)adjustment=BuildAxisAngleRotationMatrix(point.q,D_002C3CE8);
        const MATRIX3 *pmatOrig=(MATRIX3 *)&matOrig;
        *(MATRIX3 *)&matCur=adjustment**pmatOrig;
        matCur.mat[2][3]=0;matCur.mat[1][3]=0;matCur.mat[0][3]=0;
    }
    step->matTarget=*(MATRIX3 *)&matCur;
    STACKSCOPE_ctor_1AE4A0(frame,2);
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
}
} // namespace M_func_00205910

// Slot 86: the base answer 0; STEPGUARD's override tests whether the current zap is lethal (inferred).
extern "C" int IsStepZapLethal()
{
    return 0;
}

// Slot 87: empty in the base class.
extern "C" void IgnoreStepSlot87()
{
}


// ---- step's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
namespace M_UpdateStepPhys {
float D_002C3C60[2] __asm__("D_002C3C60") = {1.0f, -1.0f};
float D_002C3C68[3] __asm__("D_002C3C68") = {300.0f, 0.0f, 0.25f};
} // namespace M_UpdateStepPhys extern "C"
namespace M_UpdateStep {
float D_002C3C80[3] __asm__("D_002C3C80") __attribute__((aligned(16))) = {1.5f, -1.0f, 0.0f};
} // namespace M_UpdateStep extern "C"
namespace M_AdjustStepNewXp {
float D_002C3C90 __asm__("D_002C3C90") __attribute__((aligned(16))) = 1.5f;
} // namespace M_AdjustStepNewXp extern "C"
namespace M_AdjustStepXpVelocity {
extern float D_002C3C98[3] __asm__("D_002C3C98") = {3000.0f, 0.0f, 0.35f};
extern float D_002C3CA8[3] __asm__("D_002C3CA8") = {3000.0f, 0.0f, 0.5f};
} // namespace M_AdjustStepXpVelocity
namespace M_func_00204D48 {
VECTOR D_002C3CB8 __asm__("D_002C3CB8") = {25.0f, 0.0f, 0.25f};
} // namespace M_func_00204D48 extern "C"
namespace M_func_002057E8 {
int D_002C3CC8[2][3] __asm__("D_002C3CC8") = {{0, 1, 3}, {0, 2, 4}};
} // namespace M_func_002057E8 extern "C"
namespace M_func_00205910 {
extern float D_002C3CE0 __asm__("D_002C3CE0") = -24.0f;
extern int D_002C3CE4 __asm__("D_002C3CE4") = 2;
extern float D_002C3CE8 __asm__("D_002C3CE8") = 0.6f;
}
