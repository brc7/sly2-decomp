// grab.cpp -- the player's grab state (JTGRAB, stepplayer.h): grab candidates (gather, add, merge into the world's
// four, sparkles, the marker proxy), grab input, the hide state, grab kinds and states, rail landing and the
// per-state contact and acceleration presets. TU 0x191090..0x195950 (28 functions, address order).
// Entry points: GatherJtGrabCandidates (per frame), SetJtGrabState / SetJtGrabKind (state machine),
// UpdateJtGrabInput / UpdateJtActiveHide (per frame), PresetJtAccelHide / UpdateJtInternalXpsHide (physics).
//
// Types: sly2 headers (stepplayer.h JTGRAB/STEPPLAYER/GAIT/JTHANDS, sw.h SW/JTGRABCAND, jt.h JT, lo.h/so.h/xfm.h);
// the file-scope types after the includes are the union of the two Lilly ports' private declarations
// (AddJtGrabCandidate, GatherJtGrabCandidates: identical copies merged; GRABSPOT takes the port with all four
// virtuals, the other declared only the first). Functions on named header fields are written against them; the
// others keep the per-function views they were matched with, each in a namespace named after it (as vec.cpp, mark.cpp
// and hurt.cpp do): the hand-tuned ones and the ones that read bytes the headers leave unnamed (JT +0x1854, the TN
// fields of ResetJtGrabTnParams...). SetJtHfdk keeps its Sly 1 symbol (JT *) as a C-linkage definition on the JTGRAB.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_grab
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_grab
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/act.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/cm.h"
#include "sly2/emitter.h"
#include "player_input.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#include "sly2/classes.h"

// The embedded movement components of a player (JTAIR at +0x2C0, JTGRAB +0x4C0, ... JTTHROW +0x11B0) are taken from
// the real STEPPLAYER layout in sly2/stepplayer.h; the per-function views only declare the words they read.
static inline STEPPLAYER *AsStepplayer(void *player) { return (STEPPLAYER *)player; }
// The real classes behind the per-function views below, for their virtual calls.
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }
static inline ::BASIC *AsBasic(void *pbasic) { return (::BASIC *)pbasic; }

// 0x1957F8 StaticInit_grab and GlobalConstructors_grab: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef unsigned size_t;

extern "C" {
void InitGait(GAIT *pgait, STEPPLAYER *pstepplayer, int n);
void ResetJtHands(JTHANDS *phands);
}
LO *FindSwObject(SW *psw, int grf, OID oid, LO *ploCtx);

extern "C" {
extern qword D_002A2DD0;    /* zero vector */
extern char D_002A2D00[];   /* identity matrix */
LO *AcquireProxyObject(LO *plo, int n);
void SetLoParent(LO *plo, int n1, int n2);
void FadeSoIn(LO *plo, float dt);
LO *AcquireJtGrabMarker(SW *psw, LO *plo, JTGRABCAND *pcand);
LO *CreateLo__F3CIDP2SO3OIDi(CID cid, SO *psoParent, OID oid, int n);
void SetTnTnsOverride(TN *ptn, int n);
void ResetJtGrabTnParams(JTGRAB *pjtgrab);
void InitDirectionalBlend(STEPPLAYER *pstepplayer, BLDIR *abldir, ASEGBL **ppasegbl);
void PostJtLoadGait(GAIT *pgait);
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(STEPPLAYER *pstepplayer, int n1, int n2, void *abl, int n4, int n5, int n6, ASEGBL **ppasegbl);
void InitJtHands__FP7JTHANDSP2SOPP5ASEGA(JTHANDS *phands, STEPPLAYER *pstepplayer, ASEGA **ppasega);
}




struct BND
{
      OID oid;
      LO *plo;
      int ixf;
      XF *pxf;
      MATRIX4 matInv;
};






void *PvAllocSwClearImpl(SWALLOC *pswalloc);




struct REGNODE
{
      VECTOR4 *pplane;
      unsigned short aiChild[2];
};

struct REGTREE
{
      int cnode;
      REGNODE *anode;
      int cplane;
      VECTOR4 *aplane;
      int ccid17;
      OID *aoid;
      CID17 **apcid17;
      REGLINKS *alinks;
      int ccid124;
      CID124 **apcid124;
      int nVersion;
      int fReady;
};



/* SVN: sly2/sw.h */


/* LOUPD: sly2/sw.h */











// Initializes the player's grab/action context (STEPPLAYER+0x4C0): owner, state -1, kind -1, and the two directional
// blend blocks.
// Also known as: InitInteract (upstream readable / Sly 1 name; InitInteract__FP5ACTORi).
extern "C" void InitJtGrab(JTGRAB *grab, STEPPLAYER *player)
{
    grab->player = player;
    grab->state = -1;
    grab->kind = -1;
    grab->gadgetId = -1;
    InitGait(&grab->gaitA, player, 0x6F);
    InitGait(&grab->gaitB, player, 0x6F);
    ResetJtHands(&grab->hands);
}

/* Creates the grab's TN (CID 0xF, oid 0x148) and loads the grab animations: both direction blends, both gaits, the
   lean blend (ablC) and the hands on the player's current animation. */
