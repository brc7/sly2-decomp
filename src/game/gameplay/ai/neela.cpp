// neela.cpp -- NEELA (cid 0x3C, a STEPGUARD): she picks a target inside one of two volumes (another guard, or a
// cid 0x62 object) and turns her animated prop toward it; and her chase behaviour (BHV174, flag 0x25) with three
// states: player inside the near volume (resume her script), outside (chase the player), timed out (give up, game
// event 0x33). VU0 asm.
// TU 0x1B9F80..0x1BAB50 (14 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_neela
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_neela
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/stackscope.h"
#include "sly2/clock.h"
#include "sly2/game.h"

// 0x1BA9F8 StaticInit_neela and GlobalConstructors_neela: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_CLASS98 = 0x62,
    CID_BHVGUARD = 0x9C,
    CID_BHVANIM = 0x9D,
    CID_BHVIDLE = 0x9E,
    CID_BHVWALKPATH = 0xA7,
    CID_BHV169 = 0xA9,
    CID_BHVRAIL = 0xB8,
};
/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_IDLE = 0x2,
    SGF_KO = 0x7,
    SGF_MOVING = 0x9,
    SGF_NEELA_CHASE = 0x25,   // BHV174
};

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
#include "sly2/sw.h"
#include "sly2/classes.h"
union U{float f[4];qword q;};   /* floats first, so a U global can be initialized */
struct M3{qword rows[3];};
/* NEELA::propPart, the part turned toward the target, is an XF (its local position, row 3, and its parent). */
/* NEELA::aimAct, the act driving the part (an ACT: its angle goal at +0x20). */
struct MOTION{char pad[0x20];qword angles;};
struct L { char pad[0x30]; qword unknown30; };
/* Neela is classes.h NEELA (a STEPGUARD), her chase behaviour BHV174; the sub-behaviours it pushes are BHV169,
   BHVIDLE and BHVANIM. */
struct ROW{int key;LO*object;};
extern "C"{
void InitStepguard(NEELA *p);
void PostStepguardLoad(NEELA *p);
void BindLoOidTable(NEELA *p, int n, void *pv);
float *FindAsegLabel(void *p, int n);
void PredictAsegTransform(void *p, NEELA *ps, void *p1, XF *pspace, L *pl, int m, float a, float b);
void SetSoMass(NEELA *p, float g);
void ResetStepguardToHome(NEELA *p, LO *plo);
int GetBasicCid(void *pv);
void UpdateStepguard(NEELA*);
int StepguardHasSgf(STEPGUARD*,int);
qword GetLoWorldPos(void*);
qword ConvertPosBetweenLos(NEELA*,void*,qword);
int IsPointInVol(VOL*,qword);
void SetNeelaState(NEELA*,int);
int IsAsegaRetracted(ASEGA*);
qword TransformXfPos(XF*,XF*,qword);
M3*BuildRotateVectorsMatrix__FUxUx(M3*,qword,qword);
qword DecomposeRotationMatrixToEuler(M3*);
int FilterCid98(void*,ROW*);
void IntersectSwBoundingBox(SW*,qword,qword,int(*)(void*,ROW*),void*,int*,ROW***);
void RetractAsega(ASEGA *o);
void ReleaseAct(struct ACT **p);
BHV *FindStepBehavior(NEELA *p, int n);
void ScaleStepSpinSpeed(NEELA *p, float f);
void ApplyAsegCore(void *a, NEELA *p, int n, ASEGA **ppo, float f1, float f2);
struct ACT *NewPositionAct(NEELA *p, XF *a, int b, int c, int d, qword q);
void InitBhv(BHV174 *p);
void StartBhv(BHV174 *p);
void CloseBhv(BHV174 *p);
void UpdateBhv(BHV174 *p);
int IsBhvBusyAbove(BHV174 *p);
void AddStepguardSgf(NEELA *p, int n);
void RemoveStepguardSgf(NEELA *p, int n);
void SetBhvNeelaChaseState(BHV174 *p, int n);
void SaveBhvScriptResumeState(BHV *a, SSGRESUME *b);
void PopBhv(BHV **p);
BHV *PushStepBehavior(NEELA *p, int b);
void SetBhvScriptScript(BHV169 *o, GUARDSCRIPT *a, int b, SSGRESUME *c);
void BroadcastGameEvent(void *p, int a, NEELA *b);
void *func_002333B4(void *p, int c, int cb) __asm__("memset");
void SetBhvguardMode(BHV *p, BHVGUARDRECDATA *pf);
extern const float D_002A7620; /* defined below where the ELF has it (.rodata order) */
/* neela's .data (address order): the snip table, then NEELA's target volume: the centre offset and the box around it. */
struct SNIP {
    int grfsnip;
    int oid;
    int ib;
}; /* Sly 1's SnipLoObjects record */
SNIP D_002C0A50[3] = {{0x200, 0x5D8, SLY2_OFFSETOF(NEELA, propAnimation)}, {0x10, 0x5D9, SLY2_OFFSETOF(NEELA, unknown1030)}, {0x10, 0x5DA, SLY2_OFFSETOF(NEELA, propPart)}};
U D_002C0A80 = {{400.0f, 0.0f, 70.0f, 0.0f}};
U D_002C0A90 = {{-500.0f, -500.0f, -300.0f, 0.0f}};
U D_002C0AA0 = {{500.0f, 500.0f, 300.0f, 0.0f}};
}
extern void *D_002E1E40;    /* the player */
extern qword D_002A2DD0;