// Post-load: creates the context's private camera TN object (oid 0x148) and resets its parameters; builds the
// directional blends (InitDirectionalBlend).
// Also known as: PostLoadInteract (upstream readable / Sly 1 name; PostLoadInteract__FP5ACTOR).
extern "C" void PostLoadJtGrab(JTGRAB *grab)
{
    grab->cameraTunnel = (TN *)CreateLo__F3CIDP2SO3OIDi((CID)0xF, 0, (OID)0x148, -1);
    SetTnTnsOverride(grab->cameraTunnel, 0);
    ResetJtGrabTnParams(grab);
    InitDirectionalBlend(grab->player, grab->abldirA, &grab->pasegblA);
    InitDirectionalBlend(grab->player, grab->abldirB, &grab->pasegblB);
    PostJtLoadGait(&grab->gaitA);
    PostJtLoadGait(&grab->gaitB);
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(grab->player, sizeof(grab->ablC[0]), 3, grab->ablC, 0, 0, 0, &grab->pasegblC);
    InitJtHands__FP7JTHANDSP2SOPP5ASEGA(&grab->hands, grab->player, &grab->player->activeAnimation);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateJtGrabInput {
// UpdateJtGrabInput: per-frame update of a hero's action context (the struct at actor+0x4C0; UpdateBentleyStateMachine calls it in
// its state 3). It dispatches on the context's action and loops while the action changed the step (and not to -1).
// Action 8 / step "other" probes the ground below the actor: a box query along the actor's up axis, a line test
// against every found object (any hit with a nonzero parameter aborts), then a sphere around the actor's world bounds
// is pruned against every object's BSP and turned into contacts; any contact aborts, otherwise step 8 is entered.
//
// Matching notes (wave-4 lane 8):
// - case 8 switches on p->step (the field, not the loop's local): gcse turns the load into a copy of the loop-top
//   load, which gives the `lw a1, step` in the dispatch branch's delay slot; the local itself is spilled.
// - ProbeGround's exits are plain returns out of a STACKSCOPE scope (out-of-line ctor/dtor 0x1AE478/0x1AE4A0);
//   loop.c moves the `{ FreeSwXpList(); return; }` block out of the contact loop into the default case.
// - case 2 has its own SetControl for the `<= 0` test: the shared SetControl then has no label in front of it, so
//   jump2's cross-jumping makes a new label there and does not merge the step-8 / case-11 anim tests afterwards.
// - LengthOf takes a const reference (gcse hash table size, i.e. the order of the &cso / &apso copies: s4 / s6).
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef unsigned long ulong;

struct JOY { char pad0[0x1C]; float amount; char pad20[0x10]; uint buttonsHeld; };
struct SURF;
struct BSP { BSP *pbspNeg, *pbspPos; SURF *psurf; };
struct XP;
struct ASEGA;
struct GEOM { int numPositions; qword *positions; };
struct SO {
    char pad0[0xC]; void *vptr; ulong flags; char pad18[0x6C - 0x18]; int cbsp; char pad70[4]; BSP *absp;
    char pad78[0x90 - 0x78]; GEOM geomWorld;
};
// Line segment hit (0x60 bytes, as in IntersectSphereBsp)
struct LSG { qword apos[2]; qword anormal[2]; float au[2]; int lsgk; void *apsurf[2]; void *pedge; int iiposSwap; LSG *plsgNext; };
struct ACTOR {
    void *scriptFrame; void *scriptVariables; void *firstSubscriber; VTE *vt; char pad10[0xEC - 0x10]; void *pvFilter; char padF0[0x150 - 0xF0]; JOY *pjoy;
    char pad154[0x240 - 0x154]; qword posOffset; char pad250[0x268 - 0x250]; SO *pso; char pad26C[0xF5C - 0x26C];
    ACTOR *ptarget; float gTarget; char padF64[0x1030 - 0xF64]; ASEGA *pasega; char pad1034[0x10EC - 0x1034]; int interactMode;
};
struct ATTACH { char pad0[0x34]; ACTOR *pactor; };
struct CONTEXT {
    ACTOR *pactor; float tStamp; int action; int step; float tHold; ACTOR *pactorFocus; char pad18[8]; float gParam;
    char pad24[0x440 - 0x24]; ATTACH attach; char pad478[0x500 - 0x478]; int fInhibit;
};
struct VU_FLOAT { qword q; };


extern uint g_grfusr __asm__("g_grfusr");
extern qword g_normalZ __asm__("g_normalZ");                                // (0, 0, 1)
extern float D_002BF5B0 __asm__("D_002BF5B0");
extern float D_002BF5B4 __asm__("D_002BF5B4");
extern float D_002BF5B8 __asm__("D_002BF5B8");
int ConsumeButtonsPressedWithin__5POCTLUif(JOY *, uint, float) __asm__("ConsumeButtonsPressedWithin__5POCTLUif");
int GetButtonsPressedWithin__5POCTLUif(JOY *, uint, float) __asm__("GetButtonsPressedWithin__5POCTLUif");
void ClearPoctlJoyPressed(JOY *, uint) __asm__("ClearPoctlJoyPressed");
void SetJtAirJump(void *, int, int, ACTOR *) __asm__("SetJtAirJump");
void StartJtMode(void *, int, int, int) __asm__("StartJtMode");
int IsAsegaRetracted(ASEGA *) __asm__("IsAsegaRetracted");
void SetJtGrabKind(CONTEXT *, int) __asm__("SetJtGrabKind");                     // set the step
void *AllocFromStack(SWALLOC *) __asm__("AllocFromStack");                         // stack allocation in the current STACKSCOPE
qword GetLoWorldPos(ACTOR *) __asm__("GetLoWorldPos");                           // world position
int ShouldKeepSoPair(void *, SO *) __asm__("ShouldKeepSoPair");
void IntersectSwBoundingBox(SW *, qword, qword, int (*)(void *, SO *), void *, int *, SO ***) __asm__("IntersectSwBoundingBox");   // objects in a box
int ClipEdgeToObject(SO *, qword, qword, int, LSG *) __asm__("ClipEdgeToObject");      // line test against an object
void UpdateSoPartGeom(SO *) __asm__("UpdateSoPartGeom");
qword VecMinArray(int, qword *) __asm__("VecMinArray");                      // min of points
qword VecMaxArray(int, qword *) __asm__("VecMaxArray");                      // max of points
void MarkSoContactsSphereBsp(SW *, SO *, SO *, int, BSP *, XP **) __asm__("MarkSoContactsSphereBsp");
int PruneBspBySphere(BSP *, BSP *, float, qword, int *, BSP **) __asm__("PruneBspBySphere");   // prune a BSP to a sphere
void MarkSoContactsBspBsp(SW *, SO *, SO *, int, BSP *, BSP *, XP **) __asm__("MarkSoContactsBspBsp");
void AttachJtHandsObject(ATTACH *, ACTOR *) __asm__("AttachJtHandsObject");
int IsBasicDerivedFrom__FP5BASIC3CID(ACTOR *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
int IsStepplayerGrabReady(ACTOR *) __asm__("IsStepplayerGrabReady");
int GetStepplayerMp(ACTOR *) __asm__("GetStepplayerMp");
uint GetGadgetButtonMask(ACTOR *, int) __asm__("GetGadgetButtonMask");

void FreeSwXpList(SW *, XP *) __asm__("FreeSwXpList__FP2SWP2XP");

// Notify the actor of q (virtual slot 51).
static inline void Notify(ACTOR *p, ACTOR *q) { VTE &v = p->vt[51]; ((void (*)(void *, ACTOR *))v.pfn)((char *)p + v.d, q); }
// Allocate cb bytes, 16-byte aligned, from the frame stack.
static inline void *AllocStack(int cb) { SWALLOC swalloc; swalloc.cb = cb; swalloc.cbAlign = 0x10; return AllocFromStack(&swalloc); }

// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j"(v.q) : "r"(g)); return v; }
// The never-chosen "V" alternative is allocation-only: the word stays a MEM until reload (as in ClipCameraEye).
static inline VU_FLOAT VuFloat(const float *pg) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j,j"(v.q) : "r,V"(*(const int *)pg)); return v; }
// Return a scaled by g.
static inline qword ScaleVec(qword a, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(a), "j"(g.q)); return r; }
// a * g0 + b * g1
static inline qword CombineVec(qword a, qword b, VU_FLOAT g0, VU_FLOAT g1)
{
    qword r;
    __asm__("vmulax.xyzw $ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x" : "=j"(r) : "j"(a), "j"(g0.q), "j"(b), "j"(g1.q));
    return r;
}
// Length of the xyz part of v as a float.
static inline float LengthOf(const qword &v)
{
    qword qw, one;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(qw), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(qw) : "0"(qw));
    return VuToFloat(qw);
}
// True when a and b are closer than s.
static inline int IsWithinDistance(qword a, qword b, float s) { return LengthOf(VuSub(a, b)) < s; }

// Probe below the actor: clip a downward segment from its base against the nearby objects and record the ground hits.
static inline void ProbeGround(CONTEXT *p)
{
    STACKSCOPE scope;
    LSG alsg[8];
    int i;
    qword pos = GetLoWorldPos(p->pactor);
    qword posBase = VuAdd(pos, p->pactor->posOffset);
    qword posEnd = VuAdd(posBase, ScaleVec(g_normalZ, VuFloat(&D_002BF5B0)));
    int cso;
    SO **apso;
    IntersectSwBoundingBox(&g_sw, posEnd, posBase, ShouldKeepSoPair, p->pactor->pvFilter, &cso, &apso);
    for (i = 0; i < cso; i++) {
        int clsg = ClipEdgeToObject(apso[i], posBase, posEnd, 8, alsg);
        LSG *plsg = alsg;
        for (int j = 0; j < clsg; j++)
            if (plsg[j].au[0] != 0.0f)
                return;
    }
    SO *psoSelf = p->pactor->pso;
    GEOM *pgeom = &psoSelf->geomWorld;
    int cbspMax = 0;
    UpdateSoPartGeom(psoSelf);
    qword posMin = VecMinArray(pgeom->numPositions, pgeom->positions);
    qword posMax = VecMaxArray(pgeom->numPositions, pgeom->positions);
    IntersectSwBoundingBox(&g_sw, posMin, posMax, ShouldKeepSoPair, p->pactor->pvFilter, &cso, &apso);
    for (i = 0; i < cso; i++)
        if (cbspMax <= apso[i]->cbsp)
            cbspMax = apso[i]->cbsp;
    cbspMax = MinInt(cbspMax, 0x1555);
    BSP *absp = (BSP *)AllocStack(cbspMax * sizeof(BSP));
    qword posMid = CombineVec(posMin, posMax, VuFloat(0.5f), VuFloat(0.5f));
    float sRadius = LengthOf(VuSub(posMid, posMin));
    for (i = 0; i < cso; i++) {
        SO *so = apso[i];
        XP *xp = 0;
        if (so->flags & (1UL << 49)) {
            MarkSoContactsSphereBsp(&g_sw, so, psoSelf, 0, 0, &xp);
        } else {
            int cbspLeft = cbspMax;
            BSP *pbsp = absp;
            if (PruneBspBySphere(so->absp, (BSP *)0x7FFFFFFF, sRadius, posMid, &cbspLeft, &pbsp) == 2) {
                int cbsp = pbsp - absp;
                pbsp--;
                MarkSoContactsBspBsp(&g_sw, psoSelf, so, cbsp, so->absp, pbsp, &xp);
            }
        }
        if (xp) {
            FreeSwXpList(&g_sw, xp);
            return;
        }
    }
    SetJtGrabKind(p, 8);
}

// Per-frame input for the current grab action: X jumps off (SetJtAirJump kinds 7/1), the action buttons per state,
// gadgets.
// Also known as: UpdateInteractTransitions (upstream readable / Sly 1 name; UpdateInteractTransitions__FP5ACTOR).
extern "C" void UpdateJtGrabInput(CONTEXT *grab)
{
    JOY *joy = grab->pactor->pjoy;
    int step;
    do {
        step = grab->step;
        switch (grab->action) {
        case 1:
            if (0.15f < g_clock.time - grab->tHold && ConsumeButtonsPressedWithin__5POCTLUif(joy, BUTTON_JUMP, 0.25f)) {
                SetJtAirJump(&AsStepplayer(grab->pactor)->jtair, 7, 0, grab->pactorFocus);
                break;
            }
            if (GetButtonsPressedWithin__5POCTLUif(joy, BUTTON_LOOK, 0.15f) && AsStepplayer(grab->pactor)->CanUseGadget(3)) {
                ClearPoctlJoyPressed(joy, BUTTON_LOOK);
                StartJtMode(&AsStepplayer(grab->pactor)->jtmode, 0, 0, 0);
            }
            break;
        case 3:
        case 4:
            if (ConsumeButtonsPressedWithin__5POCTLUif(joy, BUTTON_JUMP, 0.1f)) {
                SetJtAirJump(&AsStepplayer(grab->pactor)->jtair, 1, 0, 0);
                break;
            }
            if (GetButtonsPressedWithin__5POCTLUif(joy, BUTTON_LOOK, 0.15f) && AsStepplayer(grab->pactor)->CanUseGadget(3)) {
                ClearPoctlJoyPressed(joy, BUTTON_LOOK);
                StartJtMode(&AsStepplayer(grab->pactor)->jtmode, 0, 0, 0);
            }
            break;
        case 7: {
            ACTOR *pactorFocus = grab->pactorFocus;
            AsStepplayer(grab->pactor)->SetPlayerState(0);
            Notify(pactorFocus, grab->pactor);
            break;
        }
        case 8:
            switch (grab->step) {
            case 7:
                if (IsAsegaRetracted(grab->pactor->pasega))
                    SetJtGrabKind(grab, 9);
                break;
            case 8:
                if (IsAsegaRetracted(grab->pactor->pasega))
                    AsStepplayer(grab->pactor)->SetPlayerState(0);
                break;
            default:
                ProbeGround(grab);
                break;
            }
            break;
        case 9:
            if (IsWithinDistance(GetLoWorldPos(grab->pactorFocus), GetLoWorldPos(grab->attach.pactor), D_002BF5B4)) {
                AttachJtHandsObject(&grab->attach, grab->pactorFocus);
                AsStepplayer(grab->pactor)->SetPlayerState(0);
            }
            break;
        case 10:
            if (!grab->pactor->pasega || IsAsegaRetracted(grab->pactor->pasega)) {
                AsStepplayer(grab->pactor)->SetPlayerState(0);
            } else if (D_002BF5B8 < g_clock.time - grab->tStamp) {
                if (0.0f < grab->pactor->pjoy->amount || (grab->pactor->pjoy->buttonsHeld & 0xD0)) {
                    AsStepplayer(grab->pactor)->SetPlayerState(0);
                    return;
                }
            }
            break;
        case 11:
        case 12:
            if (!grab->pactor->pasega || IsAsegaRetracted(grab->pactor->pasega))
                AsStepplayer(grab->pactor)->SetPlayerState(0);
            break;
        case 5:
            if (IsBasicDerivedFrom__FP5BASIC3CID(grab->pactorFocus, 11) && IsStepplayerGrabReady(grab->pactor)) {
                grab->pactor->ptarget = grab->pactorFocus;
                grab->pactor->gTarget = grab->gParam;
                AsStepplayer(grab->pactor)->SetPlayerState(8);
            }
            break;
        case 2:
            if (GetStepplayerMp(grab->pactor) <= 0) {
                AsStepplayer(grab->pactor)->SetPlayerState(0);
                break;
            }
            if (grab->pactor->interactMode == 4) {
                AsStepplayer(grab->pactor)->SetPlayerState(0);
                break;
            }
            if (g_grfusr & 1)
                break;
            if (joy->buttonsHeld & GetGadgetButtonMask(grab->pactor, 0x28))
                break;
            AsStepplayer(grab->pactor)->SetPlayerState(0);
            break;
        default:
            if (ConsumeButtonsPressedWithin__5POCTLUif(joy, BUTTON_JUMP, 0.1f)) {
                SetJtAirJump(&AsStepplayer(grab->pactor)->jtair, 1, 0, 0);
                break;
            }
            if (GetButtonsPressedWithin__5POCTLUif(joy, BUTTON_LOOK, 0.15f) && AsStepplayer(grab->pactor)->CanUseGadget(3) && (grab->action == 0 || grab->action == 13)) {
                ClearPoctlJoyPressed(joy, BUTTON_LOOK);
                StartJtMode(&AsStepplayer(grab->pactor)->jtmode, 0, 0, 0);
                break;
            }
            if (grab->fInhibit)
                break;
            if (g_grfusr & 1)
                break;
            if (joy->buttonsHeld & 0x20)
                break;
            AsStepplayer(grab->pactor)->SetPlayerState(0);
            break;
        }
    } while (grab->step != step && grab->step != -1);
}

#define FIELD_OFFSET(t, m) ((unsigned int)&(((t *)0x10000)->m) - 0x10000)
typedef char LSG_Size[sizeof(LSG) == 0x60 ? 1 : -1];
typedef char BSP_Size[sizeof(BSP) == 12 ? 1 : -1];
typedef char SO_absp_Offset[FIELD_OFFSET(SO, absp) == 0x74 ? 1 : -1];
typedef char SO_geomWorld_Offset[FIELD_OFFSET(SO, geomWorld) == 0x90 ? 1 : -1];
typedef char ACTOR_pso_Offset[FIELD_OFFSET(ACTOR, pso) == 0x268 ? 1 : -1];
typedef char ACTOR_x10EC_Offset[FIELD_OFFSET(ACTOR, interactMode) == 0x10EC ? 1 : -1];
typedef char CONTEXT_attach_Offset[FIELD_OFFSET(CONTEXT, attach) == 0x440 ? 1 : -1];
typedef char CONTEXT_fInhibit_Offset[FIELD_OFFSET(CONTEXT, fInhibit) == 0x500 ? 1 : -1];
} // namespace M_UpdateJtGrabInput

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateJtActiveHide {
// UpdateJtActiveHide: per-frame update of a hero's movement/action context (the struct at actor+0x4C0; called from the
// actor's mode handler, see UpdateJtPre / UpdateBentley). Dispatches on the context's action, then commits the
// animation / axis weights for the resulting step.
//
// Matched from w5lane7's near-miss (3/953). Notes on the forms that matter (each verified against the target):
// - LengthOf takes a VU_VECTOR by value: the inlined parameter copy is a store to an ADDRESSOF slot during cse1
//   (it may alias ctx->actor), and a plain register copy after purge_addressof, so only cse2 sees the case-0 lean
//   test's ctx->actor reload as the earlier one. That leaves the target's `daddu v0,v1` copy in the vy test.
// - DivVec: vdiv as its own volatile no-output asm, so jump.c cannot hoist the Normalize fallback copy.
// - LaneX: one mtc1 asm from a TI qword (gcc emits the qmfc2 itself and schedules it).
// - SqrtF: the usual volatile "=&f" sqrt.s helper (local-alloc must not tie it to its input).
// - case 2 falls through into case 4; slow-path step choice is a ternary (blocks thread_jumps).
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;

union U { float f[4]; qword q; int i[4]; };   // floats first, so U globals can be initialized
struct M3 { U r[3]; };
struct JOY { char pad0[0x10]; float x, y, heading, amount; };
/* the channel FindAsegChannel returns is aseg.h's CHN; its curve act.h's AC */

struct SMP { float fastSpeed, slowSpeed, fastDuration; };
struct HEIGHT { float min, max; };
struct ENTRY { float weight; int active; char pad8[8]; U direction; };
struct PROGRAM { void *owner; void *clip; char pad8[0x18]; float x20; char pad24[0x3C]; float x60; char pad64[0x40]; };
struct FOCUS { char pad0[0xE4]; int cyclic; char padE8[0x18C - 0xE8]; int enabled; char pad190[0x1BC - 0x190]; uint flags; char pad1C0[8]; FOCUS *other; };
typedef JT ACTOR;   /* Sly (the JT); was a whole-object view */
struct CONTEXT {
    ACTOR *actor; float stamp; int action; int step; float tHold; FOCUS *focus; void *setting; float parameter;
    float value; int reversed; int index; int x2C; char pad30[0x50 - 0x30]; float maximum; char pad54[0x68 - 0x54];
    void *x68; char pad6C[4]; ENTRY axes[8]; void *request; char pad174[0x180 - 0x174]; PROGRAM program180;
    char pad224[0x240 - 0x224]; PROGRAM program240; char pad2E4[0x330 - 0x2E4]; ENTRY axes2[8]; void *request2;
    char pad434[0x440 - 0x434]; char sub[0x10];
};
struct VU_FLOAT { qword q; };


extern ACTOR *D_002E1E40 __asm__("D_002E1E40");
extern qword D_002A2DA0 __asm__("D_002A2DA0");                    // (1, 0, 0, 0)
extern float D_002BF5BC __asm__("D_002BF5BC");
extern float D_002BF5C0 __asm__("D_002BF5C0");
extern float D_002BF5C4 __asm__("D_002BF5C4");
extern float D_002BF5C8 __asm__("D_002BF5C8");
extern float D_002BF5CC __asm__("D_002BF5CC");
extern U D_002BF5D0[3] __asm__("D_002BF5D0");   // probe points (hang)
extern U D_002BF600[3] __asm__("D_002BF600");   // probe points (low)
extern HEIGHT D_002BF630 __asm__("D_002BF630");
qword GetLoWorldPos(ACTOR *) __asm__("GetLoWorldPos");
void GetGrabspotPositionNear(void *, ACTOR *, float *, U *, float *) __asm__("GetGrabspotPositionNear");
qword PosSmooth(qword, qword, float, SMP *, U *) __asm__("PosSmooth");
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword SetVectorCylind(float, float, float) __asm__("SetVectorCylind");
float GetShapeMinDistance(FOCUS *) __asm__("GetShapeMinDistance");
float GetShapeMaxDistance(FOCUS *) __asm__("GetShapeMaxDistance");
float GetShapeMinParam(FOCUS *) __asm__("GetShapeMinParam");
float GetShapeMaxParam(FOCUS *) __asm__("GetShapeMaxParam");
float SmoothValue__FfffP3SMPPf(float, float, float, SMP *, float *) __asm__("SmoothValue__FfffP3SMPPf");
void SetJtHideFacingKind__FP2JT4HFDK(CONTEXT *, int) __asm__("SetJtHideFacingKind__FP2JT4HFDK");
void SetStepTargetHeading(ACTOR *, float) __asm__("SetStepTargetHeading");
float GetStepHeading(ACTOR *) __asm__("GetStepHeading");
float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");
void SetJtGrabKind(CONTEXT *, int) __asm__("SetJtGrabKind");
void UpdateGaitBlend(PROGRAM *, float) __asm__("UpdateGaitBlend");
void UpdateGaitFootsteps(PROGRAM *) __asm__("UpdateGaitFootsteps");
void DrainStepplayerMp(ACTOR *, float) __asm__("DrainStepplayerMp");
void FindShapeClosestPointFromParam(FOCUS *, qword, void *, U *, U *, float *, float *, float) __asm__("FindShapeClosestPointFromParam");
void SetJtAirJump(void *, int, int, int) __asm__("SetJtAirJump");
float atan2f(float, float) __asm__("atan2f");
void FindShapeClosestPoint(FOCUS *, qword, void *, void *, U *, void *, void *) __asm__("FindShapeClosestPoint");
void UpdateJtHands(void *) __asm__("UpdateJtHands");
void UpdateDirectionalBlend(ACTOR *, ENTRY *, void *) __asm__("UpdateDirectionalBlend");
void FitStepToTerrain(ACTOR *, HEIGHT *, int, U *) __asm__("FitStepToTerrain");
CHN *FindAsegChannel(void *, ACTOR *, int, int) __asm__("FindAsegChannel");
void FaceStepToward(ACTOR *, FOCUS *) __asm__("FaceStepToward");
void FixStepAngularVelocity(ACTOR *) __asm__("FixStepAngularVelocity");
void RebuildJtXmg(ACTOR *, void *, void *, void *, float, void *) __asm__("RebuildJtXmg");
void ClampJtGrabCurveParam(ACTOR *, void *, void *) __asm__("ClampJtGrabCurveParam");


// Evaluate the channel's curve (AC slot 3) at the animation's time and speed into *out.
static inline void ReadBinding(CHN *b, ASEGA *a, U *out)
{
    b->pac->Evaluate(a->time, a->speed, 0, out);

}

// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j"(v.q) : "r"(g)); return v; }
// Return a scaled by g.
static inline qword ScaleVec(qword a, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(a), "j"(g.q)); return r; }
// Return a scaled by g.x.
static inline qword ScaleVecX(qword a, qword g) { qword r; __asm__("vmulx.xyzw %0,%2,%1x" : "=j"(r) : "j"(g), "j"(a)); return r; }
// Dot product of the xyz parts of a and b (x lane).
static inline qword DotVec(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    return d;
}
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
// Length of v as a float.
static inline float LengthOf(VU_VECTOR v) { return QwordX(VuSqrtX(DotVec(v.q, v.q))); }
// XY length of v as a float.
static inline float LengthXYOf(qword v)
{
    qword l;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(l) : "j"(v));
    return QwordX(VuSqrtX(l));
}
// Divide v by the x lane of l.
static inline qword DivVec(qword v, qword l)
{
    qword r;
    __asm__ __volatile__("vdiv $Q,$vf0w,%0x\n\tvwaitq" : : "j"(l));
    __asm__("vmulq.xyzw %0,%1,$Q" : "=j"(r) : "j"(v));
    return r;
}
// Unit vector along v, or fallback when its length is too small.
static inline qword NormalizeVec(qword v, qword fallback)
{
    qword l = VuSqrtX(DotVec(v, v));
    if (QwordX(l) < 0.0001f)
        return fallback;
    return DivVec(v, l);
}
// M * v (rows of M dotted with v); w comes from row 0
static inline qword MulM3Vec(M3 *pm, qword v)
{
    qword r = pm->r[0].q;
    qword t0, t1, t2, one;
    __asm__("vmul.xyzw %2,%0,%5\n\tvmaxw.xyzw %1,$vf0,$vf0w\n\tvmul.xyzw %3,%6,%5\n\tvmul.xyzw %4,%7,%5\n\tvadday.x $ACC,%2,%2y\n\tvmaddz.x %0,%1,%2z\n\tvaddax.y $ACC,%3,%3x\n\tvmaddz.y %0,%1,%3z\n\tvaddax.z $ACC,%4,%4x\n\tvmaddy.z %0,%1,%4y"
            : "+j"(r), "=&j"(one), "=&j"(t0), "=&j"(t1), "=&j"(t2) : "j"(v), "j"(pm->r[1].q), "j"(pm->r[2].q));
    return r;
}
// Set the xyz floats of p.
static inline void SetXYZ(U *p, float x, float y, float z) { p->f[0] = x; p->f[1] = y; p->f[2] = z; }

// Per-frame update of the grab context: dispatches on the grab state (hang, shimmy, climb, hide, press ...), chooses
// the kind and drives SetJtGrabKind. Sly 1 name (same SetJtHfdk calls).
// Also known as: UpdateInteract (upstream readable / Sly 1 name; UpdateInteract__FP5ACTOR).
extern "C" void UpdateJtActiveHide(CONTEXT *ctx)
{
    U pos;
    U velOld;
    pos.q = GetLoWorldPos(ctx->actor);
    velOld.q = AsStepplayer(ctx->actor)->GetVelocity();
    JOY *joy = ((JOY *)ctx->actor->controller);

    switch (ctx->action) {
    case 0:
    case 13: {
        U goal;
        float heading;
        GetGrabspotPositionNear(ctx->setting, ctx->actor, &ctx->value, &goal, &heading);
        float speedMax;
        if (ctx->actor->unknown1040 == 1)
            speedMax = LengthOf(ctx->axes[2].direction.q) * ctx->actor->activeAnimation->speed;
        else
            speedMax = LengthOf(ctx->axes[4].direction.q) * ctx->actor->activeAnimation->speed;
        goal.f[2] = pos.f[2];
        SMP smp;
        smp.fastSpeed = speedMax;
        smp.slowSpeed = 0.0f;
        smp.fastDuration = 0.3f;
        U vel;
        PosSmooth(pos.q, goal.q, g_clock.deltaTime, &smp, &vel);
        float lateral;
        qword offset;
        if (IsBasicDerivedFrom__FP5BASIC3CID(ctx->focus, 38)) {
            offset = VuZero<qword>();
            lateral = 0.0f;
        } else {
            U side;
            float angle;
            if (ctx->actor->unknown1040 == 1)
                angle = heading - 1.5707964f;
            else
                angle = heading + 1.5707964f;
            side.q = SetVectorCylind(angle, 1.0f, 0.0f);
            U input;
            input.q = SetVectorCylind(joy->heading, 1.0f, 0.0f);
            float g = Clamp(D_002BF5BC * QwordX(DotVec(side.q, input.q)), -1.0f, 1.0f);
            g *= joy->amount;
            if (__builtin_fabsf(g) < 0.2f)
                g = 0.0f;
            lateral = g * speedMax;
            lateral = StdMin(SqrtFloat(speedMax * speedMax - QwordX(DotVec(vel.q, vel.q))), lateral);
            float s;
            FOCUS *focus;
            if (IsBasicDerivedFrom__FP5BASIC3CID(ctx->focus, 12) && !(focus = ctx->focus)->cyclic) {
                float lo, hi;
                SmoothValue__FfffP3SMPPf(ctx->value, GetShapeMinDistance(focus) + 30.0f, g_clock.deltaTime, &smp, &lo);
                SmoothValue__FfffP3SMPPf(ctx->value, GetShapeMaxDistance(focus) - 30.0f, g_clock.deltaTime, &smp, &hi);
                s = Clamp(lateral, lo, hi);
            } else {
                s = lateral;
            }
            offset = ScaleVec(side.q, VuFloat(s));
        }
        ACTOR *actor = ctx->actor;
        actor->qwUnknown190 = ScaleVec(MulM3Vec((M3 *)&actor->matqTarget, VuAdd(vel.q, offset)), VuFloat(-1.0f));
        int step;
        if (25.0f < LengthOf(ctx->actor->qwUnknown190)) {
            step = 1;
            if (ctx->action == 0) {
                if (45.0f < LengthOf(vel.q)) {
                    SetJtHideFacingKind__FP2JT4HFDK(ctx, 0);
                } else if (20.0f < ((U *)&ctx->actor->qwUnknown190)->f[1]) {
                    if (ctx->actor->unknown1040 == 1)
                        SetJtHideFacingKind__FP2JT4HFDK(ctx, 1);
                    else
                        SetJtHideFacingKind__FP2JT4HFDK(ctx, 2);
                } else if (((U *)&ctx->actor->qwUnknown190)->f[1] < -20.0f) {
                    if (ctx->actor->unknown1040 == 1)
                        SetJtHideFacingKind__FP2JT4HFDK(ctx, 2);
                    else
                        SetJtHideFacingKind__FP2JT4HFDK(ctx, 1);
                }
            }
        } else {
            step = 0;
            if (IsBasicDerivedFrom__FP5BASIC3CID(ctx->focus, 12)) {
                FOCUS *focus = ctx->focus;
                if (lateral < -25.0f && (focus->flags & 1)) {
                    step = ctx->action == 0 && ctx->actor->unknown1040 == 0 ? 3 : 2;
                } else if (25.0f < lateral && (focus->flags & 2)) {
                    step = ctx->action == 0 && ctx->actor->unknown1040 == 0 ? 2 : 3;
                }
            }
            if (ctx->action == 0)
                SetJtHideFacingKind__FP2JT4HFDK(ctx, 3);
        }
        SetStepTargetHeading(ctx->actor, heading);
        if (ctx->step == 5 && 0.25f < __builtin_fabsf(NormalizeAngle__Ff(GetStepHeading(ctx->actor) - heading)))
            step = 5;
        SetJtGrabKind(ctx, step);
        break;
    }
    case 1:
        if (ctx->focus->enabled && 0.0f < joy->amount && ctx->program180.clip) {
            SetJtGrabKind(ctx, 4);
            UpdateGaitBlend(&ctx->program180, joy->amount);
            SetStepTargetHeading(ctx->actor, ((JOY *)ctx->actor->controller)->heading);
            UpdateGaitFootsteps(&ctx->program180);
        } else {
            SetJtGrabKind(ctx, 0);
        }
        break;
    case 2:
        DrainStepplayerMp(ctx->actor, D_002BF5C0);
        if (ctx->step == 0 ? 0.1f < joy->amount : 0.0f < joy->amount) {
            SetJtGrabKind(ctx, 4);
            UpdateGaitBlend((PROGRAM *)((char *)ctx->actor + 0x1C30), joy->amount * D_002BF5C4);
        } else {
            SetJtGrabKind(ctx, 0);
            break;
        }
        // fall through: face the stick
    case 4:
        SetStepTargetHeading(ctx->actor, joy->heading);
        break;
    case 3: {
        FOCUS *focus = ctx->focus;
        U point, tangent;
        FindShapeClosestPointFromParam(focus, *ctx->actor->unknown218, 0, &point, &tangent, &ctx->parameter, &ctx->value, ctx->parameter);
        if (focus->flags && !focus->cyclic &&
            (ctx->parameter < GetShapeMinParam(focus) + 0.01f || GetShapeMaxParam(focus) - 0.01f < ctx->parameter)) {
            SetJtAirJump(&ctx->actor->jtair, 0, 0, 0);
            break;
        }
        tangent.f[2] = 0.0f;
        tangent.q = NormalizeVec(tangent.q, D_002A2DA0);
        float heading = atan2f(tangent.f[1], tangent.f[0]);
        U dir;
        U input;
        input.q = SetVectorCylind(joy->heading, 1.0f, 0.0f);
        if (!focus->other)
            dir = tangent;
        else
            FindShapeClosestPoint(focus->other, *ctx->actor->unknown218, 0, 0, &dir, 0, 0);
        dir.f[2] = 0.0f;
        dir.q = NormalizeVec(dir.q, D_002A2DA0);
        float g = QwordX(DotVec(input.q, dir.q));
        switch (ctx->index) {
        case 0:
            if (joy->amount < 0.25f)
                ctx->index = -1;
            else if (g < -0.7f)
                ctx->index = 1;
            break;
        case 1:
            if (joy->amount < 0.25f)
                ctx->index = -1;
            else if (0.7f < g)
                ctx->index = 0;
            break;
        case -1:
            if (0.35f < joy->amount) {
                if (0.0f <= g)
                    ctx->index = 0;
                else
                    ctx->index = 1;
            }
            break;
        }
        switch (ctx->index) {
        case 0:
            ctx->reversed = 0;
            break;
        case 1:
            ctx->reversed = 1;
            break;
        }
        if (focus->flags) {
            g = 0.0f;
        } else {
            SMP smp;
            float limit;
            smp.slowSpeed = ctx->program240.x20;
            smp.fastSpeed = ctx->program240.x60;
            smp.fastDuration = D_002BF5C8;
            if (focus->cyclic) {
                limit = smp.fastSpeed;
            } else if (ctx->reversed) {
                SmoothValue__FfffP3SMPPf(ctx->value, 50.0f, g_clock.deltaTime, &smp, &limit);
                limit = -limit;
            } else {
                SmoothValue__FfffP3SMPPf(ctx->value, GetShapeMaxDistance(focus) - 50.0f, g_clock.deltaTime, &smp, &limit);
            }
            float ratio = (limit - smp.slowSpeed) / (smp.fastSpeed - smp.slowSpeed);
            float amount = joy->amount;
            if (amount < 0.0f)
                g = 0.0f;
            else if (ratio < amount)
                g = ratio;
            else
                g = amount;
        }
        if (ctx->reversed)
            heading = NormalizeAngle__Ff(heading + 3.1415927f);
        if (0.0f < g) {
            SetJtGrabKind(ctx, 4);
            UpdateGaitBlend(&ctx->program240, g);
            SetStepTargetHeading(ctx->actor, heading);
            UpdateGaitFootsteps(&ctx->program240);
        } else {
            SetJtGrabKind(ctx, 0);
            SetStepTargetHeading(ctx->actor, heading);
        }
        break;
    }
    case 5:
    case 9:
        UpdateJtHands(ctx->sub);
        break;
    case 8:
        switch (ctx->step) {
        case 7:
        case 8:
            break;
        default:
            if (0.0f < joy->amount) {
                SetJtGrabKind(ctx, 10);
                ENTRY *e = &ctx->axes2[3];
                if (!e->active)
                    e = &ctx->axes2[2];
                SetXYZ((U *)&ctx->actor->qwUnknown190, -joy->y * LengthXYOf(e->direction.q), joy->x * LengthXYOf(ctx->axes2[1].direction.q), 0.0f);
                UpdateDirectionalBlend(ctx->actor, ctx->axes2, ctx->request2);
                ACTOR *actor = ctx->actor;
                float turn = -((U *)&actor->qwUnknown190)->f[1] / D_002BF5CC;
                actor->qwUnknown190 = ScaleVecX(D_002A2DA0, DotVec(actor->qwUnknown190, D_002A2DA0));
                SetStepTargetHeading(ctx->actor, ctx->actor->targetHeading + turn * g_clock.deltaTime);
            } else {
                SetJtGrabKind(ctx, 9);
            }
            if (ctx->actor == D_002E1E40)
                FitStepToTerrain(ctx->actor, &D_002BF630, 3, D_002BF5D0);
            else
                FitStepToTerrain(ctx->actor, &D_002BF630, 3, D_002BF600);
            break;
        }
        break;
    case 10: {
        U v;
        v.f[2] = 0.0f;
        v.f[1] = 0.0f;
        v.f[0] = 0.0f;
        ACTOR *actor = ctx->actor;
        ASEGA *anim = actor->activeAnimation;
        if (anim) {
            CHN *b = FindAsegChannel(anim->animation, actor, 1, 1);
            if (b) {
                ReadBinding(b, anim, &v);
                v.q = ScaleVec(v.q, VuFloat(-1.0f));
            }
        }
        *(U *)&ctx->actor->qwUnknown190 = v;
        break;
    }
    case 11:
        if (ctx->focus) {
            FaceStepToward(ctx->actor, ctx->focus);
            FixStepAngularVelocity(ctx->actor);
        }
        break;
    }

    switch (ctx->step) {
    case 1:
        UpdateDirectionalBlend(ctx->actor, ctx->axes, ctx->request);
        if (ctx->actor->unknown1024 == ctx->x68) {
            switch (ctx->x2C) {
            case 1:
            case 2:
                if (ctx->x2C == 1)
                    ctx->maximum = StdMax(ctx->maximum, ctx->axes[1].weight);
                else
                    ctx->maximum = StdMax(ctx->maximum, ctx->axes[0].weight);
                break;
            }
        }
        break;
    case 0:
        if (ctx->action == 3) {
            ACTOR *actor = ctx->actor;
            RebuildJtXmg(actor, actor->unknown19D8, actor->unknown19E0, actor->pact5Unk19f0, 5.0f, &actor->unknown1520[0]);
            RebuildJtXmg(actor, actor->unknown19D4, actor->unknown19DC, actor->pact5Unk19f4, 5.0f, &actor->unknown1520[1]);
            ClampJtGrabCurveParam(actor, &actor->unknown1520[0], actor->pact5Unk19f0);
            ClampJtGrabCurveParam(actor, &actor->unknown1520[1], actor->pact5Unk19f4);
        }
        break;
    }
}
} // namespace M_UpdateJtActiveHide

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SetJtGrabState {
void SetJtGrabKind(JTGRAB *pjtgrab, int n) __asm__("SetJtGrabKind");
void SetJtCaneState__FP2JTi(JT *pjt, int n) __asm__("SetJtCaneState__FP2JTi");
void UpdateJtCane__FP2JT(JT *pjt) __asm__("UpdateJtCane__FP2JT");
void SetJtHandsTarget(JTHANDS *phands, int n) __asm__("SetJtHandsTarget");
void ClearJtDetectionOutOfSight(JTGRAB *pjtgrab) __asm__("ClearJtDetectionOutOfSight");
// Starts a grab action: resets the kind, stores the state, time, object and grab spot, and switches the player's
// generic state to 2 (grab).
// Also known as: StartInteract (upstream readable / Sly 1 name; StartInteract__FP5ACTORii3JTL).
extern "C" void SetJtGrabState(JTGRAB *grab, int n, LO *plo, GRABSPOT *pspot)
{
    SetJtGrabKind(grab, -1);
    grab->state = n;
    grab->enterTime = g_clock.time;
    grab->grabSpot = pspot;
    grab->grabbedObject = plo;
    grab->player->SetPlayerState(2);
    switch (n) {
    case 0: case 13:
        n = 5;
        break;
    case 6:
    {
        JT *jt = (JT *)grab->player;
        n = 0;
        jt->hands.ploTarget = plo;
        SetJtCaneState__FP2JTi(jt, 0);
        UpdateJtCane__FP2JT(jt);
        break;
    }
    case 5: case 9:
        grab->hands.ploTarget = grab->grabbedObject;
        SetJtHandsTarget(&grab->hands, 0);
        n = 0;
        break;
    case 8:
        n = 7;
        ClearJtDetectionOutOfSight(grab);
        break;
    case 1:
        n = 0;
        ClearJtDetectionOutOfSight(grab);
        break;
    case 11: case 12:
        n = 0;
        grab->unknown500 = 1;
        break;
    default:
        n = 0;
        break;
    }
    SetJtGrabKind(grab, n);
}
} // namespace M_SetJtGrabState

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00192A28 {
typedef int qword __attribute__((mode(TI)));

void *FindDetectorTarget(void *a, void *b) __asm__("FindDetectorTarget");
void SetDetectorTargetState(void *a, void *b, int c) __asm__("SetDetectorTargetState");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj) __asm__("IntersectSwBoundingBox");
int FindFirstObjectHitByLine(int z, qword a, qword b, int i, int j, int k) __asm__("FindFirstObjectHitByLine");
void FilterDetectorLineOfSightHit(void) __asm__("FilterDetectorLineOfSightHit");
extern float D_002BF638 __asm__("D_002BF638");