// 0x1B9F80: NEELA vtable slot 7 (Init): STEPGUARD Init plus her movement tuning constants and flags.
void NEELA::Init()
{
    NEELA *neela = this;
    InitStepguard(neela);
    CLQ *ppoly = &neela->chaseTune.speedCurve;
    ppoly->g0 = 1.35f;
    ppoly->g1 = -0.0003f;
    ppoly->g2 = 0.0f;
    neela->chaseTune.speedRange.minValue = 0.9f;
    neela->chaseTune.speedRange.maxValue = 1.2f;
    neela->chaseTune.giveUpTime = 7.0f;
    neela->unknown364 = 0;
    neela->sensesEnabled = 0;
    neela->pdetector->options = 3;
    neela->tracksRelocatePoint = 1;
}

// 0x1BA018: NEELA vtable slot 15 (PostLoad): STEPGUARD post-load, binds her oid table, precomputes the prop
// animation's effect offset (label 0x5DB) and sets her mass to 0.25.
extern "C" void PostNeelaLoad(NEELA *neela)
{
    L l;
    qword a, b, d;
    float *pg;
    PostStepguardLoad(neela);
    BindLoOidTable(neela, 3, D_002C0A50);
    pg = FindAsegLabel(neela->propAnimation, 0x5DB);
    PredictAsegTransform(neela->propAnimation, neela, neela->unknown1030, ((XF *)neela->propPart)->parent, &l, 0, *pg, 1.0f);
    a = l.unknown30;
    b = ((XF *)neela->propPart)->localMatq.r[3].q;
    d = VuSub(a, b);
    neela->giveUpBinding.kind = 0;
    neela->propAxis = d;
    neela->giveUpBinding.flags = 1;
    SetSoMass(neela, 0.25f);
}

// 0x1BA0B8: NEELA vtable slot 26 (reset to home): ResetStepguardToHome, then hides her attached prop (its vtable
// slot 9).
extern "C" void ResetNeelaToHome(NEELA *neela, LO *plo)
{
    LO *po;
    ResetStepguardToHome(neela, plo);
    po = neela->prop;
    if (po)
        po->Remove();
}

// 0x1BA100: Box-query filter: accepts only cid 0x62 objects.
extern "C" int FilterCid98(void *pv, ROW *prow)
{
    return GetBasicCid(prow->object) == CID_CLASS98;
}