struct E { int x0; int x4; };
struct F { char pad[0xF8]; int xf8; };
struct O : STEP { char pad[0xF64 - 0x2C0]; F *xf64; char pad2[0xFE0 - 0xF68]; O *xfe0; };
struct P { void *x0; };
// The frame's scope and the two sector indices IntersectSwBoundingBox returns, laid out as ClearJtDetectionOutOfSight's frame
// has them. At namespace scope, not local to the function: g++ 2.95 emits a local class's implicit constructor and
// destructor out of line even when every use is inlined, and the ELF has no such functions.
struct SCOPEDIJ { char c; STACKSCOPE frame; int i, j; };
extern void ClearJtDetectionOutOfSight(P *p) __asm__("ClearJtDetectionOutOfSight");
// For each guard that currently tracks the player, clears the detection when the player is out of its sight.
// (inferred)
// Also known as: ForgetGuardsInteract (upstream readable / Sly 1 name; ForgetGuardsInteract__FP5ACTOR).
void ClearJtDetectionOutOfSight(P *p)
{
    O *o;
    for (o = (*(O * *)&g_sw.dlStepguard); o; o = o->xfe0) {
        E *e = (E *)FindDetectorTarget(o->xf64, p->x0);
        if (!e || !e->x4 || (o->xf64->xf8 & 1)) continue;
        qword a = GetLoWorldPos(o);
        qword b = GetLoWorldPos(p->x0);
        qword d, t;
        float dist;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d), "=&j"(t));
        __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
        bool far = D_002BF638 < dist;
        if (far) {
            SetDetectorTargetState(o->xf64, e, 0);
            continue;
        }
        SCOPEDIJ x;

        IntersectSwBoundingBox(&g_sw, a, b, (void *)FilterDetectorLineOfSightHit, o, &x.i, &x.j);
        if (!FindFirstObjectHitByLine(0, a, b, x.i, x.j, 0)) continue;
        SetDetectorTargetState(o->xf64, e, 0);
    }
}
} // namespace M_func_00192A28

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SetJtGrabKind {
// SetJtGrabKind: set the step of a hero action context (the struct at actor+0x4C0; UpdateJtGrabInput calls it).
// Leaves the old step (cleanup per step / per action), then enters the new one: picks the animation clip, the
// script, the flags and the blend for the new step and starts them on the actor.
//
// Matching notes (wave-5 lane 5, fresh rewrite):
// - p ends up in s2 only because two block-local call-crossing pointers take s0/s1 while p is live (old action 4:
//   `ACTOR *actor = p->pactor` before the position call; action 34: `AUX *aux = p->aux` before GetLoWorldPos).
// - Normalize: return-style inline plus a split vdiv / vmulq (see Unit) keeps the fallback lq/qmtc2 of the target.
// - old step 7: VuFloat's "r,V" alternative (UpdateJtGrabInput idiom) fixes the vt/fn and actor/height registers.
// - step -1 reset: the zeros are a chained `param = value = 0.0f` (0.0f in f0) and sched1 puts the stores that kill
//   -1 / 0.0f first, so their statement order is chosen to give the target's store order.

typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;

struct M3 { qword row[3]; };
struct JOY { char pad0[0x1C]; float amount; };
struct PROGRAM;
struct EFFECT;
struct COMPONENT { void *object; };
struct PROGRAM_ITEM { void *owner; ASEG *clip; char pad8[0xA0 - 8]; };
struct LO { char pad0[0xC]; void *vptr; char pad10[0x40 - 0x10]; uint flags; };
struct ACTOR {
    char pad0[0xC]; void *vptr; char pad10[0x40 - 0x10]; uint flags; char pad44[0x150 - 0x44]; JOY *pjoy;
    char pad154[0x190 - 0x154]; qword velocity; char pad1A0[0x1B0 - 0x1A0]; char local[0x20C - 0x1B0]; float tStamp;
    char pad210[0x230 - 0x210]; qword position; qword posOffset; char pad250[0x900 - 0x250]; char secondary[0x9BC - 0x900];
    int x9BC; char pad9C0[0x1024 - 0x9C0]; void *focus; char pad1028[0x1030 - 0x1028]; ASEGA *animation;
    COMPONENT component; int x1038; char pad103C[0x1048 - 0x103C]; float height; char pad104C[0x1060 - 0x104C];
    qword motion; char pad1070[0x10C0 - 0x1070]; void *anchor; char pad10C4[0x130C - 0x10C4]; int x130C;
    char pad1310[0x1318 - 0x1310]; int x1318; char pad131C[0x18D8 - 0x131C]; int x18D8; char pad18DC[0x19A0 - 0x18DC];
    LO *first; LO *second; char pad19A8[0x1AF0 - 0x19A8]; ASEG *specialClip; char pad1AF4[0x1BCC - 0x1AF4]; int x1BCC;
    char pad1BD0[0x1C30 - 0x1BD0]; PROGRAM_ITEM program; char pad1CD0[0x2048 - 0x1CD0]; EFFECT *effect;
};
struct DATA { char pad0[0x10]; float x; char pad14[0xC]; float y; char pad24[0xC]; float z; };
struct SOURCE {
    char pad0[0xC]; void *vptr; char pad10[0xE0 - 0x10]; ACTOR *owner; char padE4[0x10C - 0xE4]; float parameter;
    char pad110[0x15C - 0x110]; ACTOR *other; char pad160[0x190 - 0x160]; ACTOR *actor; char pad194[0x1BC - 0x194];
    int enabled;
};
struct AUX {
    ACTOR *owner; char pad4[0x10C - 4]; SOURCE *target; char pad110[4]; DATA *data; char pad118[0x12C - 0x118];
    int enabled; char pad130[0x150 - 0x130]; qword position; float value;
};
struct SETTING { void *focus; };
struct ATTACH { char pad0[0x30]; ASEG *clip; char pad34[0x84 - 0x34]; SOURCE *focus; char pad88[0xB0 - 0x88]; };
struct CONTEXT {
    ACTOR *pactor; float tStamp; int action; int step; float tStep; SOURCE *focus; SETTING *setting; float param;
    float value; int reversed; int index; int previous; char pad30[4]; int changed; char pad38[0x68 - 0x38]; AUX *aux;
    char pad6C[0x170 - 0x6C]; ASEG *clip170, *clip174, *clip178, *clip17C; PROGRAM_ITEM program180;
    char pad220[0x230 - 0x220]; PROGRAM *script230; ASEG *clip234, *clip238; char pad23C[4]; PROGRAM_ITEM program240;
    char pad2E0[0x314 - 0x2E0]; ASEG *overrideClip; float gain318, gain31C; ASEG *clip320, *clip324, *clip328;
    PROGRAM *script32C; char pad330[0x430 - 0x330]; ASEG *clip430; char pad434[0x440 - 0x434]; ATTACH attach;
    ASEG *clip4F0, *clip4F4, *clip4F8; int tag; int fInhibit; char pad504[0x50C - 0x504];
    ASEG *clip50C, *clip510, *clip514;
};
struct VU_FLOAT { qword q; };
union U { qword q; float f[4]; int i[4]; };


extern ACTOR *D_002E1E40 __asm__("D_002E1E40");         // the player
extern ACTOR *D_002F7900 __asm__("D_002F7900");
extern qword D_002A2DA0 __asm__("D_002A2DA0"), g_normalZ __asm__("g_normalZ"), D_002A2DD0 __asm__("D_002A2DD0"), D_002BF650 __asm__("D_002BF650");
extern float D_002BF63C __asm__("D_002BF63C");
extern float D_002BF640 __asm__("D_002BF640");
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
M3 *GetLoWorldMatrix(void *) __asm__("GetLoWorldMatrix");
void FindShapeClosestPoint(SOURCE *, qword, void *, void *, void *, float *, float *) __asm__("FindShapeClosestPoint");
void SetBarrelState(SOURCE *, int) __asm__("SetBarrelState");
void SubscribeLoObject(SOURCE *, ACTOR *) __asm__("SubscribeLoObject");
void UnsubscribeLoObject(SOURCE *, ACTOR *) __asm__("UnsubscribeLoObject");
void LinkStepAttachment(ACTOR *, SOURCE *, int) __asm__("LinkStepAttachment");
void EvaluateShapeAtParam(SOURCE *, float, qword *, qword *) __asm__("EvaluateShapeAtParam");
EFFECT *CreateSfxSound(int) __asm__("CreateSfxSound");
void SetSndSourceLo(EFFECT *, SOURCE *) __asm__("SetSndSourceLo");
void RcPtr_Assign(EFFECT **, EFFECT *) __asm__("RcPtr_Assign");
void FaceStepToward(ACTOR *, SOURCE *) __asm__("FaceStepToward");
void FixStepAngularVelocity(ACTOR *) __asm__("FixStepAngularVelocity");
void SetStepTargetHeadingFromVector(ACTOR *, qword) __asm__("SetStepTargetHeadingFromVector");
void SetCmPolicy(CM *, int, CPLCY *, ACTOR *, void *) __asm__("SetCmPolicy");
void RevokeCmPolicy(CM *, int, int, CPLCY *, ACTOR *, void *) __asm__("RevokeCmPolicy");
void SetJtCaneState__FP2JTi(ACTOR *, int) __asm__("SetJtCaneState__FP2JTi");
void SetJtHandsTarget(ATTACH *, int) __asm__("SetJtHandsTarget");
void ReleaseJtHandsActs(void *) __asm__("ReleaseJtHandsActs");
void StopSnd(EFFECT *, float) __asm__("StopSnd");
void SetStepplayerTnOverride(ACTOR *, void *) __asm__("SetStepplayerTnOverride");
void ResetJtGrabTnParams(CONTEXT *) __asm__("ResetJtGrabTnParams");
qword ConvertPosBetweenLos(void *, void *, qword) __asm__("ConvertPosBetweenLos");
void ResetGaitFeet(PROGRAM_ITEM *) __asm__("ResetGaitFeet");
void ReleaseAsega(ASEGA **) __asm__("ReleaseAsega");
void ReleaseSma(COMPONENT *) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *, ACTOR *, float, float, int, ASEGA **) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void ReplaceSma(PROGRAM *, ACTOR *, int, int, COMPONENT *) __asm__("ReplaceSma");
void EnableJtLeanActs(ACTOR *, int) __asm__("EnableJtLeanActs");


// bit 0 set: the word holds the flags itself; otherwise it points to them
static inline void AddFlags(uint *pflags, uint bits)
{
    uint w = *pflags;
    if ((w ^ 1) & 1)
        *(uint *)w |= bits;
    else
        *pflags = w | bits;
}

// The never-chosen "V" alternative is allocation-only: the word stays a MEM until reload (as in UpdateJtGrabInput).
static inline VU_FLOAT VuFloat(const float &g) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j,j"(v.q) : "r,V"(*(const int *)&g)); return v; }

// n < limit (keeps the two range tests apart, as in SetJtAirState).
static inline bool Less(int n, int limit) { return n < limit; }   // as in SetJtAirState: keeps the two range tests apart
// The vector is moved to a GPR once (gcc's own qmfc2 for the "r" inputs) and its x word is stored straight from there:
// no C construct gives a 32-bit store of the low word of a TImode GPR (unions go through the stack, casts sign-extend).
// Same StoreX/GetY/GetZ helpers as the matched RenderCid106Self.cpp.
static inline void SetX(float *pg, qword q) { __asm__("sw %1,%0" : "=m"(*pg) : "r"(q)); }

// a + b * g
static inline qword AddScaled(qword a, qword b, VU_FLOAT g)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(a), "j"(b), "j"(g.q));
    return r;
}
// Length of the xyz part of v (x lane).
static inline qword Length(qword v)
{
    qword d, one, len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(len) : "j"(d));
    return len;
}
// vdiv/vwaitq and vmulq as separate ops: the divide arm then does not start with a set of the result, so jump.c
// leaves the fallback copy on its own path (the target's lq before the branch and qmtc2 in the delay slot)
static inline qword Unit(qword v, qword len)
{
    qword r;
    __asm__ __volatile__("vdiv Q,$vf0w,%0x\n\tvwaitq" : : "j"(len));
    __asm__("vmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(v));
    return r;
}

// Unit vector along v, or fallback when its length is too small.
static inline qword Normalize(qword v, qword fallback)
{
    qword len = Length(v);
    if (QwordX(len) < 0.0001f)
        return fallback;
    return Unit(v, len);
}
// Normalize *pv in place (fallback D_002A2DA0).
static inline void Normalize(qword *pv) { *pv = Normalize(*pv, D_002A2DA0); }
// Rotate q by the 3x3 matrix m.
static inline qword Transform(const M3 &m, qword q)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
            : "=j"(r) : "j"(m.row[0]), "j"(m.row[1]), "j"(m.row[2]), "j"(q));
    return r;
}

// Sets the grab sub-step (kind, +0xC): leaves the old one (per-kind/per-state cleanup) and enters the new (animation
// clips, physics, camera).
// Also known as: SetInteractState (upstream readable / Sly 1 name; SetInteractState__FP5ACTORi).
extern "C" void SetJtGrabKind(CONTEXT *grab, int step)
{
    if (step == grab->step)
        return;

    // leave the old step
    switch (grab->step) {
    case 1:
        grab->pactor->x1038 = -1;
        break;
    case -1:
        if (grab->focus && IsBasicDerivedFrom__FP5BASIC3CID(grab->focus, 10)) {
            SOURCE *source = grab->focus;
            FindShapeClosestPoint(source, GetLoWorldPos(grab->pactor), 0, 0, 0, &grab->param, &grab->value);
        }
        switch (grab->action) {
        case 1: {
            SOURCE *source = grab->focus;
            source->actor = grab->pactor;
            SetBarrelState(source, 2);
            if (grab->pactor == D_002E1E40)
                AsLo(grab->pactor->first)->Remove();
            break;
        }
        case 4: {
            SOURCE *source = grab->focus;
            AsBasic(source)->HandleSpliceEvent(5, 0, 0, 0, 0);
            SubscribeLoObject(source, grab->pactor);
            LinkStepAttachment(grab->pactor, source, 1);
            ACTOR *actor = grab->pactor;
            actor->position = GetLoWorldPos(source);
            break;
        }
        case 3: {
            SOURCE *source = grab->focus;
            U dir, tangent;
            AsBasic(source)->HandleSpliceEvent(5, 0, 0, 0, 0);
            SubscribeLoObject(source, grab->pactor);
            LinkStepAttachment(grab->pactor, source, 1);
            grab->index = -1;
            EvaluateShapeAtParam(source, grab->param, &grab->pactor->position, &tangent.q);
            dir.i[2] = 0;
            Normalize(&dir.q);
            qword normal = dir.q;   // read before the call: a VU pseudo spilled to 0x50
            grab->reversed = QwordX(VuDot(normal, GetLoWorldMatrix(grab->pactor)->row[0])) < D_002BF63C;
            break;
        }
        case 2: {
            EFFECT *effect = CreateSfxSound(0x5C);
            SetSndSourceLo(effect, 0);
            RcPtr_Assign(&D_002E1E40->effect, effect);
            break;
        }
        case 11:
            if (grab->focus)
                SubscribeLoObject(grab->focus, grab->pactor);
            break;
        case 12:
            if (grab->focus)
                FaceStepToward(grab->pactor, grab->focus);
            FixStepAngularVelocity(grab->pactor);
            break;
        }
        grab->previous = -1;
        grab->pactor->tStamp = g_clock.time;
        break;
    case 7:
        if (Less(step, 11) && !Less(step, 9)) {
            SOURCE *source = grab->focus;
            if (source->other) {
                qword pos = GetLoWorldPos(source->other);
                AsStepplayer(grab->pactor)->TranslateToPos(VuSub(pos, grab->pactor->posOffset));
                grab->pactor->motion = AddScaled(pos, g_normalZ, VuFloat(grab->pactor->height));
                SetStepTargetHeadingFromVector(grab->pactor, GetLoWorldMatrix(source->other)->row[0]);
                AsStepplayer(grab->pactor)->RotateToMat((MATRIX3 *)grab->pactor->local);
                AsStepplayer(grab->pactor)->SetVelocity(D_002A2DD0);
                AsStepplayer(grab->pactor)->SetAngularVelocity(D_002A2DD0);
            }
            SetCmPolicy(&g_cm, 10, &g_cm.hangPolicy, grab->pactor, grab->pactor->anchor);
        }
        break;
    case 9:
    case 10:
        if (!(Less(step, 11) && !Less(step, 9)))
            RevokeCmPolicy(&g_cm, 2, 10, &g_cm.hangPolicy, 0, 0);
        break;
    }

    // enter the new one
    ASEG *clip = 0;
    PROGRAM *script = 0;
    int flags = 1;
    int special = 0;
    float blend = 0.0f;
    float speed = 1.0f;
    switch (step) {
    case -1:
        grab->fInhibit = 0;
        switch (grab->action) {
        case 1:
            SetBarrelState(grab->focus, 0);
            if (grab->pactor == D_002E1E40)
                AsLo(grab->pactor->first)->Add();
            break;
        case 3:
        case 4: {
            SOURCE *source = grab->focus;
            AsBasic(source)->HandleSpliceEvent(6, 0, 0, 0, 0);
            UnsubscribeLoObject(source, grab->pactor);
            LinkStepAttachment(grab->pactor, source, 0);
            break;
        }
        case 6:
            if (!grab->pactor->x18D8)
                SetJtCaneState__FP2JTi(grab->pactor, -1);
            break;
        case 11:
            if (grab->focus)
                UnsubscribeLoObject(grab->focus, grab->pactor);
            grab->pactor->x130C = 0;
            grab->pactor->x9BC = -1;
            break;
        case 5:
        case 9:
            SetJtHandsTarget(&grab->attach, -1);
            break;
        case 12:
            grab->pactor->x1318 = -1;
            ReleaseJtHandsActs(grab->pactor->secondary);
            if (grab->pactor == D_002E1E40)
                grab->pactor->x1BCC = 0;
            break;
        case 2:
            if (D_002E1E40->effect) {
                StopSnd(D_002E1E40->effect, 0.0f);
                RcPtr_Assign(&D_002E1E40->effect, 0);
            }
            break;
        }
        if (grab->pactor->focus) {
            if (grab->pactor->focus == grab->aux)
                SetStepplayerTnOverride(grab->pactor, 0);
            else if (grab->pactor->focus == grab->setting->focus)
                SetStepplayerTnOverride(grab->pactor, 0);
        }
        // store order: the last uses of -1 and 0.0f (tag, param) are scheduled first, the rest stay in this order
        grab->pactor->x1038 = -1;
        grab->action = -1;
        grab->step = -1;
        grab->focus = 0;
        grab->setting = 0;
        grab->param = grab->value = 0.0f;
        grab->changed = 0;
        grab->tag = -1;
        break;
    case 0:
    case 5:
        switch (grab->action) {
        case 0:
            clip = grab->clip174;
            flags = 9;
            break;
        case 13:
            clip = step == 0 ? grab->clip510 : grab->clip174;
            flags = 9;
            break;
        case 2:
            clip = grab->clip514;
            flags = 9;
            break;
        case 1:
            script = grab->script230;
            flags = 1;
            break;
        case 4:
            clip = grab->clip234;
            flags = 9;
            special = 7;
            break;
        case 3:
            if (grab->focus->enabled) {
                ASEG *override = grab->overrideClip;
                if (override) {
                    clip = override;
                    grab->gain318 = grab->gain31C = 0.0f;
                } else
                    clip = grab->clip238;
            } else
                clip = grab->clip238;
            flags = 9;
            special = 31;
            break;
        case 6:
        case 7:
            clip = grab->clip320;
            flags = 9;
            break;
        case 5:
        case 9:
            grab->attach.focus = grab->focus;
            speed = 0.0f;
            SetJtHandsTarget(&grab->attach, 0);
            clip = grab->attach.clip;
            blend = clip->duration * 0.5f;
            break;
        case 10:
            clip = grab->pactor->pjoy->amount >= D_002BF640 ? grab->clip4F0 : grab->clip4F4;
            break;
        case 11:
            if (grab->tag == 21) {
                clip = D_002F7900->specialClip;
                if (clip)
                    break;
            }
            clip = grab->clip4F8;
            break;
        case 12:
            clip = grab->clip50C;
            break;
        }
        grab->pactor->velocity = D_002A2DD0;
        break;
    case 1:
        switch (grab->action) {
        case 0:
            clip = grab->clip170;
            if (!grab->pactor->focus) {
                if (IsBasicDerivedFrom__FP5BASIC3CID(grab->focus, 12)) {
                    SOURCE *source = grab->focus;
                    if (source->actor)
                        SetStepplayerTnOverride(grab->pactor, source->actor);
                    else {
                        ResetJtGrabTnParams(grab);
                        grab->aux->target = source;
                        SetStepplayerTnOverride(grab->pactor, grab->aux);
                    }
                } else if (IsBasicDerivedFrom__FP5BASIC3CID(grab->focus, 34)) {
                    SOURCE *source = grab->focus;
                    if (source->owner)
                        SetStepplayerTnOverride(grab->pactor, source->owner);
                    else {
                        ResetJtGrabTnParams(grab);
                        grab->aux->enabled = 1;
                        AUX *aux = grab->aux;
                        aux->position = ConvertPosBetweenLos(0, grab->aux, GetLoWorldPos(source));
                        grab->aux->value = source->parameter;
                        SetStepplayerTnOverride(grab->pactor, grab->aux);
                    }
                } else if (IsBasicDerivedFrom__FP5BASIC3CID(grab->focus, 38)) {
                    SOURCE *source = grab->focus;
                    if (source->owner)
                        SetStepplayerTnOverride(grab->pactor, source->owner);
                    else {
                        ResetJtGrabTnParams(grab);
                        qword v = Transform(*GetLoWorldMatrix(source), D_002BF650);
                        SetX(&grab->aux->data->x, v);
                        grab->aux->data->y = QwordY(v);
                        grab->aux->data->z = QwordZ(v);
                        SetStepplayerTnOverride(grab->pactor, grab->aux);
                    }
                }
            }
            break;
        case 13:
            clip = grab->clip170;
            break;
        }
        flags = 9;
        break;
    case 4:
        switch (grab->action) {
        case 3:
            clip = grab->program240.clip;
            ResetGaitFeet(&grab->program240);
            break;
        case 1:
            clip = grab->program180.clip;
            ResetGaitFeet(&grab->program180);
            break;
        case 2:
            clip = grab->pactor->program.clip;
            ResetGaitFeet(&grab->pactor->program);
            break;
        }

        flags = 9;
        break;
    case 3:
        clip = grab->clip17C;
        grab->pactor->velocity = D_002A2DD0;
        break;
    case 2:
        clip = grab->clip178;
        grab->pactor->velocity = D_002A2DD0;
        break;
    case 7: {
        SOURCE *source = grab->focus;
        clip = grab->clip324;
        if (source->other) {
            M3 basis = *GetLoWorldMatrix(source->other);
            SetStepTargetHeadingFromVector(grab->pactor, basis.row[0]);
        }
        grab->pactor->velocity = D_002A2DD0;
        AsBasic(source)->HandleSpliceEvent(5, 0, 0, 0, 0);
        break;
    }
    case 8:
        clip = grab->clip328;
        grab->pactor->velocity = D_002A2DD0;
        AsBasic(grab->focus)->HandleSpliceEvent(6, 0, 0, 0, 0);
        break;
    case 9:
        script = grab->script32C;
        if (grab->step == 7) {
            AddFlags(&grab->pactor->flags, 0x10);
            if (grab->pactor == D_002E1E40) {
                AddFlags(&grab->pactor->first->flags, 0x10);
                AddFlags(&D_002E1E40->second->flags, 0x10);
            }
        }
        grab->pactor->velocity = D_002A2DD0;
        break;
    case 10:
        clip = grab->clip430;
        grab->pactor->x1038 = -1;
        flags = 9;
        grab->pactor->velocity = D_002A2DD0;
        break;
    }
    grab->step = step;
    grab->tStep = g_clock.time;
    if (step == -1)
        return;
    ReleaseAsega(&grab->pactor->animation);
    ReleaseSma(&grab->pactor->component);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(clip, grab->pactor, blend, speed, flags, &grab->pactor->animation);
    ReplaceSma(script, grab->pactor, -1, flags, &grab->pactor->component);
    AsStepplayer(grab->pactor)->ApplyStateStepParams();
    if (IsBasicDerivedFrom__FP5BASIC3CID(grab->pactor, 53))
        EnableJtLeanActs(grab->pactor, special);
}
} // namespace M_SetJtGrabKind

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FJtGrabNearestCandidate {
extern float D_002BF660 __asm__("D_002BF660");
extern float D_002BF664 __asm__("D_002BF664");
extern JT *D_002E1E40 __asm__("D_002E1E40");
int IsBasicDerivedFrom__FP5BASIC3CID(LO *plo, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetJtGrabState(JTGRAB *pjtgrab, int n, LO *plo, GRABSPOT *pspot) __asm__("SetJtGrabState");
// Walks the sorted grab candidates (distance < D_002BF660) and maps the nearest usable one's class to a grab state;
// returns nonzero if a grab started. Runtime: CID27 gives state 8, CID12/34 state 0, rails state 3.
// Also known as: FInteractCircle (upstream readable / Sly 1 name; FInteractCircle__FP5ACTOR).
extern "C" int TryGrabJtNearestCandidate(JTGRAB *grab)
{
    SW *g = &g_sw;
    int n = g->cjtgrabcand;
    int i;
    for (i = 0; i < n; i++) {
        JTGRABCAND *e = &g->ajtgrabcand[i];
        LO *o;
        int k;
        if (e->distance > D_002BF660) break;
        o = e->object;
        if (IsBasicDerivedFrom__FP5BASIC3CID(o, 0x13) || IsBasicDerivedFrom__FP5BASIC3CID(o, 0xB))
            k = (grab->player == (STEPPLAYER *)D_002E1E40) ? 6 : 5;
        else if (IsBasicDerivedFrom__FP5BASIC3CID(o, 0x1B))
            k = 8;
        else if (IsBasicDerivedFrom__FP5BASIC3CID(o, 0xC))
            k = 0;
        else if (IsBasicDerivedFrom__FP5BASIC3CID(o, 0x22))
            k = 0;
        else if (IsBasicDerivedFrom__FP5BASIC3CID(o, 0x26))
            k = ((CID38 *)e->object)->nGrabState;
        else
            k = IsBasicDerivedFrom__FP5BASIC3CID(o, 0x23) ? 7 : -1;
        if (k != -1) {
            SetJtGrabState(grab, k, o, e->grabSpot);
            return 1;
        }
    }
    return 0;
}
} // namespace M_FJtGrabNearestCandidate

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_GetJtRailLanding {
/* GetJtRailLanding (Claude agent): SetVectorCylind's qword result assigned plainly so gcc emits the qmtc2 itself; Dot's float via an mtc1 asm fed by gcc's own qmfc2. */

typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int i[4];} U;
struct T {char pad[0x18];float x18,x1C;};
struct O : SO { struct T*controller;char pad154[0x240-0x154];qword pos;};
struct P {struct O*o;char pad4[0x260-4];float x260;char pad264[0x2A0-0x264];float x2A0;};
struct CURVE;
extern float D_002BF668 __asm__("D_002BF668");
extern qword g_normalZ __asm__("g_normalZ"),D_002A2DD0 __asm__("D_002A2DD0");
void EvaluateShapeAtParam(struct CURVE*,float,qword*,qword*) __asm__("EvaluateShapeAtParam");
qword SetVectorCylind(float,float,float) __asm__("SetVectorCylind");
float GetShapeDistanceAtParam(struct CURVE*,float) __asm__("GetShapeDistanceAtParam");
float GetShapeMaxDistance(struct CURVE*) __asm__("GetShapeMaxDistance");
float SmoothValue__FfffP3SMPPf(float,float,float,struct SMP*,float*) __asm__("SmoothValue__FfffP3SMPPf");
// Remove a's component along the unit normal n (register-pinned).
extern "C" { static inline qword Reject(qword a,qword n){register qword d __asm__("vf2");qword one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a),"j"(n));__asm__("vmulx.xyzw %0,%1,%2x":"=j"(d):"j"(n),"j"(d));__asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"0"(d));return d;} }
// Unit vector along a, or fallback when its length is too small (register-pinned).
extern "C" { static inline qword Normal(qword a,qword fallback){register qword d __asm__("vf1");register qword one __asm__("vf4");float len;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));__asm__("qmfc2.ni %0,%1":"=r"(len):"j"(d));if(len<0.0001f){d=fallback;}else{__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(d):"j"(d),"j"(a));}return d;} }
// Dot product of the xyz parts of a and b as a float.
extern "C" { static inline float Dot(qword a,qword b){qword d=b,one;float f;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a),"0"(d));__asm__("mtc1 %1,%0":"=f"(f):"r"(d));return f;} }
// Return a scaled by g.
extern "C" { static inline qword Scale(qword a,float g){U u;qword s,r;__asm__("qmtc2.ni %1,%0":"=j"(s):"r"(g));u.q=s;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(s));return r;} }
// For a rail target at parameter t: the landing position offset and a velocity along the rail tangent.
// Also known as: RailPointVelocity (upstream readable / Sly 1 name;
//   RailPointVelocity__FP5ACTORP2LOPG9VU_VECTORP6VECTORf).
extern "C" void GetJtRailLanding(struct P *p, struct CURVE *curve, qword *outPos, qword *outVel, float t)
{
    U pos, dir, dead;
    struct SMP smp;
    float low, high;
    qword v;
    float dot, current;
    struct T *shape = p->o->controller;
    EvaluateShapeAtParam(curve, t, &pos.q, &dir.q);
    dir.q = Normal(Reject(dir.q, g_normalZ), D_002A2DD0);
    {
        register float width __asm__("$f0") = shape->x1C;
        register float factor __asm__("$f13") = p->x2A0;
        v = SetVectorCylind(p->o->controller->x18, factor * width, 0.0f);
    }
    dot = Dot(v, dir.q);
    dead.q = v;
    current = GetShapeDistanceAtParam(curve, t);
    smp.slowSpeed = p->x260;
    smp.fastSpeed = p->x2A0;
    smp.fastDuration = D_002BF668;
    (void)SmoothValue__FfffP3SMPPf(current, 50.0f, g_clock.deltaTime, &smp, &low);
    (void)SmoothValue__FfffP3SMPPf(current, GetShapeMaxDistance(curve) - 50.0f, g_clock.deltaTime, &smp, &high);
    {
        float hi = high;
        *outPos = VuSub(pos.q, p->o->pos);
        {
            float g;
            if (dot < low)
                g = low;
            else if (hi < dot)
                g = hi;
            else
                g = dot;
            *outVel = Scale(dir.q, g);
        }
    }
}
} // namespace M_GetJtRailLanding

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00193C98 {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct Q { int x0; int x4; float x8; int xc; U x10; qword x20; };
struct HIT { qword a; U b; qword c[4]; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void SetActRotationGoal(void *r, qword q) __asm__("SetActRotationGoal");
float GetShapeMinDistance(SHAPE *pshape) __asm__("GetShapeMinDistance");
float GetShapeMaxDistance(SHAPE *pshape) __asm__("GetShapeMaxDistance");
float ModPositive__Fff(float g, float hi) __asm__("ModPositive__Fff");
void EvaluateShapeAtDistance(SHAPE *pshape, float g, U *pv, int n) __asm__("EvaluateShapeAtDistance");
qword ConvertPosBetweenLos(int n, STEPPLAYER *pstepplayer, qword q) __asm__("ConvertPosBetweenLos");
int ClipEdgeToSphere(qword a, float g, qword b, qword c, int n, struct HIT *pv) __asm__("ClipEdgeToSphere");
// Advances/clamps the grab parameter along the held curve object (wraps on closed curves, clamps on open ones).
extern "C" void ClampJtGrabCurveParam(STEPPLAYER *grab, struct Q *q, void *r)
{
    SHAPE *o;
    float g, lo, hi, t;
    U v;
    struct HIT hit;
    qword d;
    if (!q->x4) {
        SetActRotationGoal(r, D_002A2DD0);
        return;
    }
    o = (SHAPE *)grab->jtgrab.grabbedObject;
    g = q->x10.f;
    if (grab->jtgrab.isFlipped)
        g = -g;
    lo = GetShapeMinDistance(o);
    hi = GetShapeMaxDistance(o);
    if (o->isClosed)
        t = ModPositive__Fff(grab->jtgrab.spotDistance + g, hi);
    else
        t = Clamp(grab->jtgrab.spotDistance + g, lo, hi);
    EvaluateShapeAtDistance(o, t, &v, 0);
    v.q = ConvertPosBetweenLos(0, grab, v.q);
    if (ClipEdgeToSphere(q->x20, q->x8, q->x10.q, v.q, 1, &hit) > 0) {
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(hit.b.q), "j"(q->x10.q));
        SetActRotationGoal(r, d);
    }
}
} // namespace M_func_00193C98

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateJtInternalXpsHide {
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
typedef struct {qword row[4];} M4;
struct PROJECTION {qword direction;int kind;};
extern qword D_002A2DB0 __asm__("D_002A2DB0");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
M4 *GetLoWorldMatrix(STEPPLAYER *p) __asm__("GetLoWorldMatrix");
void FindShapeClosestPointFromParam(RAIL *t, qword a, struct PROJECTION *projection, qword *p0, qword *p1, float *pf0, float *pf1, float g) __asm__("FindShapeClosestPointFromParam");
float GetShapeMinDistance(RAIL *t) __asm__("GetShapeMinDistance");
float GetShapeMaxDistance(RAIL *t) __asm__("GetShapeMaxDistance");
void EvaluateShapeAtDistance(RAIL *t, float g, qword *point, qword *direction) __asm__("EvaluateShapeAtDistance");
// Dot product of the xyz parts of a and b as a float.
extern "C" { static __inline__ float Dot(qword a, qword b)
{
    qword d, t;
    float r;
    __asm__("vmul.xyz %0, %2, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(t) : "j"(a), "0"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    return r;
} }
// Dot product of the xyz parts of a and b as a float (second register variant).
extern "C" { static __inline__ float Dot2(qword a, qword b)
{
    qword d, t;
    float r;
    int bits;
    __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(d), "=&j"(t)
            : "j"(b), "0"(a));
    __asm__("qmfc2.ni %0,%2\n\tmtc1 %0,%1" : "=&r"(bits), "=f"(r) : "j"(d));
    return r;
} }
// Adds the contact constraints for the held object (state 4: point constraint; state 3: rail/edge ...).
// Sly 1 name and body.
// Also known as: AddInteractContacts (upstream readable / Sly 1 name; AddInteractContacts__FP5ACTOR).
extern "C" void UpdateJtInternalXpsHide(JTGRAB *p)
{
    switch (p->state) {
    case 4: {
        U pos;
        pos.q = GetLoWorldPos(p->grabbedObject);
        p->player->AddAxisXps(0x23, p->grabbedObject, pos.q, D_002A2DB0, *p->player->unknown218);
        break;
    }
    case 3: {
        U v0, v1;
        RAIL *t = (RAIL *)p->grabbedObject;
        int k;
        FindShapeClosestPointFromParam(t, *p->player->unknown218, 0, &v0.q, &v1.q, &p->spotParam, &p->spotDistance, p->spotParam);
        k = 0x22;
        if (t->fSlide == 0 && t->isClosed == 0) {
            U vel;
            M4 *m;
            register qword w __asm__("vf2");
            float d0, d1;
            vel.q = p->player->GetVelocity();
            d0 = Dot(vel.q, v1.q);
            m = GetLoWorldMatrix(p->player);
            w=p->player->qwUnknown190;
            __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(w):"j"(m->row[0]),"j"(m->row[1]),"j"(m->row[2]));
            d1 = -Dot2(w, v1.q);
            if ((d0 < -2.0f || d1 < 2.0f) && p->spotDistance < GetShapeMinDistance(t) + 50.0f) {
                k = 0x2A;
                EvaluateShapeAtDistance(t, GetShapeMinDistance(t) + 50.0f, &v0.q, 0);
            } else if ((2.0f < d0 || -2.0f < d1) && GetShapeMaxDistance(t) - 50.0f < p->spotDistance) {
                U s;
                k = 0x2A;
                EvaluateShapeAtDistance(t, GetShapeMaxDistance(t) - 50.0f, &v0.q, 0);
                qword sq, v1q;
                __asm__("qmtc2.ni %1, %0" : "=j"(sq) : "r"(-1.0f));
                s.q = sq;
                __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v1q) : "j"(v1.q), "j"(s.q));
                v1.q = v1q;
            }
        }
        p->player->AddAxisXps(k, t, v0.q, v1.q, *p->player->unknown218);
        break;
    }
    }
}