// VU0 helper: moves a CPU quadword into a VU register.
static inline qword Vector(qword r){qword v;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(r));return v;}
// VU0 helper: squared length of a vector.
static inline float Square(qword a)
{
    float f;
    a = VuDot(a, a);
    f = VuToFloat(a);
    return f;
}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char ROW_Size[sizeof(ROW)==8?1:-1];
typedef char MOTION_angles_Offset[FIELD_OFFSET(MOTION,angles)==32?1:-1];
typedef char ROW_object_Offset[FIELD_OFFSET(ROW,object)==4?1:-1];
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A7620 __asm__("D_002A7620") = 3.40282347e+38f; }
// 0x1BA128: NEELA vtable slot 16 (Update): STEPGUARD update; state 0: with flag 0x25 in modes 9/2, looks for the
// nearest other guard in a volume or a cid 0x62 object in a box and goes to state 1; state 1: aims her prop at the
// target until its animation ends.
extern "C" void UpdateNeela(NEELA *neela)
{
    UpdateStepguard(neela);
    switch (neela->state) {
    case 0: {
        if (!StepguardHasSgf(neela, SGF_NEELA_CHASE))
            break;
        VOL *volume;
        if (StepguardHasSgf(neela, SGF_MOVING)) {
            volume = neela->moveVolume;
            neela->target = 0;
        } else if (StepguardHasSgf(neela, SGF_IDLE)) {
            volume = neela->idleVolume;
            neela->target = 0;
        } else
            break;
        float nearest = D_002A7620;
        for (STEPGUARD *node = (*(STEPGUARD **)&g_sw.dlStepguard); node; node = (STEPGUARD *)node->dleGuard.next) {
            if (node == neela || StepguardHasSgf(node, SGF_KO))
                continue;
            qword origin = Vector(GetLoWorldPos(neela));
            float distance = Square(VuSub(origin, GetLoWorldPos(node)));
            if (nearest < distance)
                continue;
            if (IsPointInVol(volume, GetLoWorldPos(node))) {
                neela->target = node;
                nearest = distance;
            }
        }
        if (!neela->target && !StepguardHasSgf(neela, SGF_IDLE)) {
            qword position = Vector(ConvertPosBetweenLos(neela, 0, D_002C0A80.q));
            qword low = VuAdd(position, D_002C0A90.q);
            qword high = VuAdd(position, D_002C0AA0.q);
            STACKSCOPE arena;
            int count;
            ROW **rows;
            IntersectSwBoundingBox(&g_sw, low, high, FilterCid98, 0, &count, &rows);
            for (int i = 0; i < count; i++) {
                ROW *row = rows[i];
                if (IsPointInVol(volume, GetLoWorldPos(row->object))) {
                    neela->target = row->object;
                    break;
                }
            }
        }
        if (neela->target)
            SetNeelaState(neela, 1);
        break;
    }
    case 1: {
        if (IsAsegaRetracted(neela->propAsega)) {
            SetNeelaState(neela, 0);
            break;
        }
        XF *node = (XF *)neela->propPart;
        qword position = GetLoWorldPos(neela->target);
        qword local = TransformXfPos(0, node->parent, position);
        qword direction = VuSub(local, ((XF *)neela->propPart)->localMatq.r[3].q);
        M3 matrix;
        BuildRotateVectorsMatrix__FUxUx(&matrix, neela->propAxis, direction);
        MOTION *motion = (MOTION *)neela->aimAct;
        motion->angles = DecomposeRotationMatrixToEuler(&matrix);
        break;
    }
    default:
        break;
    }
}

// 0x1BA408: Neela state change: state 1 starts the prop animation, constrains it toward the target, points her
// chase behaviour at it and speeds it up 4x; leaving state 1 restores them.
// Also known as: SetStrikeState (upstream readable / Sly 1 name; SetStrikeState__FP5GUARDi).
extern "C" void SetNeelaState(NEELA *neela, int st)
{
    if (st == neela->state) return;
    if (neela->state == 1) {
        RetractAsega(neela->propAsega);
        ((BASIC *)neela->propAsega)->Release();
        neela->propAsega = 0;
        ReleaseAct(&neela->aimAct);
        neela->aimAct = 0;
        neela->target = 0;
        if (StepguardHasSgf(neela, SGF_IDLE)) ((BHVIDLE *)FindStepBehavior(neela, CID_BHVIDLE))->lookAt = (LO *)D_002E1E40;
        ScaleStepSpinSpeed(neela, 1.0f);
    }
    neela->state = st;
    neela->stateTime = g_clock.time;
    if (st != 1) return;
    ApplyAsegCore(neela->propAnimation, neela, 1, &neela->propAsega, 0.0f, 1.0f);
    ((BASIC *)neela->propAsega)->AddRef();
    neela->aimAct = NewPositionAct(neela, (XF *)neela->propPart, 5, 8, 3, D_002A2DD0);
    if (StepguardHasSgf(neela, SGF_IDLE)) {
        ((BHVIDLE *)FindStepBehavior(neela, CID_BHVIDLE))->lookAt = neela->target;
        ScaleStepSpinSpeed(neela, 4.0f);
    }
}

// 0x1BA560: Neela chase behaviour (BHV174) vtable slot 7 (OnPush): base, state -1, raises guard flag 0x25.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvNeelaChase(BHV174 *behavior)
{
    InitBhv(behavior);
    behavior->state = -1;
    AddStepguardSgf((NEELA *)behavior->owner, SGF_NEELA_CHASE);
}

// 0x1BA598: Neela chase behaviour vtable slot 8 (Start): base, then state 0.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHV174::Start()
{
    BHV174 *behavior = this;
    StartBhv(behavior);
    SetBhvNeelaChaseState(behavior, 0);
}

// 0x1BA5C8: Neela chase behaviour vtable slot 9 (Close): state -1, base, lowers flag 0x25.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvNeelaChase(BHV174 *behavior)
{
    SetBhvNeelaChaseState(behavior, -1);
    CloseBhv(behavior);
    RemoveStepguardSgf((NEELA *)behavior->owner, SGF_NEELA_CHASE);
}