typedef char offset_3_direction[(unsigned)&((struct PROJECTION*)0)->direction==0?1:-1];
typedef char offset_3_kind[(unsigned)&((struct PROJECTION*)0)->kind==16?1:-1];
typedef char offset_6_row[(unsigned)&((M4*)0)->row==0?1:-1];
typedef char size_0[sizeof(qword)==16?1:-1];
typedef char size_1[sizeof(U)==16?1:-1];
typedef char size_2[sizeof(M4)==64?1:-1];
typedef char size_6[sizeof(struct PROJECTION)==32?1:-1];
typedef char size_10[sizeof(int)==4?1:-1];
typedef char size_11[sizeof(float)==4?1:-1];
typedef char size_12[sizeof(short)==2?1:-1];
typedef char size_13[sizeof(void*)==4?1:-1];
typedef char alignment_0[__alignof__(U)==16?1:-1];
typedef char alignment_1[__alignof__(qword)==16?1:-1];
typedef char alignment_2[__alignof__(M4)==16?1:-1];
typedef char alignment_4[__alignof__(struct PROJECTION)==16?1:-1];
} // namespace M_UpdateJtInternalXpsHide

// Sets the hide/grab facing kind (+0x2C) and its time; kind 3 and 1/2 adjust the camera flag.
// (Sly 1-derived name: SetJtHfdk.)
/* Sets the grab's hand/foot kind (stamping the time) and from it the focus kind: 1 for kind 3, for kinds 1 and 2 2
   when the player's current TN is the grab's, else 0. The Sly 1 symbol says JT *; Sly 2's takes the JTGRAB, so it
   keeps the symbol as a C-linkage definition. */
// Also known as: SetJtHfdk (upstream readable / Sly 1 name).
extern "C" void SetJtHideFacingKind__FP2JT4HFDK(JTGRAB *grab, HFDK hfdk)
{
    if (hfdk == grab->hfdk) return;
    grab->hfdk = hfdk;
    grab->tHfdk = g_clock.time;
    grab->uFlattenClamp = 0;
    switch (hfdk) {
    case 3:
        grab->jtfock = (JTFOCK)1;
        break;
    case 1: case 2:
        if (grab->player->unknown1024 == grab->cameraTunnel) grab->jtfock = (JTFOCK)2;
        else grab->jtfock = (JTFOCK)0;
        break;
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00194118 {
extern CPD D_002BF670 __asm__("D_002BF670");   /* the grab TN's camera definition (defined below as GRAB_DATA's CPDINIT) */
// Resets a block of the private camera TN's parameters to defaults: no BSP or path, the one camera definition
// D_002BF670, priority 3, mode bits 5, pull on entry.
// Also known as: ResetZoneInteract (upstream readable / Sly 1 name; ResetZoneInteract__FP2LO).
extern "C" void ResetJtGrabTnParams(JTGRAB *grab)
{
    TN *t = grab->cameraTunnel;
    t->bspc.numNodesAllocated = 0;
    t->bspc.numNodes = 0;
    t->bspc.nodes = 0;
    t->grftnm = 5;
    t->path = 0;
    t->numCameraDefinitions = 1;
    t->fCylind = 0;
    t->unknown124 = 0;
    t->pullsWhileIdle = 0;
    t->cameraDefinitions = &D_002BF670;
    t->panLimitAngle = 0.0f;
    t->pullsOnEnter = 1;
    t->priority = 3;
    t->progress = 0.0f;
}
} // namespace M_func_00194118

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PresetJtAccelHide {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct SMP { float a,b,c; };
extern struct SMP D_002BF700 __asm__("D_002BF700");
void EvaluateShapeAtParam(RAIL *prail, U *position, U *direction, float g) __asm__("EvaluateShapeAtParam");
float SmoothValue__FfffP3SMPPf(float a, float b, float dt, struct SMP *p, float *pg) __asm__("SmoothValue__FfffP3SMPPf");
void AddSoAcceleration(STEPPLAYER *pstepplayer, qword q) __asm__("AddSoAcceleration");
// Grab physics preset: in grab state 3 on a curve object smooths the velocity along the curve. Sly 1 name and body.
extern "C" void PresetJtAccelHide(JTGRAB *p)
{
    U v, w, r, k;
    float a, b;
    RAIL *s;
    float d1, d2, x;
    struct SMP *psmp;
    if (p->state != 3)
        return;
    s = (RAIL *)p->grabbedObject;
    if (!s->fSlide)
        return;
    v.q = p->player->GetVelocity();
    EvaluateShapeAtParam(s, 0, &w, p->spotParam);
    psmp = &D_002BF700;
    {
    register float goal __asm__("$f13");
    register float dt __asm__("$f14");
    {
        register qword velocity __asm__("vf1") = v.q;
        register qword direction __asm__("vf3") = w.q;
        register qword other __asm__("vf2");
        register qword one __asm__("vf4");
        __asm__("vmul.xyz %0,%2,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(velocity), "=&j"(one) : "j"(direction));
        __asm__("qmfc2.ni %0,%1" : "=r"(d1) : "j"(velocity));
        other = *(qword *)&p->player->physics->gravity;   // the gravity VECTOR read as a quadword
        __asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(direction), "=&j"(one) : "j"(other));
        {
            int bits;
            goal = s->svSlide;
            __asm__("qmfc2.ni %0,%1" : "=r"(bits) : "j"(direction));
            dt = g_clock.deltaTime;
            __asm__ __volatile__("mtc1 %1,%0" : "=f"(d2) : "r"(bits));
        }
    }
    (void)SmoothValue__FfffP3SMPPf(d1, goal, dt, psmp, &a);
    }
    (void)SmoothValue__FfffP3SMPPf(d1, -s->svSlide, g_clock.deltaTime, psmp, &b);
    x = Clamp(s->rSlide * d2, b, a) - d2;
    qword kq, rq;
    __asm__("qmtc2.ni %1, %0" : "=j"(kq) : "r"(x));
    k.q = kq;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(rq) : "j"(w.q), "j"(k.q));
    r.q = rq;
    AddSoAcceleration(p->player, r.q);
}
} // namespace M_PresetJtAccelHide

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001942F0 {
typedef int qword __attribute__((mode(TI)));
struct S4C { char pad[0x148]; long fl; };
struct A { char pad[0x40]; int x40; char pad2[0x4C-0x44]; struct S4C *x4c; char pad3[0x60-0x50]; qword x60; char pad4[0x88-0x70]; float x88; };
struct O : PO { char pad[0x208 - 0x180]; struct A *x208; char pad2[0x9E0-0x20C]; char places[0x108C-0x9E0]; int x108c; };
struct T { char pad[0xEC]; int xec; char pad2[0x10C-0xF0]; float x10c; char pad3[0x184-0x110]; float x184; char pad4[0x19C-0x188]; int x19c; };
struct P { struct O *x0; int x4; int x8; int pad[2]; struct T *x14; void *x18; int x1c; float x20; };
int IsBasicDerivedFrom__FP5BASIC3CID(struct T *t, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void AddJtPlaceOnCurve(void *p, void *s, float g) __asm__("AddJtPlaceOnCurve");
void AddJtPlaceAtPosition(void *p, int a, qword q) __asm__("AddJtPlaceAtPosition");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
float atan2f(float y, float x) __asm__("atan2f");
// Per grab state, records the held spot/parameter in the player's safe-place table.
// Also known as: UpdateSafeInteract (upstream readable / Sly 1 name; UpdateSafeInteract__FP5ACTOR).
extern "C" void RecordJtGrabPlace(struct P *grab)
{
    float g, s;
    switch (grab->x8) {
    case 3:
        if (grab->x14->x19c)
            return;
        g = grab->x20;
        AddJtPlaceOnCurve(grab->x0->places, grab->x18, g);
        break;
    case 4:
        if (grab->x14->xec)
            return;
        g = 0.0f;
        AddJtPlaceOnCurve(grab->x0->places, grab->x18, g);
        break;
    case 0:
    case 13:
        if (IsBasicDerivedFrom__FP5BASIC3CID(grab->x14, 0xC)) {
            if (grab->x14->x19c)
                return;
            g = grab->x20;
            AddJtPlaceOnCurve(grab->x0->places, grab->x18, g);
            break;
        }
        if (IsBasicDerivedFrom__FP5BASIC3CID(grab->x14, 0x26)) {
            if (grab->x14->xec)
                return;
            g = 0.0f;
            AddJtPlaceOnCurve(grab->x0->places, grab->x18, g);
            break;
        }
        if (IsBasicDerivedFrom__FP5BASIC3CID(grab->x14, 0x22)) {
            if (grab->x14->xec)
                return;
            s = grab->x14->x10c;
        } else
            s = grab->x14->x184;
        {
            qword a;
            QWROW d;
            a = GetLoWorldPos(grab->x0);
            qword dq;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(dq) : "j"(a), "j"(GetLoWorldPos(grab->x14)));
            d.q = dq;
            g = atan2f(d.f[1], d.f[0]) * s;
        }
        AddJtPlaceOnCurve(grab->x0->places, grab->x18, g);
        break;
    default:
    {
        struct O *o = grab->x0;
        struct A *pa = o->x208;
        if (pa && pa->x88 > 0.7f && (pa->x4c->fl & 0x20000000) == 0 && !o->x108c)
            AddJtPlaceAtPosition(o->places, pa->x40, pa->x60);
        return;
    }
    }
}
} // namespace M_func_001942F0

/* The grab's record for its state and kind (unknown54/58/60/64); *pfFlip set in state 8 for kinds 9 and 10. */
// The adjust record for the grab kind and a flag.
// Also known as: IsoeInteractMesh (upstream readable / Sly 1 name; IsoeInteractMesh__FP5ACTORPi).
extern "C" void *GetJtGrabAdjustRecord(JTGRAB *grab, int *pfFlip)
{
    int fFlip = 0;
    void *record;
    switch (grab->state) {
    case 0:
        if (grab->kind == 5)
            record = grab->unknown60;
        else
            record = grab->unknown54;
        break;
    case 1: record = grab->unknown58; break;
    case 8:
        switch (grab->kind) {
        case 9:
        case 10:
            record = grab->unknown64;
            fFlip = 1;
            break;
        default: record = 0; break;
        }
        break;
    default: record = 0; break;
    }
    *pfFlip = fFlip;
    return record;
}

extern "C" {
extern int g_grfjoyt;
int IsCameraPolicyActive__FP5CPLCY(void *pcplcy);
}
// True when the grab may hand the camera to the player: the step player allows it, the free-camera option bit is set
// and no camera policy is active.
static inline int IsGrabCameraFree(JTGRAB *grab)
{
    int f = 0;
    if (grab->player->unknown12F0 && (g_grfjoyt & 2)) f = !IsCameraPolicyActive__FP5CPLCY((*(char (*)[])&g_cm.part3a0));
    return f;
}
/* Whether input n is allowed in the current grab state (n 3: only in states 0, 1, 3, 4 and 13 with the camera
   free; states 3 and 4 allow only 1, 2 and 0x1F; states 1, 8, 11 and 12 none). */
// Whether gadget/action k can be used in this grab kind (kinds 1/8/11/12 forbid; 3/4 allow ...).
// Also known as: FInteractCanDo (upstream readable / Sly 1 name; FInteractCanDo__FP5ACTORi).
extern "C" int CanJtUseGadgetInGrab(JTGRAB *grab, int n)
{
    if (n == 3) {
        switch (grab->state) {
        case 0: case 1: case 3: case 4: case 13:
            return IsGrabCameraFree(grab);
        }
        return 0;
    }
    switch (grab->state) {
    case 1: case 8: case 11: case 12:
        return 0;
    case 3: case 4: {
        int f = 0;
        if (n == 1 || n == 2 || n == 0x1F) f = 1;
        return f;
    }
    }
    return 1;
}

/* Whether the grab makes the player immune to zaps (state 12; state 8 with kinds 9 and 10). */
// True for grab kind 0xC, or kind 8 when the helper says so (the grab makes the player immune to zaps).
static inline int IsGrabFlipping(JTGRAB *grab) { return grab->kind == 9 || grab->kind == 10; }
// True for grab kind 0xC, or kind 8 when the helper says so.
extern "C" int IsJtGrabBusy(JTGRAB *grab)
{
    switch (grab->state) {
    case 8:
        if (IsGrabFlipping(grab))
            return 1;
        break;
    case 0xC:
        return 1;
    }
    return 0;

}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_AddJtGrabCandidate__FP2SWP10STEPPLAYERUxP2LOP8GRABSPOTiP10JTGRABCAND {
// AddJtGrabCandidate: ported from Lilly's personal Sly 2 decomp (src/P2/jtgrab.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF664 (D_002BF664), D_002E1E4C (D_002E1E4C), D_002E1E40 (g_SheetGangSly), g_clock
// 0x00194688 AddJtGrabCandidate__FP2SWP10STEPPLAYERUxP2LOP8GRABSPOTiP10JTGRABCAND  (MATCHES Sly 2; our name)
// Tests one grab spot against the hero (qwPosJt is the hero's position) and, when the hero may take hold of it, puts it into the
// sorted array of ccandMax candidates: by priority (higher first), then by distance, one entry per group (the nearer spot of a
// group wins). Objects the hero already holds / just let go of, spots whose mask the hero's flags do not share and objects
// farther than their own busyRadius or than 1000 are skipped.
qword PosLoWorld(LO *plo) __asm__("GetLoWorldPos");










void CopyAb(void *pvDst, void *pvSrc, uint cb) __asm__("CopyAb__FPvT0Ui");









// Length of v as a float.
static inline float LengthOf(qword v)
{
    return VuToFloat(VuLength(v));
}
extern void *memmove(void *pvDst, const void *pvSrc, size_t cb) __asm__("memmove");
extern JT *D_002E1E40 __asm__("D_002E1E40");
extern int D_002E1E4C __asm__("D_002E1E4C");   // C symbol: this view has no extern "C" block; defined at file scope below
extern float D_002BF664 __asm__("D_002BF664");
// True when the object's world position is farther from pos than its busy radius (busyRadius).
static inline int IsBeyondBusyRadius(LO *lo, qword pos)
{
    qword qwDiff = VuSub(PosLoWorld(lo), pos);
    return lo->busyRadius * lo->busyRadius < VuToFloat(VuDot(qwDiff, qwDiff));
}
// Zero a grab candidate.
static inline void ZeroJtGrabCand(JTGRABCAND *candidate)
{
    memset(candidate, 0, sizeof(JTGRABCAND));
}
// memmove grab candidates.
static inline void MoveJtGrabCands(JTGRABCAND *pcandDst, JTGRABCAND *pcandSrc, size_t cb)
{
    memmove(pcandDst, pcandSrc, cb);
}
void AddJtGrabCandidate(SW *psw, STEPPLAYER *pstepplayer, qword qwPosJt, LO *plo, GRABSPOT *pspot, int ccandMax, JTGRABCAND *ajtgrabcand) __asm__("AddJtGrabCandidate__FP2SWP10STEPPLAYERUxP2LOP8GRABSPOTiP10JTGRABCAND");
// Tests one grab spot against the player and inserts it into the candidate list if usable.
// Also known as: RateJt (upstream readable / Sly 1 name; RateJt__FP5ACTORP6VECTORG9VU_VECTORP2LOP5JTENTiP10JTGRABCAND).
void AddJtGrabCandidate(SW *world, STEPPLAYER *player, qword qwPosJt, LO *lo, GRABSPOT *spot, int ccandMax, JTGRABCAND *candidates)
{
    JTGRABCAND cand;
    if (lo->parent && IsBeyondBusyRadius(lo, qwPosJt))
        return;
    if (lo == player->jtgrab.grabbedObject)
        return;
    if (lo == player->jtair.jumpObject)
        return;
    if (spot->ploGroup && player->jtgrab.grabSpot && spot->ploGroup == player->jtgrab.grabSpot->ploGroup)
        return;
    if (lo == player->jtair.unknownBC)
        return;
    if (lo == (LO *)player->jtpipe.pipeCurve)
        return;
    if (player == (STEPPLAYER *)D_002E1E40)
    {
        if (lo == ((JT *)player)->hangHandle)
            return;
        if (lo == ((JT *)player)->unknown1F18)
            return;
    }
    if (!(player->grfGrabTarget & spot->grfPlayers))
        return;
    if (D_002E1E4C)
    {
        if (player->jtgrab.grabbedObject)
            return;
        if (player->jtpipe.pipeCurve)
            return;
        if (player == (STEPPLAYER *)D_002E1E40)
        {
            if (((JT *)player)->hangHandle)
                return;
            if (((JT *)player)->unknown1F18)
                return;
        }
    }
    spot->GetClosestParam(player, &cand.spotDistance);
    spot->GetGrabPos(cand.spotDistance, player, &cand.position, 0);
    cand.priority = spot->nPriority;
    cand.distance = LengthOf(VuSub(qwPosJt, cand.position));
    spot->AdjustScore(&cand.distance);
    if (cand.distance > D_002BF664)
        return;
    int i;
    if (spot->ploGroup)
    {
        for (i = 0; i < ccandMax; i++)
        {
            JTGRABCAND *candidate = &candidates[i];
            if (!candidate->grabSpot)
                break;
            if (candidate->grabSpot->ploGroup == spot->ploGroup)
            {
                if (cand.distance < candidates[i].distance)
                {
                    CopyAb(&candidates[i], &candidates[i + 1], (ccandMax - 1 - i) * sizeof(JTGRABCAND));
                    ZeroJtGrabCand(&candidates[ccandMax] - 1);
                    candidates[ccandMax - 1].distance = D_002BF664;
                }
                else
                    return;
                break;
            }
        }
    }
    i = ccandMax;
    do
    {
        JTGRABCAND *pcandPrev = &candidates[--i];
        if (pcandPrev->priority > cand.priority || (pcandPrev->priority == cand.priority && pcandPrev->distance < cand.distance))
        {
            i++;
            break;
        }
    } while (i != 0);
    if (i == ccandMax)
        return;
    MoveJtGrabCands(&candidates[i + 1], &candidates[i], (ccandMax - i) * sizeof(JTGRABCAND) - sizeof(JTGRABCAND));
    cand.object = lo;
    cand.grabSpot = spot;
    cand.unknown14 = g_clock.time;
    cand.unknown10 = 0;
    cand.dsParam = 0.0f;
    candidates[i] = cand;
}
} // namespace M_AddJtGrabCandidate__FP2SWP10STEPPLAYERUxP2LOP8GRABSPOTiP10JTGRABCAND

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi {
// GatherJtGrabCandidates: ported from Lilly's personal Sly 2 decomp (src/P2/jtgrab.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF660 (D_002BF660), D_002BF664 (D_002BF664), D_002BF70C (D_002BF70C), D_003DA0C0 (g_hintbox), g_sw
// 0x00194A40 GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi  (MATCHES Sly 2; our name for the record)
// Every frame (UpdateJtPre 0x18B7E8 asks with 0xFFFF; the others with a mask the current state allows): collects the hero's grab
// candidates. grf says which of the world's grab lists to walk: 1 the HNDs (found in the TARGET list), 2 PIPEs, 4 class 27, 8 HSHAPEs,
// 0x10 class 34, 0x20 class 38, 0x80 class 35, 0x200 RAILs, 0x400 class 39. 0x195368 and 0x194F70 then refine the four best, and the
// hint box (the popup at the top of the screen) shows the hint text of the best one's spot (nUnk14) while it is within 250 - or the
// text 0x125 for an object of oid 0x624 that has been the best for more than a second.

int IsBasicDerivedFrom(BASIC *pbasic, CID cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword PosLoWorld(LO *plo) __asm__("GetLoWorldPos");










void UpdateJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand) __asm__("UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND");
void EmitJtGrabSparkles(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand) __asm__("EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND");

















PROXY *PproxyFromLo(LO *plo) __asm__("GetProxyCopyValue");
// Length of v as a float.
static inline float LengthOf(qword v)
{
    return VuToFloat(VuLength(v));
}

int IsPoCurrent(STEPPLAYER *pstepplayer) __asm__("IsPoCurrent");


int GetHintBoxTextId(HINTBOX *phintbox) __asm__("GetCurrentNoticeTextId");


void PostNotice(HINTBOX *phintbox, int idText, float dt) __asm__("PostNotice");


void CancelNotice(HINTBOX *phintbox, int idText) __asm__("CancelNotice");

extern float D_002BF660 __asm__("D_002BF660");
extern float D_002BF664 __asm__("D_002BF664");
extern float D_002BF70C __asm__("D_002BF70C");   // C symbol: this view has no extern "C" block
extern HINTBOX D_003DA0C0 __asm__("D_003DA0C0");
void AddJtGrabCandidate(SW *psw, STEPPLAYER *pstepplayer, qword qwPosJt, LO *plo, GRABSPOT *pspot, int ccandMax, JTGRABCAND *ajtgrabcand) __asm__("AddJtGrabCandidate__FP2SWP10STEPPLAYERUxP2LOP8GRABSPOTiP10JTGRABCAND");
void GatherJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, uint grf) __asm__("GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi");
// Every frame collects the player's grab candidates from the world's grab lists.
// Also known as: UpdateJtCandidates (upstream readable / Sly 1 name; UpdateJtCandidates__FP5ACTORP10STEPPLAYERUi).
void GatherJtGrabCandidates(SW *world, STEPPLAYER *player, uint grf)
{
    JTGRABCAND candidates[4];
    if (!IsPoCurrent(player))
        return;
    memset(candidates, 0, sizeof(candidates));
    uint i = 0;
    do
    {
        candidates[i].priority = -1;
        candidates[i].distance = D_002BF664;
        i++;
    } while (i < 4);
    qword qwPosJt = PosLoWorld(player);
    if (grf & 8)
    {
        DL *pdlpcid12 = &g_sw.dlCid12;
        for (CID12 *pcid12 = (CID12 *)pdlpcid12->head; pcid12; pcid12 = (CID12 *)pcid12->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid12, pcid12, 4, candidates);
    }
    if (grf & 0x10)
    {
        DL *pdlpcid34 = &g_sw.dlCid34;
        for (CID34 *pcid34 = (CID34 *)pdlpcid34->head; pcid34; pcid34 = (CID34 *)pcid34->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid34, pcid34, 4, candidates);
    }
    if (grf & 0x80)
    {
        DL *pdlpcid35 = &g_sw.dlCid35;
        for (CID35 *pcid35 = (CID35 *)pdlpcid35->head; pcid35; pcid35 = (CID35 *)pcid35->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid35, pcid35, 4, candidates);
    }
    if (grf & 4)
    {
        DL *pdlpcid27 = &g_sw.dlCid27;
        for (CID27 *pcid27 = (CID27 *)pdlpcid27->head; pcid27; pcid27 = (CID27 *)pcid27->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid27, pcid27, 4, candidates);
    }
    if (grf & 0x400)
    {
        DL *pdlpcid39 = &g_sw.dlCid39;
        for (CID39 *pcid39 = (CID39 *)pdlpcid39->head; pcid39; pcid39 = (CID39 *)pcid39->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid39, pcid39, 4, candidates);
    }
    if (grf & 0x200)
    {
        DL *pdlpcid13 = &g_sw.dlCid13;
        for (RAIL *pcid13 = (RAIL *)pdlpcid13->head; pcid13; pcid13 = (RAIL *)pcid13->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid13, pcid13, 4, candidates);
    }
    if (grf & 2)
    {
        DL *pdlpcid11 = &g_sw.dlCid11;
        for (PIPE *pcid11 = (PIPE *)pdlpcid11->head; pcid11; pcid11 = (PIPE *)pcid11->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid11, pcid11, 4, candidates);
    }
    if (grf & 1)
    {
        DL *pdlTarget = &g_sw.dlTarget;
        for (TARGET *target = (TARGET *)pdlTarget->head; target; target = (TARGET *)target->targetListLink.next)
        {
            if ((target->targetKindFlags & 1) && IsBasicDerivedFrom(target, (CID)19))
                AddJtGrabCandidate(world, player, qwPosJt, target, (HND *)target, 4, candidates);
        }
    }
    if (grf & 0x20)
    {
        DL *pdlpcid38 = &g_sw.dlCid38;
        for (CID38 *pcid38 = (CID38 *)pdlpcid38->head; pcid38; pcid38 = (CID38 *)pcid38->dleGrab.next)
            AddJtGrabCandidate(world, player, qwPosJt, pcid38, pcid38, 4, candidates);
    }
    int ccand;
    for (ccand = 4; ccand > 0 && !candidates[ccand - 1].object; ccand--)
        ;
    SW *pswG = &g_sw;
    EmitJtGrabSparkles(pswG, player, ccand, candidates);
    UpdateJtGrabCandidates(pswG, player, ccand, candidates);
    int idHintOld = GetHintBoxTextId(&D_003DA0C0);
    if (idHintOld != -1 && idHintOld != pswG->idHintGrab)
        return;
    int idHint;
    int fFar;
    if (pswG->cjtgrabcand == 0)
        idHint = -1;
    else if (fFar = D_002BF660 < LengthOf(VuSub(qwPosJt, pswG->ajtgrabcand[0].position)), fFar)
        idHint = -1;
    else
    {
        PROXY *proxy = pswG->ajtgrabcand[0].unknown10 ? PproxyFromLo(pswG->ajtgrabcand[0].unknown10) : 0;
        if (proxy && proxy->oid == 0x624 && D_002BF70C < pswG->ajtgrabcand[0].unknown14)
            idHint = 0x125;
        else
            idHint = g_sw.ajtgrabcand[0].grabSpot->nUnk14;
    }
    if (idHint == idHintOld)
        return;
    if (idHint == -1)
        CancelNotice(&D_003DA0C0, g_sw.idHintGrab);
    else
        PostNotice(&D_003DA0C0, idHint, -1.0f);
    g_sw.idHintGrab = idHint;
}
} // namespace M_GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi

/* qsort order of grab candidates: nearer first. */
// qsort comparator by candidate score.
extern "C" int CompareJtGrabCandidates(JTGRABCAND *pcand0, JTGRABCAND *pcand1) { if (pcand0->distance < pcand1->distance) return -1; return 1; }

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND {
// MATCH (w3lane3, 2026-10-25): UpdateJtGrabCandidates = UpdateJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand)
// (name and structures from src/game/gameplay/player/grab/GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi.cpp, the GatherJtGrabCandidates port that calls it).
// Merges this frame's sorted grab candidates into the four the world keeps (SW::ajtgrabcand): an old candidate
// whose object is found again takes the new distance and smooths its parameter towards the new one (wrapping
// around the spot's range), one that is gone is dropped, and the remaining new ones are linked and appended.

struct SMP { float g0, g4, g8; };


int D_002E1E4C __asm__("D_002E1E4C");   /* .bss order: file-scope plain objects are emitted first, in first-declaration order */
int D_002E1E50 __asm__("D_002E1E50");
extern float D_002BF710 __asm__("D_002BF710");
extern float D_002BF714 __asm__("D_002BF714");
extern SMP D_002BF718 __asm__("D_002BF718");
int CompareJtGrabCandidates(const void *, const void *) __asm__("CompareJtGrabCandidates");   // JTGRABCAND compare for qsort
void qsort(void *base, uint c, uint cb, int (*pfnCmp)(const void *, const void *)) __asm__("qsort");   // qsort
void *memmove(void *dst, const void *src, uint cb) __asm__("memmove");   // memmove
float SmoothValue__FfffP3SMPPf(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext) __asm__("SmoothValue__FfffP3SMPPf");   // SmoothValue
void FadeSoOut(LO *plo, float g) __asm__("FadeSoOut");
void BroadcastGameEvent(void *pv, int msg, LO *plo) __asm__("BroadcastGameEvent");
LO *AcquireJtGrabMarker(SW *psw, ::PROXY *pproxy, JTGRABCAND *pcand) __asm__("AcquireJtGrabMarker");
void UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand) __asm__("UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND");