// VU0 helper: distance between two points.
static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    d = VuSub(a, b);
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    f = VuToFloat(d);
    return f;
}
// Evaluates a quadratic c0 + x (c1 + x c2).
static inline float EvaluateQuadratic(CLQ *pp, float x) { return pp->g0 + x * (pp->g1 + x * pp->g2); }
// 0x1BA608: Neela chase behaviour vtable slot 10 (Update): state 0 while the player is inside the outer volume, 1
// once outside, back to 0 inside the inner volume, 2 after a timeout; scales a child parameter by the distance to
// the player.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvNeelaChase(BHV174 *behavior)
{
    int s;
    UpdateBhv(behavior);
    for (;;) {
        if (IsBhvBusyAbove(behavior))
            break;
        s = behavior->state;
        switch (s) {
        case 0: {
            DETECTOR *detector = ((NEELA *)behavior->owner)->pdetector;
            if (!IsPointInVol(detector->pvolFar, GetLoWorldPos(D_002E1E40)))
                SetBhvNeelaChaseState(behavior, 1);
            break;
        }
        case 1: {
            DETECTOR *detector = ((NEELA *)behavior->owner)->pdetector;
            if (IsPointInVol(detector->pvolNear, GetLoWorldPos(D_002E1E40)))
                SetBhvNeelaChaseState(behavior, 0);
            else if (((NEELA *)behavior->owner)->chaseTune.giveUpTime < g_clock.time - behavior->stateTime)
                SetBhvNeelaChaseState(behavior, 2);
            break;
        }
        case 2:
            break;
        }
        if (s == behavior->state)
            break;
    }
    if (behavior->state == 0 && StepguardHasSgf((NEELA *)behavior->owner, SGF_MOVING)) {
        float d = Dist(GetLoWorldPos(behavior->owner), GetLoWorldPos(D_002E1E40));
        NEELA *neela = (NEELA *)behavior->owner;
        float r = Clamp(EvaluateQuadratic(&neela->chaseTune.speedCurve, d), neela->chaseTune.speedRange.minValue, neela->chaseTune.speedRange.maxValue);
        BHVWALKPATH *q1 = (BHVWALKPATH *)FindStepBehavior((NEELA *)behavior->owner, CID_BHVWALKPATH);
        BHVRAIL *q2 = (BHVRAIL *)FindStepBehavior((NEELA *)behavior->owner, CID_BHVRAIL);
        if (q1)
            q1->rate = r;
        else if (q2)
            q2->animSpeed = r;
    }
}

// 0x1BA828: Neela chase state change: 0 resumes the patrol script (BHV169), 1 chases the player (cid 0x9E), 2 plays
// an animation (cid 0x9D) and broadcasts game event 0x33.
// Also known as: SetProxState (upstream readable / Sly 1 name; SetProxState__7JOBPROXi).
extern "C" void SetBhvNeelaChaseState(BHV174 *behavior, int st)
{
    if (st == behavior->state) return;
    if (behavior->state == 0) SaveBhvScriptResumeState(behavior->pbhvNext, &((NEELA *)behavior->owner)->resumeState);
    PopBhv(&behavior->pbhvNext);
    behavior->state = st;
    behavior->stateTime = g_clock.time;
    switch (st) {
    case 0: {
        BHV169 *o = (BHV169 *)PushStepBehavior((NEELA *)behavior->owner, CID_BHV169);
        SetBhvScriptScript(o, behavior->chaseScript, 0, &((NEELA *)behavior->owner)->resumeState);
        o->Start();
        break;
    }
    case 1: {
        BHVIDLE *o = (BHVIDLE *)PushStepBehavior((NEELA *)behavior->owner, CID_BHVIDLE);
        o->lookAt = (LO *)D_002E1E40;
        o->Start();
        break;
    }
    case 2: {
        BHVANIM *o = (BHVANIM *)PushStepBehavior((NEELA *)behavior->owner, CID_BHVANIM);
        o->binding = &((NEELA *)behavior->owner)->giveUpBinding;
        o->Start();
        BroadcastGameEvent(&g_game, 0x33, (NEELA *)behavior->owner);
        break;
    }
    }
}

// 0x1BA980: Sends mode record 0xF with `value` to the guard's root behaviour (BHVGUARD, cid 0x9C).
extern "C" void SendRootBhvMode15(NEELA *neela, int n)
{
    BHVGUARDRECDATA f;
    BHV *po;
    func_002333B4(&neela->resumeState, 0, 0xC);
    neela->chaseScript = (GUARDSCRIPT *)n;
    func_002333B4(&f, 0, sizeof(f));
    f.kind = 0xF;
    po = FindStepBehavior(neela, CID_BHVGUARD);
    if (po) SetBhvguardMode(po, &f);
}
#undef FIELD_OFFSET