// Refines the best grab candidates (parameter and position on the spot, reachability).
// Also known as: TrackJt (upstream readable / Sly 1 name; TrackJt__FP5ACTORP5JTENTiP10JTGRABCAND).
void UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND(SW *world, STEPPLAYER *player, int ccand, JTGRABCAND *candidates)
{
    float asRange[2];
    qword pos;
    int i, j;
    JTGRABCAND *oldCandidate, *pcand;
    if (ccand > 0) {
        qsort(candidates, ccand, sizeof(JTGRABCAND), CompareJtGrabCandidates);
        if (candidates->grabSpot->nUnk14 != -1)
            candidates->priority = 1;
    }
    JTGRABCAND *ajtgrabcandOld = world->ajtgrabcand;
    for (i = world->cjtgrabcand; --i >= 0; ) {
        oldCandidate = &world->ajtgrabcand[i];
        // Search the new candidates from the end; the drop code sits in the loop's exit test (its calls stop gcc
        // from duplicating the test in front of the loop, giving the original's jump-to-test entry).
        j = ccand;
        for (;;) {
            if (--j < 0) {
                // Not seen this frame: drop it.
                if (oldCandidate->unknown10)
                    FadeSoOut(oldCandidate->unknown10, 0.5f);
                memmove(oldCandidate, oldCandidate + 1, (3 - i) * (int)sizeof(JTGRABCAND));
                world->cjtgrabcand--;
                break;
            }
            pcand = &candidates[j];
            if (oldCandidate->object == pcand->object) {
                oldCandidate->distance = pcand->distance;
                if (!oldCandidate->unknown10) {
                    oldCandidate->spotDistance = pcand->spotDistance;
                    oldCandidate->dsParam = 0.0f;
                } else {
                    float sRange;
                    int fWrap = pcand->grabSpot->GetGrabRange(asRange);
                    if (fWrap) {
                        float ds;
                        sRange = asRange[1] - asRange[0];
                        float sNew = pcand->spotDistance;
                        float sOld = oldCandidate->spotDistance;
                        ds = sNew - sOld;
                        // Move the new parameter to the wrap of the range closest to the old one.
                        if (sRange * 0.5f < ds) {
                            ds -= sRange;
                            pcand->spotDistance = sOld + ds;
                        } else if (ds < sRange * -0.5f) {
                            ds += sRange;
                            pcand->spotDistance = sOld + ds;
                        } else {
                            fWrap = 0;
                        }
                    }
                    if (D_002E1E50) {
                        float dt = g_clock.deltaTime;
                        float dsPrev = oldCandidate->dsParam;
                        float dsNew = dsPrev + dt * ((pcand->spotDistance - oldCandidate->spotDistance) * D_002BF710 + (pcand->dsParam - dsPrev) * D_002BF714);
                        float dsAvg;
                        oldCandidate->dsParam = dsNew;
                        dsAvg = (dsPrev + dsNew) * 0.5f;
                        oldCandidate->spotDistance = oldCandidate->spotDistance + dt * dsAvg;
                    } else {
                        oldCandidate->spotDistance = SmoothValue__FfffP3SMPPf(oldCandidate->spotDistance, pcand->spotDistance, g_clock.deltaTime, &D_002BF718, &oldCandidate->dsParam);
                    }
                    if (fWrap) {
                        if (asRange[1] < oldCandidate->spotDistance)
                            oldCandidate->spotDistance = oldCandidate->spotDistance - sRange;
                        else if (oldCandidate->spotDistance < asRange[0])
                            oldCandidate->spotDistance = oldCandidate->spotDistance + sRange;
                    }
                    GRABSPOT *spot = oldCandidate->grabSpot;
                    spot->GetGrabPos(oldCandidate->spotDistance, player, &pos, 0);
                    AsLo(oldCandidate->unknown10)->TranslateToPos(pos);
                }
                memmove(pcand, pcand + 1, (ccand - j) * (int)sizeof(JTGRABCAND) - (int)sizeof(JTGRABCAND));
                ccand--;
                break;
            }
        }
    }
    for (j = 0; j < ccand; j++) {
        pcand = &candidates[j];
        if (player->jtgrab.pproxyMarker && pcand->priority == 1) {
            register LO *plo asm("$6"); // the one register pin: the original keeps the link result in a2, global-alloc would pick v0
            plo = AcquireJtGrabMarker(world, player->jtgrab.pproxyMarker, pcand);
            if (!plo)
                break;
            pcand->unknown10 = plo;
        }
        world->ajtgrabcand[world->cjtgrabcand++] = *pcand;
        BroadcastGameEvent(&g_game, 0x3D, pcand->object);
    }
    qsort(ajtgrabcandOld, world->cjtgrabcand, sizeof(JTGRABCAND), CompareJtGrabCandidates);
}
} // namespace M_UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND {
// EmitJtGrabSparkles = EmitJtGrabSparkles(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand)
// (name and JTGRABCAND layout from src/game/gameplay/player/grab/GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi.cpp / UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND.cpp; psw is unused).
// For every grab candidate that has a spot, emits a random number of sparkles (emitter rate * dt, the fraction
// rounded up with that probability): each one samples a position and velocity from the grab spot, scales the
// velocity, picks a birth time in the past and optionally pushes the position along the velocity. At most ten are
// collected, then handed to the emitter's particle system in one batch.

struct VU_FLOAT
{
    qword q;
    // Uninitialised VU float.
    VU_FLOAT() {}
    // By reference: at loop time the bound temporary adds copies (keeps the inner loop at loop.c's size), and by
    // sched1 combine has folded the call-value copy into the asm input, so the qmtc2 reads $f0 directly.
    VU_FLOAT(const float &g) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(g)); }
};
struct VU_VECTOR
{
    qword q;
    // Uninitialised vector.
    VU_VECTOR() {}
    // Wrap a qword.
    VU_VECTOR(qword a) { q = a; }
    // Wrap a VECTOR.
    VU_VECTOR(const QWROW &v) { q = v.q; }
};


float RandomFloatInRange__Fff(float gMin, float gMax) __asm__("RandomFloatInRange__Fff");                    // RandomFloatInRange
void *func_002333B4(void *pv, int b, unsigned int cb) __asm__("memset");          // memset
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(EMITB *pemitb, EMITG *pemitg, int cpart, QWROW *apos, QWROW *av, float *atBirth, float *atDeath,
                   void *pv1, void *pv2) __asm__("EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3");
void EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand) __asm__("EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND");

void OnEmitterValuesChanged(EMITTER *pemitter) __asm__("OnEmitterValuesChanged__FP7EMITTER");

// Random float in the range [minValue, maxValue].
static inline float RandomInRange(LM *range) { return RandomFloatInRange__Fff(range->minValue, range->maxValue); }
// Return v scaled by g.
static inline VU_VECTOR operator*(VU_FLOAT g, VU_VECTOR v)
{
    __asm__("vmulx.xyzw %0,%0,%1x" : "+j"(v.q) : "j"(g.q));
    return v;
}
// a + b * g
static inline VU_VECTOR AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT g)
{
    __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x" : "+j"(a.q) : "j"(b.q), "j"(g.q));
    return a;
}
// Ask the candidate's grab spot for its sparkle position and direction.
static inline void GetSparkle(JTGRABCAND *candidate, QWROW *posOut, QWROW *pv)
{
    candidate->grabSpot->GetSparklePos(&posOut->q, &pv->q, candidate->spotDistance);
}

// For every candidate spawns/refreshes the grab-hint sparkle effect.
// Also known as: SprayJtSparkles (upstream readable / Sly 1 name; SprayJtSparkles__FP5ACTORP5JTENTiP10JTGRABCAND).
void EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND(SW *world, STEPPLAYER *player, int ccand,
                                                          JTGRABCAND *candidates)
{
    QWROW apos[10], av[10];
    float atBirth[10], atDeath[10];
    EMITTER *pemitter = player->pemitterGrab;
    EMITVEL *pemitp;
    int cpart;
    int icand;

    if (!pemitter)
        return;
    if (pemitter->fDirty)
        OnEmitterValuesChanged(pemitter);
    pemitp = &pemitter->pemitb->vel;

    cpart = 0;
    for (icand = 0; icand < ccand; icand++) {
        JTGRABCAND *candidate = &candidates[icand];
        float g, gRand;
        int c;

        if (candidate->priority == -1)
            continue;
        g = pemitter->rate.maxValue * g_clock.deltaTime;
        gRand = RandomFloatInRange__Fff(0.0f, 1.0f);
        c = (int)g;
        g -= (float)c;
        if (gRand < g)
            c++;

        while (--c >= 0) {
            QWROW pos, v, vScaled;
            VU_FLOAT s;

            GetSparkle(candidate, &pos, &v);
            s = VU_FLOAT(RandomInRange(&pemitp->lmSpeed));
            VU_VECTOR vs = s * v;
            vScaled.q = vs.q;
            float tBirth = g_clock.time;
            tBirth -= RandomInRange(&pemitp->lmAge);
            atBirth[cpart] = tBirth;
            atDeath[cpart] = tBirth + pemitp->lifetime;
            if (pemitp->lmPush.minValue != pemitp->lmPush.maxValue) {
                VU_VECTOR posOld = pos;
                s = VU_FLOAT(RandomInRange(&pemitp->lmPush));
                pos.q = AddScaled(posOld, vScaled, s).q;
            }
            apos[cpart] = pos;
            av[cpart] = vScaled;
            cpart++;
            if (cpart >= sizeof(atBirth) / sizeof(atBirth[0]))
                goto LEmit;
        }
    }

LEmit:
    if (cpart > 0) {
        EMITG emitg;
        func_002333B4(&emitg, 0, sizeof(emitg));
        emitg.ppblipg = &pemitter->pblipg;
        emitg.ploSubscriber = pemitter;
        EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(pemitter->pemitb, &emitg, cpart, apos, av, atBirth, atDeath, 0,
                                                      0);
    }
}
} // namespace M_EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND

/* Moves the world's grab marker (SW::unknown4744) onto the candidate for plo: acquires a new marker there and
   removes the candidate's old one (LO slot 9). */
// Splice entry: for the candidate with the id, acquires a new marker proxy and replaces the old one.
// Also known as: ShowJtAura (upstream readable / Sly 1 name; ShowJtAura__FP2SWi).
extern "C" void SetGrabMarkerProxy(SW *world, LO *lo)
{
    JTGRABCAND *candidate;
    LO *ploMarker;
    int i;
    if (!lo || !world->unknown4744)
        return;
    for (i = 0; i < world->cjtgrabcand; i++) {
        JTGRABCAND *pcandI = &world->ajtgrabcand[i];
        if (lo == pcandI->object) {
            candidate = pcandI;
            break;
        }
    }
    if (!candidate)
        return;
    ploMarker = AcquireJtGrabMarker(world, world->unknown4744, candidate);
    if (!ploMarker)
        return;
    if (candidate->unknown10) {
        LO *ploOld = candidate->unknown10;
        ploOld->Remove();
    }
    candidate->unknown10 = ploMarker;
}

/* A grab marker proxy of plo placed at the candidate: unparented, shown (LO slot 9), faded in, moved to the
   candidate's position (slot 29) with matrix D_002A2D00 (slot 30) and zero velocities (slots 35, 36). */
// Acquires a marker proxy, unparents it, fades it in over 0.25 s and places it at the candidate.
// Also known as: PloNewJtAura (upstream readable / Sly 1 name; PloNewJtAura__FP2LOiP5JTENT).
extern "C" LO *AcquireJtGrabMarker(SW *world, LO *lo, JTGRABCAND *candidate)
{
    LO *ploMarker;
    if (!lo)
        return 0;
    ploMarker = AcquireProxyObject(lo, 0);
    if (ploMarker) {
        SetLoParent(ploMarker, 0, 0);
        ploMarker->Remove();
        FadeSoIn(ploMarker, 0.25f);
        ploMarker->TranslateToPos(candidate->position);
        ploMarker->RotateToMat((MATRIX3 *)D_002A2D00);
        ploMarker->SetVelocity(D_002A2DD0);
        ploMarker->SetAngularVelocity(D_002A2DD0);
    }
    return ploMarker;
}

/* The world's grab marker object (oid 0x624). */
// World post-load: finds the grab marker template (oid 0x624) into g_sw+0x4744.
extern "C" void PostLoadGrabMarker(SW *world)
{
    world->unknown4744 = FindSwObject(world, 0x405, (OID)0x624, 0);
}

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.

// ---- grab's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
namespace M_UpdateJtGrabInput {
float D_002BF5B0 __asm__("D_002BF5B0") = 200.0f;
float D_002BF5B4 __asm__("D_002BF5B4") = 25.0f;
float D_002BF5B8 __asm__("D_002BF5B8") = 0.35f;
} // namespace M_UpdateJtGrabInput extern "C"
namespace M_UpdateJtActiveHide {
float D_002BF5BC __asm__("D_002BF5BC") = 1.5f;
float D_002BF5C0 __asm__("D_002BF5C0") = 3.0f;
float D_002BF5C4 __asm__("D_002BF5C4") = 0.75f;
float D_002BF5C8 __asm__("D_002BF5C8") = 0.2f;
float D_002BF5CC __asm__("D_002BF5CC") = 55.0f;
U D_002BF5D0[3] __asm__("D_002BF5D0") = {{{0.0f, 0.0f, -50.0f, 0.0f}}, {{45.0f, -40.0f, -50.0f, 0.0f}}, {{45.0f, 40.0f, -50.0f, 0.0f}}};
U D_002BF600[3] __asm__("D_002BF600") = {{{0.0f, 0.0f, -15.0f, 0.0f}}, {{45.0f, -40.0f, -15.0f, 0.0f}}, {{45.0f, 40.0f, -15.0f, 0.0f}}};
HEIGHT D_002BF630 __asm__("D_002BF630") = {-50.0f, 0.0f};
} // namespace M_UpdateJtActiveHide extern "C"
namespace M_func_00192A28 {
float D_002BF638 __asm__("D_002BF638") = 1500.0f;
} // namespace M_func_00192A28 extern "C"
namespace M_SetJtGrabKind {
float D_002BF63C __asm__("D_002BF63C") = -0.2f;
float D_002BF640 __asm__("D_002BF640") = 0.5f;
} // namespace M_SetJtGrabKind extern "C"
VECTOR4 D_002BF650 = {225.0f, 0.0f, 125.0f, 0.0f};
namespace M_FJtGrabNearestCandidate {
float D_002BF660 __asm__("D_002BF660") = 250.0f;
float D_002BF664 __asm__("D_002BF664") = 1000.0f;
} // namespace M_FJtGrabNearestCandidate extern "C"
namespace M_GetJtRailLanding {
float D_002BF668 __asm__("D_002BF668") = 0.2f;
/* A camera-parameter record (CPD layout, as tn's default D_002C4120 with its lmTilt: cid 0x30, offsets, limits,
 * weights) that ResetJtGrabTnParams points a target at (+0x114); 0x90 bytes, 16-aligned. */
namespace GRAB_DATA { struct CPDINIT { int cid; float ag[35]; };
extern CPDINIT r_D_002BF670 __asm__("D_002BF670") __attribute__((aligned(16))); }
GRAB_DATA::CPDINIT GRAB_DATA::r_D_002BF670 = {0x30, {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -225.0f,
    1.0f, 0.0f, 0.0f, 20.0f, 1.0f, 0.0f, 0.0f, -3.40282347e+38f,
    3.40282347e+38f, -3.40282347e+38f, 3.40282347e+38f, -3.40282347e+38f, 3.40282347e+38f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -3.40282347e+38f, 3.40282347e+38f}};
} // namespace M_GetJtRailLanding extern "C"
namespace M_PresetJtAccelHide {
struct SMP D_002BF700 __asm__("D_002BF700") = {2500.0f, 0.0f, 1.0f};
} // namespace M_PresetJtAccelHide extern "C"
namespace M_GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi {
extern float D_002BF70C __asm__("D_002BF70C") = 1.0f;
} // namespace M_GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi
namespace M_UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND {
float D_002BF710 __asm__("D_002BF710") = 15.0f;
float D_002BF714 __asm__("D_002BF714") = 5.0f;
SMP D_002BF718 __asm__("D_002BF718") = {1500.0f, 0.0f, 2.0f};
}
