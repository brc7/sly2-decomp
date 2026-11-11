// bentley.cpp -- Bentley (BENTLEY : STEPPLAYER, players.h): init/load, state machine and states, darts and aim,
// messages, player states, gadgets and pockets, the blend set (BTBLENDS) and the camera definition.
// TU 0x134648..0x1378D8 (48 functions, address order).
// Entry points: InitBentley/LoadBentleyFromBrx/PostBentleyLoad, UpdateBentley (per frame), SetBentleyState,
// HandleBentleyMessage, the dart aim (UpdateBentleyAim, FireBentleyDart), the gadget slots (CanBentleyUseGadget,
// StartBentleyGadget) and the STEPPLAYER virtual overrides (slots 50..105).
//
// Types: players.h BENTLEY/BTBLENDS, jt.h JTPOCKET, cm.h CPDEFI; the 11 m2c bodies are written on them (two words
// players.h leaves unnamed through macros). The other functions keep the per-function views they were matched
// with, each in a namespace named after it (as vec.cpp and mark.cpp do).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bentley
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bentley
#include "sly2/basic.h"
#include "sly2/cm.h"
#include "sly2/jt.h"
#include "sly2/players.h"
#include "sly2/slotheap.h"
#include "sly2/classes.h"
#include "sly2/act.h"
#include "player_input.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/game.h"
#include "sly2/clock.h"
#include "sly2/sw.h"

// Bentley's state (BENTLEY +0x13D0), switched by SetBentleyState and run by UpdateBentleyStateMachine. Meanings from
// docs/systems/player.md section 4 except where marked.
enum BentleyState {
    BENTLEY_IDLE = 0,
    BENTLEY_RUN = 1,              // run button held
    BENTLEY_AIR = 2,              // JTAIR
    BENTLEY_GRAB = 3,             // JTGRAB
    BENTLEY_HURT = 4,             // JTHURT
    BENTLEY_NEAR_TARGET = 5,      // next to an interact target
    BENTLEY_GADGET = 6,           // gadget use (sub-state at +0x16B0)
    BENTLEY_WALK = 7,             // full stick without the run button
    BENTLEY_HOVER = 8,            // Hover Pack (machine at +0x1800)
    BENTLEY_PLAYER_STATE_4 = 9,   // entered for generic player state 4 (inferred; meaning unknown)
    BENTLEY_SCRIPT_ANIM = 10,     // scripted animation, generic player state 5 (inferred)
    BENTLEY_SCRIPT_SMA = 11,      // scripted state-machine animation, generic player state 6 (inferred)
    BENTLEY_THROW = 12,           // JTTHROW: aiming/holding a Trigger Bomb; also generic player state 7
    BENTLEY_PIPE = 13,            // JTPIPE
    BENTLEY_ADRENALINE = 14,      // timed clip for the Adrenaline Burst gadget
};

// 0x137780 StaticInit_bentley and GlobalConstructors_bentley: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The contact friction field (engine/physics/mark.cpp's XP). */
struct XP { char pad0[0x94]; float gFriction; };

extern "C" {
/* Bentley's snip table (Sly 1 SNIP {grfsnip, oid, ib}, BindLoOidTable with a count of 0x76), bentley's first .data. */
struct SNIP { int grfsnip; int oid; int ib; };
extern SNIP D_002BC390[0x76];
extern const char D_002A4AE8[];   /* defined below where the ELF has it (.rodata order) */
extern float D_002BC9B4;   /* defined below, in address order */
void LoadStepFromBrx(STEP *pstep, CBinaryInputStream *pbis);
void BindLoOidTable(LO *plo, int c, void *aoid);
void GetStepplayerCpdefi(STEPPLAYER *pstepplayer, CPDEFI *pcpdefi);
void GetJtPipeVelocity(JTPIPE *pjtpipe, CPDEFI *pcpdefi);
int IsThrowableGadget(LO *plo);
void SetGadgetRecordObject(BENTLEY *pbentley, JTPOCKET *ppocket, LO *plo);
void UnlockProxyObject(PROXY *pproxy, LO *plo);
void SetBentleyCrosshairState(BTBLENDS *pbtblends, int n);
void OnStepplayerGadgetsChanged(STEPPLAYER *pstepplayer);
int IsPoCurrent(PO *ppo);
int GetGadgetButtonMask(STEPPLAYER *pstepplayer, int n);
void ShowSoGlobsById(SO *pso, int id);
void HideSoGlobsById(SO *pso, int id);
void FaceStepToward(STEP *pstep, LO *plo);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_InitBentley {
BENTLEY *D_002DD5BC __asm__("D_002DD5BC");   /* the Bentley object (bentley's .bss; medium: by layout) */
void InitStepplayer(BENTLEY *p) __asm__("InitStepplayer");
void InitGait(GAIT *pv, BENTLEY *p, int n) __asm__("InitGait");
// BENTLEY Init (vtable slot 7): STEPPLAYER init, then Bentley's defaults: camera offsets 45/125, the two gait records
// (+0x1320/+0x1600) and his gadget slot kinds (7/9/11 ...).
// Also known as: InitBentley (upstream readable / Sly 1 name; InitBentley__FP7BENTLEY).
extern "C" void InitBentley(BENTLEY *bentley)
{
    InitStepplayer(bentley);
//{
    bentley->unknown1040 = 0;
    bentley->state = -1;
    bentley->subState = -1;
    bentley->aimState = -1;
    bentley->hoverState = -1;
    bentley->grabRadius = 45.0f;
    bentley->unknown1048 = 125.0f;
//}
    InitGait(&bentley->gaitA, bentley, 0x73);
    InitGait(&bentley->gaitB, bentley, 0x73);
    bentley->jtair.unknown118 = -70.0f;
    bentley->waterProbe[0] = -100.0f;
    bentley->waterProbe[1] = 100.0f;
    bentley->waterProbe[2] = 50.0f;
    bentley->waterProbe[3] = -10.0f;
    {
        SMPA *ps = &bentley->unknown10A8;
        ps->fastSpeed = 250.0f;
        ps->fastDuration = 1.0f;
        ps->slowSpeed = 0.0f;
        ps->maxSpeedChange = 4500.0f;
    }
    bentley->grfGrabTarget = 2;
    bentley->apocket[1].idGadget = 7;
    bentley->apocket[2].idGadget = 9;
    bentley->apocket[3].idGadget = 11;
    bentley->apocket[4].idGadget = 13;
    bentley->apocket[5].idGadget = 4;
    bentley->apocket[0].idGadget = 5;
    bentley->unknown1870 = 5;
    D_002DD5BC = bentley;
}
} // namespace M_InitBentley

// BENTLEY LoadFromBrx (slot 14): STEP base, then binds his class-specific object table (0x76 entries).
// Layout parse-verified: a parser following it reads every byte of 4 levels.
extern "C" void LoadBentleyFromBrx(BENTLEY *bentley, CBinaryInputStream *pbis)
{
    LoadStepFromBrx(bentley, pbis);
    BindLoOidTable(bentley, 0x76, D_002BC390);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PostBentleyLoad {
typedef int ti128 __attribute__((mode(TI)));
union VQ { float af[4]; ti128 q; };   /* a 16-byte vector that can be initialized, read as a quadword (.q) */
extern VQ D_002BC920 __asm__("D_002BC920");
void PostLoadStepplayer(BENTLEY *) __asm__("PostLoadStepplayer");
void *FindSoXf(BENTLEY *, int, int) __asm__("FindSoXf");
void InitDirectionalBlend(BENTLEY *, void *, void *) __asm__("InitDirectionalBlend");
void PostJtLoadGait(void *) __asm__("PostJtLoadGait");
void InitBentleyCrosshair(void *, BENTLEY *, ti128, float, float) __asm__("InitBentleyCrosshair");
void AttachStepplayerToStartMarker(BENTLEY *, int) __asm__("AttachStepplayerToStartMarker");
void SetBentleyState(BENTLEY *, int) __asm__("SetBentleyState");
// BENTLEY post-load (slot 15): STEPPLAYER post-load, finds child SO 0xB, binds the animation blends, initializes the
// crosshair (InitBentleyCrosshair) and attaches him to his start marker.
// Also known as: PostLoadBentley (upstream readable / Sly 1 name; PostLoadBentley__FP7BENTLEY).
extern "C" void PostBentleyLoad(BENTLEY *bentley) {
    PostLoadStepplayer(bentley);
    bentley->unknown10B8 = FindSoXf(bentley, 0xb, 1);
    InitDirectionalBlend(bentley, bentley->abldirA, &bentley->pasegblA);
    InitDirectionalBlend(bentley, bentley->abldirB, &bentley->pasegblB);
    PostJtLoadGait(&bentley->gaitA);
    PostJtLoadGait(&bentley->gaitB);
    InitBentleyCrosshair(&bentley->btblends, bentley, D_002BC920.q, -50.0f, 50.0f);
    AttachStepplayerToStartMarker(bentley, 0x16);
    SetBentleyState(bentley, BENTLEY_IDLE);
}
} // namespace M_PostBentleyLoad

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateBentleyStateMachine {
extern float D_002BC930 __asm__("D_002BC930");
void SetJtAirJump(JTAIR *pv, int a, int b, int c) __asm__("SetJtAirJump");
int IsStepplayerNearInteractTarget(BENTLEY *p, int n) __asm__("IsStepplayerNearInteractTarget");
void SetBentleyState(BENTLEY *p, int n) __asm__("SetBentleyState");
void UpdateJtAirInput(JTAIR *pv) __asm__("UpdateJtAirInput");
void UpdateJtGrabInput(JTGRAB *pv) __asm__("UpdateJtGrabInput");
void UpdateJtHurtInput(JTHURT *pv) __asm__("UpdateJtHurtInput");
void StartJtMode(JTMODE *pv, int a, int b, int c) __asm__("StartJtMode");
void TryGrabJtNearestCandidate(JTGRAB *pv, int a, int b) __asm__("TryGrabJtNearestCandidate");
JTPOCKET *GetBentleyGadgetFromId(BENTLEY *p, int n) __asm__("GetBentleyGadgetFromId");
void UpdateJtActivePipe(JTPIPE *pv) __asm__("UpdateJtActivePipe");
void UpdateBentleySubState(BENTLEY *p) __asm__("UpdateBentleySubState");
void UpdateBentleyStateMisc(BENTLEY *p) __asm__("UpdateBentleyStateMisc");
void UpdateJtThrow(JTTHROW *pv) __asm__("UpdateJtThrow");
int IsAsegaRetracted(ASEGA *pv) __asm__("IsAsegaRetracted");
int IsSmaRetracted(SMA *pv) __asm__("IsSmaRetracted");
void UpdateBentleyAim(BENTLEY *p) __asm__("UpdateBentleyAim");

// Per-frame Bentley state transitions driven by the component states, then the sub-machines (sub-state,
// aim, hover).
// Also known as: UpdateBentleyTransitions (upstream readable / Sly 1 name; UpdateBentleyTransitions__FP7BENTLEY).
extern "C" void UpdateBentleyStateMachine(BENTLEY *p)
{
    int s;
    do {
        s = p->state;
        switch (s) {
        case BENTLEY_AIR:
        case BENTLEY_HURT:
        case BENTLEY_GADGET:
        case BENTLEY_HOVER:
        case BENTLEY_PLAYER_STATE_4:
        case BENTLEY_SCRIPT_ANIM:
        case BENTLEY_SCRIPT_SMA:
        case BENTLEY_PIPE:
        case BENTLEY_ADRENALINE:
            break;
        case BENTLEY_GRAB:
            if (p->jtgrab.state == 1 || p->jtgrab.state == 12)
                break;
            if (0.1f < g_clock.time - p->unknown20C)
                SetJtAirJump(&p->jtair, 0, 0, 0);
            break;
        case BENTLEY_NEAR_TARGET:
        case BENTLEY_WALK:
        case BENTLEY_THROW:
        default:
            if (0.1f < g_clock.time - p->unknown20C)
                SetJtAirJump(&p->jtair, 0, 0, 0);
            break;
        }
        switch (p->state) {
        case BENTLEY_IDLE:
        case BENTLEY_RUN:
        case BENTLEY_WALK:
            if (!(p->controller->buttonsHeld & BUTTON_RUN) && IsStepplayerNearInteractTarget(p, 0)) {
                SetBentleyState(p, BENTLEY_NEAR_TARGET);
            } else if (p->controller->ConsumeButtonsPressedWithin(BUTTON_JUMP, 0.1f)) {
                SetJtAirJump(&p->jtair, 1, 0, 0);
            } else if (p->controller->GetButtonsPressedWithin(BUTTON_LOOK, 0.15f) && p->CanUseGadget(3)) {
                p->controller->ClearJoyPressed(BUTTON_LOOK);
                StartJtMode(&p->jtmode, 0, 0, 0);
            } else if (p->controller->GetButtonsPressedWithin(BUTTON_ALT_ATTACK, 0.15f) && p->CanUseGadget(p->unknown1870)) {
                p->controller->ClearJoyPressed(BUTTON_ALT_ATTACK);
                p->ppocketCur = GetBentleyGadgetFromId(p, p->unknown1870);
                SetBentleyState(p, BENTLEY_GADGET);
            } else if (p->controller->ConsumeButtonsPressedWithin(BUTTON_ACTION, 0.2f)) {
                TryGrabJtNearestCandidate(&p->jtgrab, 0xF8FE, 0);
            } else {
                float g = p->controller->leftStickDeflection;
                if ((p->state == BENTLEY_IDLE && 0.1f < g) || (p->state != BENTLEY_IDLE && 0.0f < g)) {
                    if (p->controller->buttonsHeld & BUTTON_RUN)
                        SetBentleyState(p, BENTLEY_RUN);
                    else
                        SetBentleyState(p, BENTLEY_WALK);
                } else {
                    SetBentleyState(p, BENTLEY_IDLE);
                }
            }
            break;
        case BENTLEY_AIR:
            UpdateJtAirInput(&p->jtair);
            if (p->controller->GetButtonsPressedWithin(BUTTON_ALT_ATTACK, 0.15f) && p->CanUseGadget(p->unknown1870)) {
                p->controller->ClearJoyPressed(BUTTON_ALT_ATTACK);
                p->ppocketCur = GetBentleyGadgetFromId(p, p->unknown1870);
                SetBentleyState(p, BENTLEY_GADGET);
            }
            break;
        case BENTLEY_GRAB:
            UpdateJtGrabInput(&p->jtgrab);
            break;
        case BENTLEY_HURT:
            UpdateJtHurtInput(&p->jthurt);
            break;
        case BENTLEY_NEAR_TARGET:
            if ((p->controller->buttonsHeld & BUTTON_RUN) || !IsStepplayerNearInteractTarget(p, 1)) {
                SetBentleyState(p, BENTLEY_IDLE);
            } else if (p->controller->ConsumeButtonsPressedWithin(BUTTON_JUMP, 0.1f)) {
                SetJtAirJump(&p->jtair, 1, 0, 0);
            } else if (p->controller->GetButtonsPressedWithin(BUTTON_LOOK, 0.15f) && p->CanUseGadget(3)) {
                p->controller->ClearJoyPressed(BUTTON_LOOK);
                StartJtMode(&p->jtmode, 0, 0, 0);
            } else if (p->controller->ConsumeButtonsPressedWithin(BUTTON_ACTION, 0.2f)) {
                TryGrabJtNearestCandidate(&p->jtgrab, 0xF8FE, 0);
            } else if (p->controller->GetButtonsPressedWithin(BUTTON_ALT_ATTACK, 0.15f) && p->CanUseGadget(p->unknown1870)) {
                p->controller->ClearJoyPressed(BUTTON_ALT_ATTACK);
                p->ppocketCur = GetBentleyGadgetFromId(p, p->unknown1870);
                SetBentleyState(p, BENTLEY_GADGET);
            }
            break;
        case BENTLEY_PIPE:
            UpdateJtActivePipe(&p->jtpipe);
            break;
        case BENTLEY_GADGET:
            UpdateBentleySubState(p);
            break;
        case BENTLEY_HOVER:
            UpdateBentleyStateMisc(p);
            break;
        case BENTLEY_THROW:
            UpdateJtThrow(&p->jtthrow);
            break;
        case BENTLEY_SCRIPT_ANIM:
            if (p->activeAnimation && IsAsegaRetracted(p->activeAnimation))
                SetBentleyState(p, BENTLEY_IDLE);
            else if (p->activeStateMachine && IsSmaRetracted(p->activeStateMachine))
                SetBentleyState(p, BENTLEY_IDLE);
            break;
        case BENTLEY_ADRENALINE:
            if (D_002BC930 < g_clock.time - p->stateTime)
                SetBentleyState(p, BENTLEY_IDLE);
            break;
        }
        UpdateBentleyAim(p);
    } while (p->state != s);
}
} // namespace M_UpdateBentleyStateMachine

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00134D30 {
int IsStepplayerInJtMode(BENTLEY *) __asm__("IsStepplayerInJtMode");
int ConsumeButtonsPressedWithin__5POCTLUif(void *, int, float) __asm__("ConsumeButtonsPressedWithin__5POCTLUif");
void EndJtMode(void *) __asm__("EndJtMode");
void UpdateJtMode(void *) __asm__("UpdateJtMode");
// While in a look mode: in recon (mode 1, kind 0) the face buttons (mask 0x4F0) end it; then updates the mode.
// Also known as: UpdateBentleyBinoc (upstream readable / Sly 1 name; UpdateBentleyBinoc__FP7BENTLEY).
extern "C" void UpdateBentleyJtMode(BENTLEY *bentley) {
    if (!IsStepplayerInJtMode(bentley))
        return;
    void *pv = &bentley->jtmode;
    if (bentley->jtmode.state == 1 && bentley->jtmode.kind == 0 && ConsumeButtonsPressedWithin__5POCTLUif(bentley->controller, BUTTON_FACE | BUTTON_LOOK, 0.25f)) EndJtMode(pv);
    UpdateJtMode(pv);
}
} // namespace M_func_00134D30

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateBentley {
/* MATCH (Claude agent laneB18, 2026-10-23): case 5 uses LenXY5, which adds allocation-only "r"(p150) on the vsqrt asm and two "r"(p1030) on the qmfc2 asm (one "r"(p1030): 5). One register pin (MulT vf3). */
/* v6 (Claude agent laneB16, 2026-10-23): aligned 7 -> 1. Case 11 uses PullB: qmtc2 into a plain qword t, then VOLATILE stores p->v190 = r and u.q = t (keeps the v190 store before the u store in sched1, so u lives longer and local-alloc gives r vf1 / u vf2 like the target; the lw p1030 now hoists too). Cases 10/14 keep Pull (their target order differs). Left: case 5 lw p150 before the LenXY vmul. */
/* v5 (Claude agent laneB11, 2026-10-23): aligned 16 -> 7. MulT: r pinned to vf3 (one commented pin) + inputs (r[1], r[2]) order. Left: case 5 LenXY lw p150 after vaddy (target before vmul), case 11 Pull: lw p1030 / u vs r VF swap (vf1/vf2) and sqc2 order. */
/* v4 (Claude agent, laneB5, 2026-10-23): aligned 17 -> 16. LenXY as asm1 = vmul.xy+vaddy.x fused, asm2 = vsqrt+vwaitq+vaddq.x (fixes case 6 lw p1030 placement). Case 5 lw p150 still after vmul. MulT r[0] vf1 (target vf3): "=j"(r)+"0"(r[0]) last, r[1]/r[2] operand swaps, r0 local: no change (16-17). Pull by-value VF helpers: 30. */
/* v3 (Claude agent, laneB4, 2026-10-23): aligned 37 -> 17. LenXY as two asms: vmul.xy alone, then vaddy.x+vsqrt+vwaitq+vaddq.x fused (fixes the extra-instruction offset). */
/* v2 (Claude agent, 2026-10-23): aligned 42 -> 37. MulT asm operand order: outputs (r, one, t0, t1, t2), inputs (v, r[2], r[1]) gives the target's temp VF numbers (t0 vf7, one vf8, t1 vf6, t2 vf5). Left: r = r[0] lands in vf1 (target vf3; "0"(r[0]) tie same), LenXY: target loads p150 before the vmul and p1030 after vaddq (fused LenXY asm worse, 46-54), the mtc1/add.s nop in case 6. */
struct POLY { float a; float b; float c; };
extern struct LM g_lmZeroOne __asm__("g_lmZeroOne");
extern float D_002BC934 __asm__("D_002BC934");
extern struct POLY D_002BC940 __asm__("D_002BC940");
extern struct POLY D_002BC950 __asm__("D_002BC950");
extern float D_002BC960 __asm__("D_002BC960");
void UpdateStepplayer(BENTLEY *p) __asm__("UpdateStepplayer");
void GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi(void *pv, BENTLEY *p, int n) __asm__("GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi");
void UpdateBentleyJtMode(BENTLEY *p) __asm__("UpdateBentleyJtMode");
void UpdateBentleyStateMachine(BENTLEY *p) __asm__("UpdateBentleyStateMachine");
void SetStepTargetHeading(BENTLEY *p, float g) __asm__("SetStepTargetHeading");
int GetStepplayerLeanFlags(BENTLEY *p) __asm__("GetStepplayerLeanFlags");
void SetSmaMask(SMA *a, int b) __asm__("SetSmaMask");
void UpdateGaitBlend(GAIT *pv, float g) __asm__("UpdateGaitBlend");
void UpdateGaitFootsteps(GAIT *pv) __asm__("UpdateGaitFootsteps");
void UpdateJtActiveJump(JTAIR *pv) __asm__("UpdateJtActiveJump");
void UpdateJtActiveHide(JTGRAB *pv) __asm__("UpdateJtActiveHide");
void FaceStepToward(BENTLEY *p, LO *plo) __asm__("FaceStepToward");
qword SetVectorCylind(float a, float b, float c) __asm__("SetVectorCylind");
void UpdateDirectionalBlend(BENTLEY *p, BLDIR *pv, ASEGBL *n) __asm__("UpdateDirectionalBlend");
qword GetStepAsegaTranslation(BENTLEY *p, struct ASEGA *px, float g) __asm__("GetStepAsegaTranslation");
void SetAsegaSpeed__FP5ASEGAf(struct ASEGA *px, float g) __asm__("SetAsegaSpeed__FP5ASEGAf");
float NormalizeAngle__Ff(float g) __asm__("NormalizeAngle__Ff");
float ClampAbs__Fff(float g, float dg) __asm__("ClampAbs__Fff");
void UpdateBentleyPlaces(BENTLEY *p) __asm__("UpdateBentleyPlaces");
void UpdateJtActiveZap(JTHURT *pv) __asm__("UpdateJtActiveZap");

// Evaluate the quadratic a + b*t + c*t^2.
extern "C" { static __inline__ float EvaluatePoly(struct POLY *pp, float t) { return pp->a + t * (pp->b + t * pp->c); } }
// Clamp g to the range [minValue, maxValue] of range.
extern "C" { static __inline__ float ClampF(float g, struct LM *range)
{
    float lo = range->minValue;
    float hi = range->maxValue;
    if (g < lo) return lo;
    if (hi < g) return hi;
    return g;
} }
// XY length of v as a float.
extern "C" { static __inline__ float LenXY(qword v)
{
    qword l;
    float g;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(l) : "j"(v));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
    return g;
} }
// XY length of v as a float (pa and pb only influence register allocation).
extern "C" { static __inline__ float LenXY5(qword v, POCTL *pa, struct ASEGA *pb)
{
    qword l;
    float g;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(l) : "j"(v));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l), "r"(pa));  /* allocation-only "r"(pa): raises the lw p150 sched priority so it issues before the vmul */
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l), "r"(pb), "r"(pb));  /* allocation-only "r"(pb) x2: raises p1030's local-alloc priority above p150 (p1030 $v0, p150 $v1) */
    return g;
} }
// Rotate v by the transpose of the 3x3 matrix pm (result pinned to vf3).
extern "C" { static __inline__ qword MulT(qword *pm, qword v)
{
    register qword r __asm__("vf3");  /* register pin: VF allocation tie, target keeps the result in vf3 */
    qword t0, t1, t2, one;
    r = pm[0];
    __asm__("vmul.xyzw %2, %0, %5\n\tvmaxw.xyzw %1, $vf0, $vf0w\n\tvmul.xyzw %3, %6, %5\n\tvmul.xyzw %4, %7, %5\n\tvadday.x $ACC, %2, %2y\n\tvmaddz.x %0, %1, %2z\n\tvaddax.y $ACC, %3, %3x\n\tvmaddz.y %0, %1, %3z\n\tvaddax.z $ACC, %4, %4x\n\tvmaddy.z %0, %1, %4y"
            : "+j"(r), "=&j"(one), "=&j"(t0), "=&j"(t1), "=&j"(t2) : "j"(v), "j"(pm[1]), "j"(pm[2]));
    return r;
} }
// Set the actor's qwUnknown190 velocity to v rotated by its target orientation, reset blendSide and update the
// directional blend.
extern "C" { static __inline__ void SetVel(BENTLEY *p, qword v, BLDIR *pv, ASEGBL *n)
{
    p->qwUnknown190 = v;
    p->qwUnknown190 = MulT(p->aqwTarget, v);
    p->blendSide = -1;
    UpdateDirectionalBlend(p, pv, n);
} }
// Set v190 to the negated step-animation translation of the current action.
extern "C" { static __inline__ void Pull(BENTLEY *p)
{
    QWROW u;
    qword v = GetStepAsegaTranslation(p, p->activeAnimation, p->activeAnimation->time);
    qword r;
    qword uq;
    __asm__("qmtc2.ni %1, %0" : "=j"(uq) : "r"(-1.0f));
    u.q = uq;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(u.q));
    p->qwUnknown190 = r;
} }
// Set v190 to the negated step-animation translation of the current action (volatile stores keep the original order).
extern "C" { static __inline__ void PullB(BENTLEY *p)
{
    QWROW u;
    qword v = GetStepAsegaTranslation(p, p->activeAnimation, p->activeAnimation->time);
    qword r, t;
    __asm__("qmtc2.ni %1, %0" : "=j"(t) : "r"(-1.0f));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(t));
    *(volatile qword *)&p->qwUnknown190 = r;
    *(volatile qword *)&u.q = t;
} }
// Update the gait blend by g and then the footsteps.
extern "C" { static __inline__ void Kick(GAIT *pv, float g)
{
    UpdateGaitBlend(pv, g);
    UpdateGaitFootsteps(pv);
} }

// BENTLEY Update (slot 16): UpdateStepplayer, then (if in the world) the state machine and per-state actions.
// Also known as: UpdateBentley (upstream readable / Sly 1 name; UpdateBentley__FP7BENTLEY).
extern "C" void UpdateBentley(BENTLEY *bentley)
{
    UpdateStepplayer(bentley);
    if (((bentley->isInWorld) & 1) == 0)
        return;
    GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi(&g_sw, bentley, 0xF8FE);
    UpdateBentleyJtMode(bentley);
    UpdateBentleyStateMachine(bentley);
    switch (bentley->state) {
    case BENTLEY_IDLE:
        if (bentley->unknown181C) {
            SetStepTargetHeading(bentley, D_002BC934);
            break;
        }
        SetSmaMask(bentley->activeStateMachine, GetStepplayerLeanFlags(bentley));
        break;
    case BENTLEY_RUN:
        SetStepTargetHeading(bentley, bentley->controller->leftStickAngle);
        Kick(&bentley->gaitA, 1.0f);
        break;
    case BENTLEY_WALK:
        SetStepTargetHeading(bentley, bentley->controller->leftStickAngle);
        Kick(&bentley->gaitB, bentley->controller->leftStickDeflection);
        break;
    case BENTLEY_AIR:
        UpdateJtActiveJump(&bentley->jtair);
        break;
    case BENTLEY_GRAB:
        UpdateJtActiveHide(&bentley->jtgrab);
        break;
    case BENTLEY_HURT:
    case BENTLEY_HOVER:
    case BENTLEY_PLAYER_STATE_4:
    case BENTLEY_THROW:
    case BENTLEY_PIPE:
        break;
    case BENTLEY_NEAR_TARGET:
        FaceStepToward(bentley, bentley->interactTarget);
        {
            POCTL *pa = bentley->controller;
            struct ASEGA *pb = bentley->activeAnimation;
            SetVel(bentley, SetVectorCylind(pa->leftStickAngle + 3.1415927f, pa->leftStickDeflection * LenXY5(bentley->abldirA[1].vRoot, pa, pb) * pb->speed, 0.0f), bentley->abldirA, bentley->pasegblA);
        }
        break;
    case BENTLEY_GADGET:
        if (bentley->subState == 1) {
            float g0 = ClampF(EvaluatePoly(&D_002BC940, bentley->controller->leftStickDeflection), &g_lmZeroOne);
            float g1 = ClampF(EvaluatePoly(&D_002BC950, bentley->controller->leftStickDeflection), &g_lmZeroOne);
            float z = 0.0f;
            if (z < bentley->controller->leftStickDeflection) {
                float d = NormalizeAngle__Ff(bentley->controller->leftStickAngle - bentley->targetHeading);
                SetStepTargetHeading(bentley, bentley->targetHeading + ClampAbs__Fff(d, g1 * D_002BC960 * g_clock.deltaTime));
            }
            SetVel(bentley, SetVectorCylind(bentley->controller->leftStickAngle + 3.1415927f, g0 * LenXY(bentley->abldirB[1].vRoot) * bentley->activeAnimation->speed, z), bentley->abldirB, bentley->pasegblB);
        }
        break;
    case BENTLEY_SCRIPT_ANIM:
        Pull(bentley);
        break;
    case BENTLEY_SCRIPT_SMA:
        PullB(bentley);
        SetAsegaSpeed__FP5ASEGAf(bentley->activeAnimation, bentley->controller->leftStickDeflection);
        SetStepTargetHeading(bentley, bentley->controller->leftStickAngle);
        break;
    case BENTLEY_ADRENALINE:
        Pull(bentley);
        SetStepTargetHeading(bentley, bentley->controller->leftStickAngle);
        break;
    }
    UpdateBentleyPlaces(bentley);
    UpdateJtActiveZap(&bentley->jthurt);
}
} // namespace M_UpdateBentley

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SetBentleyState {
/* The held tossable (classes.h CID100: its slot 76 is not declared there). */
struct VO { char pad[0xC]; VTE *vt; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void SetXfRole(BENTLEY *, XF *, int) __asm__("SetXfRole");
void ScheduleIdleTimer(BENTLEY *) __asm__("ScheduleIdleTimer");
void ExplodeWithDefaults(LO *) __asm__("ExplodeWithDefaults");
void SetJtAirState(JTAIR *, int, int) __asm__("SetJtAirState");
void SetJtGrabKind(JTGRAB *, int) __asm__("SetJtGrabKind");
void SetJtHurtState(JTHURT *, int) __asm__("SetJtHurtState");
void SetJtPipeState(JTPIPE *, int) __asm__("SetJtPipeState");
void ReleaseBentleyHeldObject(BENTLEY *) __asm__("ReleaseBentleyHeldObject");
void SetBentleySubState(BENTLEY *, int) __asm__("SetBentleySubState");
void SetBentleyHoverState(BENTLEY *, int) __asm__("SetBentleyHoverState");
void SetJtThrowState(JTTHROW *, int) __asm__("SetJtThrowState");
int FindSmStateOptional__FP2SM3OID(void *, int) __asm__("FindSmStateOptional__FP2SM3OID");
int GetStepplayerLeanFlags(BENTLEY *) __asm__("GetStepplayerLeanFlags");
void InitIdleTimer(BENTLEY *, int) __asm__("InitIdleTimer");
void ResetGaitFeet(GAIT *) __asm__("ResetGaitFeet");
void SetJtModeState__FP6JTMODEi(JTMODE *, int) __asm__("SetJtModeState__FP6JTMODEi");
void ReleaseAsega(ASEGA **) __asm__("ReleaseAsega");
void ReleaseSma(SMA **) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(struct ASEG *, BENTLEY *, int, ASEGA **, float, float) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void ReplaceSmaEx(void *, BENTLEY *, int, int, int, SMA **) __asm__("ReplaceSmaEx");

#define VCALL1(p, n, a) ((void (*)(void *, int))(p)->vt[n].pfn)((char *)(p) + (p)->vt[n].d, a)

// Bentley state change: exit/enter actions, animations and component state changes.
// Also known as: SetBentleyState (upstream readable / Sly 1 name; SetBentleyState__FP7BENTLEYi).
extern "C" void SetBentleyState(BENTLEY *p, int k)
{
    int kOld;
    struct ASEG *asega;
    void *jt;
    int c;
    int anim;
    int n;
    float g;
    float rate;
    GAIT *pa;

    if (k == p->state)
        return;
    switch (p->state) {
    case BENTLEY_IDLE:
        SetXfRole(p, p->positionXf, 2);
        SetXfRole(p, p->rotationXf, 2);
        ScheduleIdleTimer(p);
        break;
    case BENTLEY_PLAYER_STATE_4:
    case BENTLEY_SCRIPT_ANIM:
    case BENTLEY_SCRIPT_SMA:
        SetXfRole(p, p->positionXf, 2);
        SetXfRole(p, p->rotationXf, 2);
        break;
    case BENTLEY_AIR:
        if (p->pwater == 0) {
            LO *object = 0;
            if (k >= 0) {
                if (k < 2) {
                    object = p->aploLandEffect[0];
                } else if (k == 3) {
                    switch (p->jtgrab.state) {
                    case 1:
                        break;
                    case 3:
                    case 4:
                        object = p->aploLandEffect[1];
                        break;
                    default:
                        object = p->aploLandEffect[0];
                        break;
                    }
                }
            }
            if (object) {
                object->Add();
                ExplodeWithDefaults(object);
                object->Remove();
            }
        }
        SetJtAirState(&p->jtair, -1, 0);
        break;
    case BENTLEY_GRAB:
        SetJtGrabKind(&p->jtgrab, -1);
        break;
    case BENTLEY_HURT:
        SetJtHurtState(&p->jthurt, -1);
        break;
    case BENTLEY_PIPE:
        SetJtPipeState(&p->jtpipe, -1);
        break;
    case BENTLEY_GADGET:
        if (p->ploHeld) {
            VCALL1((struct VO *)p->ploHeld, 76, 1);
            ReleaseBentleyHeldObject(p);
        }
        SetBentleySubState(p, -1);
        break;
    case BENTLEY_HOVER:
        SetBentleyHoverState(p, -1);
        p->SetCtlState(1);
        break;
    case BENTLEY_RUN:
    case BENTLEY_WALK:
        p->gaitBlend = p->activeAnimation->time / p->activeAnimation->animation->duration;
        break;
    case BENTLEY_THROW:
        SetJtThrowState(&p->jtthrow, -1);
        break;
    }
    kOld = p->state;
    p->state = k;
    asega = 0;
    jt = 0;
    n = 1;
    anim = -1;
    c = 4;
    g = 0.0f;
    rate = 1.0f;
    p->stateTime = g_clock.time;
    switch (k) {
    case BENTLEY_IDLE:
        SetXfRole(p, p->positionXf, 1);
        SetXfRole(p, p->rotationXf, 1);
        if (p->unknown181C) {
            asega = p->unknown1820;
            n = 9;
        } else {
            jt = p->unknown1028;
            if (kOld != 2)
                anim = kOld != 7 ? 0x175 : 0x176;
            else
                anim = 0x178;
            if (FindSmStateOptional__FP2SM3OID(jt, anim) < 0)
                anim = -1;
        }
        p->qwUnknown190 = D_002A2DD0;
        c = GetStepplayerLeanFlags(p);
        InitIdleTimer(p, 1);
        break;
    case BENTLEY_RUN:
        asega = p->gaitA.blendedAnimation;
        pa = &p->gaitA;
        goto L1;
    case BENTLEY_WALK:
        asega = p->gaitB.blendedAnimation;
        pa = &p->gaitB;
    L1:
        n = 9;
        g = p->gaitBlend * asega->duration;
        ResetGaitFeet(pa);
        break;
    case BENTLEY_HURT:
        SetJtModeState__FP6JTMODEi(&p->jtmode, -1);
        break;
    case BENTLEY_NEAR_TARGET:
        asega = p->pasegblA;
        n = 9;
        break;
    case BENTLEY_PIPE:
        SetJtPipeState(&p->jtpipe, 0);
        return;
    case BENTLEY_GADGET:
        SetBentleySubState(p, 0);
        return;
    case BENTLEY_HOVER:
        SetBentleyHoverState(p, 0);
        return;
    case BENTLEY_PLAYER_STATE_4:
        jt = p->unknown102C;
    case BENTLEY_SCRIPT_ANIM:
    case BENTLEY_SCRIPT_SMA:
        SetXfRole(p, p->positionXf, 1);
        SetXfRole(p, p->rotationXf, 1);
        p->qwUnknown190 = D_002A2DD0;
        break;
    case BENTLEY_ADRENALINE:
        asega = p->pasegAdrenaline;
        if (!asega) {
            asega = p->gaitA.abl[1].animation;
            rate = 1.5f;
        }
        n = 9;
        break;
    case BENTLEY_THROW:
        return;
    }
    ReleaseAsega(&p->activeAnimation);
    ReleaseSma(&p->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(asega, p, n, &p->activeAnimation, g, rate);
    ReplaceSmaEx(jt, p, anim, n, c, &p->activeStateMachine);
    p->ApplyStateStepParams();
}
} // namespace M_SetBentleyState

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001356E8 {
typedef int ti128 __attribute__((mode(TI)));
struct C { int a[4]; };
struct LSG { ti128 pos; int pad[0x14]; };
union VQ { float af[4]; ti128 q; };
extern VQ D_002BC970 __asm__("D_002BC970");    /* screen points for ConvertCmScreenToWorld */
extern VQ D_002BC980 __asm__("D_002BC980");
ti128 ConvertCmScreenToWorld(void *, ti128) __asm__("ConvertCmScreenToWorld");
struct C *STACKSCOPE_ctor(struct C *) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(struct C *, int) __asm__("_$_10STACKSCOPE");
void ShouldKeepSoNotSelf() __asm__("ShouldKeepSoNotSelf");
void IntersectSwBoundingBox(void *, ti128, ti128, void (*)(), SOTREE *, int *, int *) __asm__("IntersectSwBoundingBox");
int FindFirstObjectHitByLine(int, ti128, ti128, int, int, struct LSG *) __asm__("FindFirstObjectHitByLine");
// Casts a ray through two screen points into the world (box query plus line test against objects) and stores the hit
// (or the far point) as the aim point.
// Also known as: FireBentleyDart (upstream readable / Sly 1 name; FireBentleyDart__FP7BENTLEY).
extern "C" void UpdateBentleyAimPoint(BENTLEY *bentley) {
    struct C ctx;
    struct LSG lsg;
    int cso;
    int aso;
    ti128 v1 = ConvertCmScreenToWorld(&g_cm, D_002BC970.q);
    ti128 v2 = ConvertCmScreenToWorld(&g_cm, D_002BC980.q);
    STACKSCOPE_ctor(&ctx);
    IntersectSwBoundingBox(&g_sw, v1, v2, ShouldKeepSoNotSelf, bentley->physicsTree, &cso, &aso);
    if (FindFirstObjectHitByLine(1, v1, v2, cso, aso, &lsg)) bentley->aimPoint = lsg.pos;
    else bentley->aimPoint = v2;
    STACKSCOPE_ctor_1AE4A0(&ctx, 2);
}
} // namespace M_func_001356E8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_FireBentleyDart {
typedef int ti128 __attribute__((mode(TI)));
/* The launcher (BENTLEY::psoLauncher, CID 112) as written here: its target and a flag. */
struct Q { char pad[0x50]; ti128 v50; int f60; };
extern ti128 D_002A2DD0 __asm__("D_002A2DD0");
void UpdateBentleyAimPoint(BENTLEY *) __asm__("UpdateBentleyAimPoint");
SO *AcquireProxyObject(PROXY *, int) __asm__("AcquireProxyObject");
void LaunchSoFromLauncher(struct Q *, SO *) __asm__("LaunchSoFromLauncher");
void PrimeBomb__FP4BOMBf(SO *, float) __asm__("PrimeBomb__FP4BOMBf");
void SetBentleyAimState(BENTLEY *, int) __asm__("SetBentleyAimState");
// Fires the binocucom crossbow dart: clones the selected projectile template and launches it from the launcher
// (CID112) toward the aim point. (runtime-verified: the dart, not a bomb)
extern "C" void FireBentleyDart(BENTLEY *bentley, int a) {
    PROXY *pv;
    SO *object;
    struct Q *pq;
    UpdateBentleyAimPoint(bentley);
    switch (bentley->dartKind) {
    case 0: pv = bentley->aproxyDart[0]; break;
    case 1: pv = bentley->aproxyDart[1]; break;
    default: return;
    }
    if (!pv) return;
    object = AcquireProxyObject(pv, 0);
    if (!object) return;
    object->parts->zapKindFlags = a;
    pq = (struct Q *)bentley->launcher;
    pq->f60 = 0;
    pq->v50 = bentley->aimPoint;
    LaunchSoFromLauncher(pq, object);
    ((SO *)object)->SetAngularVelocity(D_002A2DD0);
    PrimeBomb__FP4BOMBf(object, 4.0f);
    SetBentleyAimState(bentley, 1);
    bentley->lastDartTime = g_clock.time;
}
} // namespace M_FireBentleyDart

// BENTLEY camera definition (slot 50): GetStepplayerCpdefi plus per-state tweaks (pipe 13 uses the pipe velocity,
// state 5 sets a flag).
// Also known as: GetBentleyCpdefi (upstream readable / Sly 1 name; GetBentleyCpdefi__FP7BENTLEYP9ACTORPOSE).
extern "C" void GetBentleyCpdefi(BENTLEY *bentley, CPDEFI *pcpdefi)
{
    int state;
    state = bentley->state;
    if (state != 0 && (state != 9) && (bentley->jtgrab.kind != 0)) {
        bentley->unknown1080 = g_clock.time;
    }
    GetStepplayerCpdefi(bentley, pcpdefi);
    switch (bentley->state) {
    case BENTLEY_PIPE:
        GetJtPipeVelocity(&bentley->jtpipe, pcpdefi);
        return;
    case BENTLEY_NEAR_TARGET:
        pcpdefi->grfcpd |= 1;
        return;
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00135940 {
typedef union { qword q; float f; } U;
/* Bentley as the register-pinned drag code reads him: the g++ vtable pointer at +0xC. */
struct VOBJ { char pad[0xC]; VTE *pvt; };
extern float D_002BC990 __asm__("D_002BC990");
void PresetSoAccel(BENTLEY *p) __asm__("PresetSoAccel");
void PresetStepAccel(BENTLEY *p) __asm__("PresetStepAccel");
void PresetJtAccelJump(JTAIR *p) __asm__("PresetJtAccelJump");
void PresetJtAccelHide(JTGRAB *p) __asm__("PresetJtAccelHide");
void PresetJtAccelZap(JTHURT *p) __asm__("PresetJtAccelZap");
void PresetJtAccelPipe(JTPIPE *p) __asm__("PresetJtAccelPipe");
void AddSoAcceleration(BENTLEY *p, qword v) __asm__("AddSoAcceleration");
// BENTLEY physics preset (slot 70): per state, the pipe / air / grab / hurt component's preset; state 6 sub-state 2
// applies drag.
// Also known as: UpdateBentleyMotion (upstream readable / Sly 1 name; UpdateBentleyMotion__FP3ALO).
extern "C" void PresetBentleyAccel(BENTLEY *bentley)
{
    if (bentley->state == BENTLEY_PIPE)
        PresetSoAccel(bentley);
    else
        PresetStepAccel(bentley);
    switch (bentley->state) {
    case BENTLEY_AIR: PresetJtAccelJump(&bentley->jtair); break;
    case BENTLEY_GRAB: PresetJtAccelHide(&bentley->jtgrab); break;
    case BENTLEY_HURT: PresetJtAccelZap(&bentley->jthurt); break;
    case BENTLEY_PIPE: PresetJtAccelPipe(&bentley->jtpipe); break;
    case BENTLEY_GADGET:
        if (bentley->subState == 2) {
            register VTE *vt __asm__("$3") = ((VOBJ *)bentley)->pvt;
            U a, u;
            qword v, r, q;
            qword (*function)(void *) = (qword (*)(void *))vt[33].pfn;
            short d = vt[33].d;
            __asm__("mfc1 %0, %1" : "=r"(q) : "f"(-D_002BC990));
            a.q = q;
            u.q = q;
            v = function((char *)bentley + d);
            __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "0"(u.q), "j"(v));
            AddSoAcceleration(bentley, r);
        }
        break;
    }
}
} // namespace M_func_00135940

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00135A48 {
struct O { char pad[0x10]; unsigned long grf; };
struct E { struct O *po; char pad[0x3c]; };
void AdjustJtHurtXp(void *, struct E *) __asm__("AdjustJtHurtXp");
void ResetJtPipeAdjust(void *, struct E *) __asm__("ResetJtPipeAdjust");
void AdjustStepNewXp(BENTLEY *, struct E *, int) __asm__("AdjustStepNewXp");
// BENTLEY contact adjust (slot 55): hurt -> AdjustJtHurtXp, pipe -> ResetJtPipeAdjust, else the STEP base.
// Also known as: OnBentleyXpNew (upstream readable / Sly 1 name; OnBentleyXpNew__FP3ALOP2XPi).
extern "C" void AdjustBentleyXp(BENTLEY *bentley, struct E *ae, int i) {
    struct E *pe = &ae[i];
    if (pe->po->grf & (0x8000L << 33)) return;
    switch (bentley->state) {
    case BENTLEY_HURT: AdjustJtHurtXp(&bentley->jthurt, ae); break;
    case BENTLEY_PIPE: ResetJtPipeAdjust(&bentley->jtpipe, ae); break;
    default: AdjustStepNewXp(bentley, ae, i); break;
    }
}
} // namespace M_func_00135A48

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_HandleBentleyMessage {
/* The spawned tossable (BENTLEY::ploHeld; classes.h CID100 has no fields yet): its gadget id and slot 76. */
struct O { char pad[0xC]; void *vptr; char pad2[0x15C - 0x10]; int x15C; };
struct M { void *x0; int x4; int x8; };
void HandleStepplayerMessage(BENTLEY *p, int k, struct M *pm) __asm__("HandleStepplayerMessage");
LO *AcquireProxyObject(PROXY *pv, int n) __asm__("AcquireProxyObject");
void LockProxyObject(PROXY *pv, LO *po) __asm__("LockProxyObject");
void FireBentleyDart(BENTLEY *p, int n) __asm__("FireBentleyDart");
void HandleBentleyCrosshairMessage(BTBLENDS *pv, int k, struct M *pm) __asm__("HandleBentleyCrosshairMessage");
void SetGadgetRecordObject(BENTLEY *p, JTPOCKET *pe, int n) __asm__("SetGadgetRecordObject");
// BENTLEY message handler (slot 5): HandleStepplayerMessage, then Bentley-specific animation events (message 9 kinds:
// 0x216 spawns an object from a template, ...).
// Also known as: OnBentleyMessage (upstream readable / Sly 1 name; OnBentleyMessage__FP2LO5MSGIDPv).
extern "C" void HandleBentleyMessage(BENTLEY *bentley, int k, struct M *pm)
{
    HandleStepplayerMessage(bentley, k, pm);
    switch (k) {
    case 9:
        if (pm->x0 == bentley->activeAnimation) {
            if (pm->x8 == 0x216) {
                struct O *object;
                bentley->ploHeld = AcquireProxyObject(bentley->ppocketCur->proxy, 0);
                LockProxyObject(bentley->ppocketCur->proxy, bentley->ploHeld);
                ((struct O *)bentley->ploHeld)->x15C = bentley->ppocketCur->idGadget;
                object = (struct O *)bentley->ploHeld;
                ((CID100 *)object)->SetState(0);
            }
        } else if (pm->x0 == bentley->pasegaAim) {
            if (pm->x8 == 0x243)
                FireBentleyDart(bentley, 0x80000);
        } else {
            HandleBentleyCrosshairMessage(&bentley->btblends, 9, pm);
        }
        break;
    case 1: {
        JTPOCKET *pe = bentley->apocket;
        int i;
        for (i = 0; i < 6; i++, pe++) {
            if ((void *)pe->pcid100 == pm)
                SetGadgetRecordObject(bentley, pe, 0);
        }
        break;
    }
    }
}
} // namespace M_HandleBentleyMessage

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_SetBentleyPlayerState {
void SetBentleyState(void *, int) __asm__("SetBentleyState");
// BENTLEY slot 88: maps a generic player state 0..8 to Bentley states (0, 2, 3, 4, 9, 10, 11, 12, 13).
// Also known as: SetBentleyJumpState (upstream readable / Sly 1 name; SetBentleyJumpState__FP2LOi).
extern "C" void SetBentleyPlayerState(void *bentley, unsigned int n)
{
    switch (n) {
    case 0: SetBentleyState(bentley, BENTLEY_IDLE); break;
    case 1: SetBentleyState(bentley, BENTLEY_AIR); break;
    case 2: SetBentleyState(bentley, BENTLEY_GRAB); break;
    case 3: SetBentleyState(bentley, BENTLEY_HURT); break;
    case 4: SetBentleyState(bentley, BENTLEY_PLAYER_STATE_4); break;
    case 5: SetBentleyState(bentley, BENTLEY_SCRIPT_ANIM); break;
    case 6: SetBentleyState(bentley, BENTLEY_SCRIPT_SMA); break;
    case 7: SetBentleyState(bentley, BENTLEY_THROW); break;
    case 8: SetBentleyState(bentley, BENTLEY_PIPE); break;
    }
}
} // namespace M_SetBentleyPlayerState

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BENTLEY_PlayerStateCur {
// BENTLEY slot 89: the inverse map, Bentley state -> generic player state 0..8 (else -1).
// Also known as: BentleyJumpKind (upstream readable / Sly 1 name; BentleyJumpKind__FP7BENTLEY).
extern "C" int GetBentleyPlayerState(BENTLEY *bentley) {
    switch (bentley->state) {
    case BENTLEY_IDLE: return 0;
    case BENTLEY_AIR: return 1;
    case BENTLEY_GRAB: return 2;
    case BENTLEY_HURT: return 3;
    case BENTLEY_PLAYER_STATE_4: return 4;
    case BENTLEY_SCRIPT_ANIM: return 5;
    case BENTLEY_SCRIPT_SMA: return 6;
    case BENTLEY_THROW: return 7;
    case BENTLEY_PIPE: return 8;
    }
    return -1;
}
} // namespace M_BENTLEY_PlayerStateCur

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00135D80 {
SOPART *GetJtGrabAdjustRecord(JTGRAB *, int *) __asm__("GetJtGrabAdjustRecord");
SOPART *GetJtAirAdjustRecord(JTAIR *, int *) __asm__("GetJtAirAdjustRecord");
SOPART *GetJtHurtAdjustRecord(JTHURT *, int *) __asm__("GetJtHurtAdjustRecord");
void SetStepPhys(BENTLEY *, SOPART *, int) __asm__("SetStepPhys");
// BENTLEY slot 90: asks the active component (air/grab/hurt) for the per-state STEP parameter block, default +0x268,
// then applies it (STEP base).
extern "C" void ApplyBentleyStateStepParams(BENTLEY *bentley) {
    int n = 0;
    SOPART *r;
    switch (bentley->state) {
    case BENTLEY_GRAB: r = GetJtGrabAdjustRecord(&bentley->jtgrab, &n); break;
    case BENTLEY_HURT: r = GetJtHurtAdjustRecord(&bentley->jthurt, &n); break;
    case BENTLEY_AIR: r = GetJtAirAdjustRecord(&bentley->jtair, &n); break;
    default: r = 0; break;
    }
    if (!r) r = bentley->ppartPhysMesh;
    SetStepPhys(bentley, r, n);
}
} // namespace M_func_00135D80

// BENTLEY slot 91 (end the melee attack; Sly cancels his cane state here): Bentley has none, empty.
extern "C" void EndBentleyMeleeAttack()
{
}

// BENTLEY slot 92 (face the object he is attached to; Sly does): returns 0.
extern "C" int TryFaceBentleyAttachedObject()
{
    return 0;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00135E40 {
int GetStepplayerMovementNoise(BENTLEY *) __asm__("GetStepplayerMovementNoise");
// BENTLEY slot 98, movement-noise class (inferred; see GetStepplayerMovementNoise): the base result unless it is -1;
// then 1 (noisy) in BENTLEY_RUN/NEAR_TARGET/ADRENALINE, else -1.
extern "C" int GetBentleyMovementNoise(BENTLEY *bentley) {
    int r = GetStepplayerMovementNoise(bentley);
    if (r != -1) return r;
    switch (bentley->state) {
    case BENTLEY_RUN: case BENTLEY_NEAR_TARGET: case BENTLEY_ADRENALINE: return 1;
    }
    return -1;
}
} // namespace M_func_00135E40

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00135EB0 {
typedef int ti128 __attribute__((mode(TI)));
struct Q { char pad[0x148]; unsigned long grf; };
/* The floor contact (STEP::floorContact; XP has no shared definition yet) and its surface material flags. */
struct A { char pad[0x40]; int f40; char pad1[8]; struct Q *pq; char pad2[0x60 - 0x50]; ti128 v60; char pad3[0x88 - 0x70]; float f88; };
void AddJtPlaceAtPosition(void *, int, ti128) __asm__("AddJtPlaceAtPosition");
void RecordJtGrabPlace(void *) __asm__("RecordJtGrabPlace");
void RecordJtPipePlace(void *) __asm__("RecordJtPipePlace");
// Per state: records standing places when on solid ground (AddJtPlaceAtPosition), or the grab/pipe updates.
// Also known as: UpdateActorSupport (upstream readable / Sly 1 name; UpdateActorSupport__FP5ACTOR).
extern "C" void UpdateBentleyPlaces(BENTLEY *bentley) {
    struct A *pa;
    switch (bentley->state) {
    case BENTLEY_IDLE: case BENTLEY_RUN: case BENTLEY_NEAR_TARGET: case BENTLEY_GADGET: case BENTLEY_WALK: case BENTLEY_SCRIPT_SMA: case BENTLEY_THROW: case BENTLEY_ADRENALINE:
        pa = (struct A *)bentley->floorContact;
        if (pa && 0.7f < pa->f88 && !(pa->pq->grf & 0x20000000) && !bentley->pwater)
            AddJtPlaceAtPosition(&bentley->jtplaces, pa->f40, pa->v60);
        break;
    case BENTLEY_GRAB:
        RecordJtGrabPlace(&bentley->jtgrab);
        break;
    case BENTLEY_PIPE:
        RecordJtPipePlace(&bentley->jtpipe);
        break;
    }
}
} // namespace M_func_00135EB0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BENTLEY_FInvulnerable {
int IsJtGrabBusy(JTGRAB *, int) __asm__("IsJtGrabBusy");
int IsStepplayerInvulnerable(BENTLEY *, int) __asm__("IsStepplayerInvulnerable");
// BENTLEY slot 76: on a pipe with zap kind 0x4000, or the grab check, else the STEPPLAYER base.
extern "C" int IsBentleyInvulnerable(BENTLEY *bentley, int msg) {
    switch (bentley->state) { case BENTLEY_PIPE: if (msg == 0x4000) return 1; break; case BENTLEY_GRAB: if (IsJtGrabBusy(&bentley->jtgrab, msg)) return 1; break; }
    return IsStepplayerInvulnerable(bentley, msg);
}
} // namespace M_BENTLEY_FInvulnerable

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00135FF0 {
// BENTLEY slot 94: 1 in states 0/1/5/7 (free movement), else 0. (inferred)
extern "C" int IsBentleyFreeMoving(BENTLEY *bentley) {
    switch (bentley->state) {
    case BENTLEY_IDLE: case BENTLEY_RUN: case BENTLEY_NEAR_TARGET: case BENTLEY_WALK: return 1;
    }
    return 0;
}
} // namespace M_func_00135FF0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136040 {
void ReleaseSma(void *) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *, BENTLEY *, float, float, int, ASEGA **) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
// Sets the +0x16B0 sub-state and plays its animation.
// Also known as: SetBentleyHold (upstream readable / Sly 1 name; SetBentleyHold__FP7BENTLEYi).
extern "C" void SetBentleySubState(BENTLEY *bentley, int n) {
    ASEG *aseg;
    int grfapl;
    if (n == bentley->subState) return;
    bentley->subState = n;
    aseg = 0;
    grfapl = 1;
    switch (n) {
    case 0:
        aseg = bentley->pasegSubState0;
        {
            VECTOR *pv = &bentley->unknown190;
            pv->z = 0.0f;
            pv->y = 0.0f;
            pv->x = 0.0f;
        }
        break;
    case 1:
        aseg = bentley->pasegblB;
        grfapl = 9;
        break;
    case 2:
        aseg = bentley->pasegSubState2;
        if (!aseg) aseg = bentley->abldirB[7].animation;
        grfapl = 9;
        break;
    }
    ReleaseSma(&bentley->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(aseg, bentley, 0.0f, 1.0f, grfapl, &bentley->activeAnimation);
    bentley->ApplyStateStepParams();
}
} // namespace M_func_00136040

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136118 {
/* The held tossable (BENTLEY::ploHeld; classes.h CID100 has no fields yet): state, a stick target and slot 76. */
struct O { char pad[0xC]; void *vptr; char pad1[0x158 - 0x10]; int x158; char pad2[0x1A0 - 0x15C]; int x1A0; };
int IsAsegaRetracted(ASEGA *pv) __asm__("IsAsegaRetracted");
void SetBentleySubState(BENTLEY *p, int n) __asm__("SetBentleySubState");
void StickTossable(struct O *po, int n) __asm__("StickTossable");
int GetGadgetButtonMask(BENTLEY *p, int n) __asm__("GetGadgetButtonMask");
int POCTL_IsButtonDown(POCTL *pv, int n) __asm__("POCTL_IsButtonDown");
void ReleaseBentleyHeldObject(BENTLEY *p) __asm__("ReleaseBentleyHeldObject");
void SetJtAirJump(JTAIR *pv, int a, int b, int c) __asm__("SetJtAirJump");
// The subState machine.
// Also known as: UpdateBentleyHold (upstream readable / Sly 1 name; UpdateBentleyHold__FP7BENTLEY).
extern "C" void UpdateBentleySubState(BENTLEY *bentley)
{
    int s;
    for (;;) {
        s = bentley->subState;
        switch (s) {
        case 0:
            if (IsAsegaRetracted(bentley->activeAnimation))
                SetBentleySubState(bentley, 1);
            break;
        case 1:
        case 2: {
            struct O *object = (struct O *)bentley->ploHeld;
            if (object->x158 == 2) {
                StickTossable(object, object->x1A0);
                ReleaseBentleyHeldObject(bentley);
                SetJtAirJump(&bentley->jtair, 0xB, 0, 0);
            } else if (object->x158 == 4 || !POCTL_IsButtonDown(bentley->controller, GetGadgetButtonMask(bentley, bentley->ppocketCur->idGadget) | BUTTON_ALT_ATTACK)) {
                ((CID100 *)object)->SetState(1);
                ReleaseBentleyHeldObject(bentley);
                SetJtAirJump(&bentley->jtair, 0xB, 0, 0);
            } else if
 (0.1f < g_clock.time - bentley->unknown20C) {
                SetBentleySubState(bentley, 2);
            } else {
                SetBentleySubState(bentley, 1);
            }
            break;
        }
        }
        if (bentley->subState == s || bentley->state != BENTLEY_GADGET)
            break;
    }
}
} // namespace M_func_00136118

/* Hands the proxy held at +0x186C back: hooks it to Bentley if IsThrowableGadget allows, then unlocks it in the
   current pocket's proxy. */
// Releases the object at +0x186C back to its manager.
// Also known as: ReleaseBentleyCarried (upstream readable / Sly 1 name; ReleaseBentleyCarried__FP7BENTLEY).
extern "C" void ReleaseBentleyHeldObject(BENTLEY *bentley)
{
    if (IsThrowableGadget(bentley->ploHeld) != 0) {
        *(BENTLEY **)((char *)bentley->ploHeld + 0x16C) = bentley;
        SetGadgetRecordObject(bentley, bentley->ppocketCur, bentley->ploHeld);
    }
    UnlockProxyObject(bentley->ppocketCur->proxy, bentley->ploHeld);
    bentley->ploHeld = 0;
    bentley->ppocketCur = 0;
}

// Sets the dart aim state (+0x16BC) with a timestamp; state 0 also stops the crosshair.
// Also known as: SetBentleyState2 (upstream readable / Sly 1 name; SetBentleyState2__FP7BENTLEYi).
extern "C" void SetBentleyAimState(BENTLEY *bentley, int n)
{
    if (n == bentley->aimState)
        return;
    bentley->aimState = n;
    bentley->aimStateTime = g_clock.time;
    if (n == 0) {
        SetBentleyCrosshairState(&bentley->btblends, -1);
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_UpdateBentleyAim {
extern float D_002BC994 __asm__("D_002BC994");
int IsStepplayerInJtMode(BENTLEY *p) __asm__("IsStepplayerInJtMode");
int ConsumeButtonsPressedWithin__5POCTLUif(void *pv, int n, float g) __asm__("ConsumeButtonsPressedWithin__5POCTLUif");
void FireBentleyDart(BENTLEY *p, int n) __asm__("FireBentleyDart");
void UpdateBentleyCrosshair(void *pv, int n) __asm__("UpdateBentleyCrosshair");
int FindSmStateOptional__FP2SM3OID(void *pv, int n) __asm__("FindSmStateOptional__FP2SM3OID");
void SnapSma(void *pv, int n) __asm__("SnapSma");
// Binocucom dart aim state machine (+0x16BC): -1 outside look mode, 0 ready, 1 fired/cool-down; the fire button within
// 0.25 s while ready fires (FireBentleyDart). The cool-down is 2.0 s.
// Also known as: UpdateBentleyState2 (upstream readable / Sly 1 name; UpdateBentleyState2__FP7BENTLEY).
extern "C" void UpdateBentleyAim(BENTLEY *bentley)
{
    int s;
    do {
        s = bentley->aimState;
        switch (s) {
        case 0:
        case 1:
            if (!IsStepplayerInJtMode(bentley)) {
                SetBentleyAimState(bentley, -1);
            } else if (bentley->aimState == 1) {
                if (D_002BC994 < g_clock.time - bentley->lastDartTime)
                    SetBentleyAimState(bentley, 0);
            } else if (ConsumeButtonsPressedWithin__5POCTLUif(bentley->controller, BUTTON_RUN, 0.25f)) {   // in look mode the run button fires
                FireBentleyDart(bentley, 0x80000);
            }
            break;
        case -1:
            if (IsStepplayerInJtMode(bentley) && bentley->jtmode.kind != 2) {
                if (D_002BC994 < g_clock.time - bentley->lastDartTime)
                    SetBentleyAimState(bentley, 0);
                else
                    SetBentleyAimState(bentley, 1);
            }
            switch (bentley->state) {
            case BENTLEY_IDLE:
                UpdateBentleyCrosshair(&bentley->btblends, 0);
                if (bentley->unknown181C && bentley->btblends.state != -1 && bentley->activeStateMachine && FindSmStateOptional__FP2SM3OID(bentley->unknown1028, 0x17B) >= 0)
                    SnapSma(bentley->activeStateMachine, 0x17B);
                break;
            case BENTLEY_RUN: case BENTLEY_AIR: case BENTLEY_GRAB: case BENTLEY_WALK:
                UpdateBentleyCrosshair(&bentley->btblends, 1);
                break;
            case BENTLEY_NEAR_TARGET:
                UpdateBentleyCrosshair(&bentley->btblends, 2);
                break;
            default:
                UpdateBentleyCrosshair(&bentley->btblends, -1);
                break;
            }
            break;
        }
    } while (s != bentley->aimState);
}
} // namespace M_UpdateBentleyAim

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136530 {
typedef struct { qword r[3]; qword pos; } M4;
extern MURRAY *D_002F7900 __asm__("D_002F7900");
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void SetLoParent(BENTLEY *, int, int) __asm__("SetLoParent");
void ReleaseAct(ACT **) __asm__("ReleaseAct");
void SetXfRole(BENTLEY *, XF *, int) __asm__("SetXfRole");
float GetStepHeading(BENTLEY *) __asm__("GetStepHeading");
void SetStepTargetHeading(BENTLEY *, float) __asm__("SetStepTargetHeading");
void FixStepAngularVelocity(BENTLEY *) __asm__("FixStepAngularVelocity");
void SetJtThrowState(JTTHROW *, int) __asm__("SetJtThrowState");
M4 GetRelativeXfMatrix(void *, void *, M4 *) __asm__("GetRelativeXfMatrix");
ACT *NewPositionAct(BENTLEY *p, XF *pxf, int a, int b, int c, qword q) __asm__("NewPositionAct");
qword DecomposeRotationMatrixToEuler(M4 *pm) __asm__("DecomposeRotationMatrixToEuler");
void ReleaseSma(SMA **) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *, BENTLEY *, int, ASEGA **, float, float) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");

// State machine on hoverState with reparenting and actuator teardown; also called from the throw code. (inferred: hover
// pack)
// Also known as: SetBentleyCarried (upstream readable / Sly 1 name; SetBentleyCarried__FP7BENTLEYi).
extern "C" void SetBentleyHoverState(BENTLEY *bentley, int k)
{
    ASEG *pv;
    if (k == bentley->hoverState)
        return;
    switch (bentley->hoverState) {
    case 0: {
        int save = bentley->jtplaces.cplace;
        SetLoParent(bentley, 0, 0xF);
        bentley->jtplaces.cplace = save;
        ReleaseAct(&bentley->apactHover[0]);
        ReleaseAct(&bentley->apactHover[1]);
        SetXfRole(bentley, bentley->positionXf, 2);
        SetXfRole(bentley, bentley->rotationXf, 2);
        break;
    }
    case 1:
        SetStepTargetHeading(bentley, GetStepHeading(bentley));
        bentley->unknown1A4 = 0;
        FixStepAngularVelocity(bentley);
        break;
    }
    bentley->hoverState = k;
    bentley->hoverStateTime = g_clock.time;
    pv = bentley->apasegHover[k];
    switch (k) {
    case -1: {
        MURRAY *pg = D_002F7900;
        if (pg->state != 0xF)
            return;
        if (pg->jtthrowAim.state == 7)
            return;
        SetJtThrowState(&pg->jtthrowAim, 0);
        return;
    }
    case 0: {
        M4 m;
        XF *pxf;
        SO *pq = (SO *)D_002F7900->jtthrowAim.ploHoldParent;
        pxf = pq->GetLocalTransform((MATRIX4 *)&m);
        { M4 t = GetRelativeXfMatrix(bentley->transforms->parent, bentley->positionXf->parent, &m); m = t; }
        {
            int save = bentley->jtplaces.cplace;
            bentley->SetParent(D_002F7900, pxf, 3);
            bentley->jtplaces.cplace = save;
        }
        bentley->SetVelocity(D_002A2DD0);
        bentley->SetAngularVelocity(D_002A2DD0);
        bentley->apactHover[0] = NewPositionAct(bentley, bentley->positionXf, 1, 7, 2, m.pos);
        bentley->apactHover[1] = NewPositionAct(bentley, bentley->rotationXf, 1, 7, 2, DecomposeRotationMatrixToEuler(&m));
        break;
    }
    case 1:
        bentley->unknown1A4 = -1;
        break;
    }
    ReleaseSma(&bentley->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(pv, bentley, 9, &bentley->activeAnimation, 0.0f, 1.0f);
    bentley->ApplyStateStepParams();
}
} // namespace M_func_00136530

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001367D8 {
void SetBentleyState(BENTLEY *, int) __asm__("SetBentleyState");
// Small transition loop: state 1 returns to 0 and makes Bentley the current player object.
extern "C" void UpdateBentleyStateMisc(BENTLEY *bentley) {
    int st;
    do {
        st = bentley->hoverState;
        switch (st) {
        case 0: break;
        case 1:
            if (bentley->floorContact) {
                SetBentleyState(bentley, BENTLEY_IDLE);
                bentley->SetCtlState(1);
            }
            break;
        }
    } while (bentley->hoverState != st && bentley->hoverState != -1);
}
} // namespace M_func_001367D8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_BENTLEY_FCanUseGadget {
/* MATCH (Claude agent laneB13, 2026-10-23), from v4 (4). One register pin (f in v1) plus commented empty asms that emulate the original toolchain's reorg/cross-jump state: case 12 has its own `return 0` (with an empty asm so it isn't merged with the shared fail), the shared fail has one too, the 1CADE0 path ends in `return 0`, and an empty asm using pe in the pv != 0 block keeps v0 live so reorg annuls the beql slot. */
/* v4 (Claude agent laneB12, 2026-10-23): aligned 10 -> 4. f pinned to v1 (one commented pin) and used for case 12's x15C and the ChooseProxySpare result; cases 2/3/13 test their call results directly; an empty volatile asm "+r"(f) after the 1CADE0 call keeps the target's `daddu v1,v0; bnez v1` (cse otherwise tests v0). Left: reorg fill choices: case 12 beq slot (target steals li v0,1 from `one`, ours takes li v0,0 from the fallthrough; `if (f != k) goto fail` flips the layout: 4), and the pv == 0 test (target beql, ours beqz). */
/* v3 (Claude agent, 2026-10-17): aligned 10. Shared function-scope `void *pv` for case 12 pa11C0 and pe->p4 puts pa in a0 like the target. Left: target loads x15C into v1 and copies the 1CADE0 result to v1 (daddu v1,v0; bnez v1). */
/* The carried tossable (classes.h CID100 has no fields yet): its gadget id. */
struct A { char pad[0x15C]; int x15C; };
extern int g_grfjoyt __asm__("g_grfjoyt");
int IsStepplayerInJtMode() __asm__("IsStepplayerInJtMode");
int CanJtUseGadgetInAir(void *pv, int k) __asm__("CanJtUseGadgetInAir");
int CanJtUseGadgetInGrab(void *pv, int k) __asm__("CanJtUseGadgetInGrab");
int CanJtUseGadgetOnPipe(void *pv, int k) __asm__("CanJtUseGadgetOnPipe");
JTPOCKET *GetBentleyGadgetFromId(BENTLEY *p, int k) __asm__("GetBentleyGadgetFromId");
int IsTossableActive(void *pv) __asm__("IsTossableActive");
int ChooseProxySpare(void *pv, int n) __asm__("ChooseProxySpare");
int IsCameraPolicyActive__FP5CPLCY(void *pv) __asm__("IsCameraPolicyActive__FP5CPLCY");
// BENTLEY slot 99: per-state checks and the per-gadget record's availability.
// Also known as: FBentleyAbilityOk (upstream readable / Sly 1 name; FBentleyAbilityOk__FP7BENTLEYi).
extern "C" int CanBentleyUseGadget(BENTLEY *bentley, int k)
{
    register int f __asm__("$3");  /* register pin: target keeps x15C and the ChooseProxySpare result in v1 */
    void *pv;
    if (bentley->unknown1818Wide != 0)
        goto fail;
    if (IsStepplayerInJtMode())
        return 0;
    switch (bentley->state) {
    case BENTLEY_AIR:
        if (CanJtUseGadgetInAir(&bentley->jtair, k))
            goto inner;
        goto fail;
    case BENTLEY_GRAB:
        if (CanJtUseGadgetInGrab(&bentley->jtgrab, k))
            goto inner;
        goto fail;
    case BENTLEY_PIPE:
        if (CanJtUseGadgetOnPipe(&bentley->jtpipe, k))
            goto inner;
    default:
    fail:
        __asm__ __volatile__("");  /* keeps the shared return 0 apart from case 12's */
        return 0;
    case BENTLEY_THROW:
        if ((unsigned int)(bentley->jtthrow.state - 4) < 3) {
            switch (k) {
            case 4: case 7: case 9: case 11: case 13:
                pv = bentley->jtthrow.ploCarried;
                f = ((struct A *)pv)->x15C;
                if (f == k)
                    goto one;
                __asm__ __volatile__("");  /* keeps case 12's return 0 from being merged with the shared one */
                return 0;
            }
        }
        goto fail;
    case BENTLEY_IDLE: case BENTLEY_RUN: case BENTLEY_NEAR_TARGET: case BENTLEY_WALK:
    inner:
        switch (k) {
        case 4: case 5: case 7: case 9: case 11: case 13: {
            JTPOCKET *pe = GetBentleyGadgetFromId(bentley, k);
            if (pe->pcid100) {
                if (IsTossableActive(pe->pcid100))
                    return 1;
                goto fail;
            }
            if ((pv = pe->proxy)) {
                __asm__ __volatile__("" : : "r"(pe));  /* keeps pe (v0) live into this block, so reorg annuls the beql slot */
                f = ChooseProxySpare(pv, 2);
                __asm__ __volatile__("" : "+r"(f));  /* keeps the copy in v1 that the target tests */
                if (f)
                    goto one;
            }
            return 0;
        }
        case 3: {
            int r = 0;
            if (bentley->unknown12F0 && (g_grfjoyt & 2))
                r = IsCameraPolicyActive__FP5CPLCY(&g_cm.part3a0) == 0;
            return r;
        }
        case 1: case 2: case 8: case 10: case 12: case 14:
        one:
            return 1;
        }
        goto fail;
    }
}
} // namespace M_BENTLEY_FCanUseGadget

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_PgadgetBentleyFromId {
/* Sly 2 version takes a base pointer in $a0 and saak in $a1 and returns a pointer into it. */
// The gadget record for a gadget id.
// Also known as: CbFromSaak (upstream readable / Sly 1 name; CbFromSaak__F4SAAK).
extern "C" int GetBentleyGadgetFromId(char *bentley, int saak)
{
    switch (saak) {
    case 5: return (int)(bentley + 0x1824);
    case 7: return (int)(bentley + 0x1830);
    case 9: return (int)(bentley + 0x183c);
    case 11: return (int)(bentley + 0x1848);
    case 13: return (int)(bentley + 0x1854);
    case 4: return (int)(bentley + 0x1860);
    }
    return 0;
}
} // namespace M_PgadgetBentleyFromId

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_StartBentleyGadget {
/* The spawned tossable (classes.h CID100 has no fields yet): its gadget id and owner. */
struct O { char pad[0x15C]; int x15C; char pad1[0x16C - 0x160]; void *p16C; };
void BroadcastGameEvent(GAME *pg, int n, BENTLEY *p) __asm__("BroadcastGameEvent");
JTPOCKET *GetBentleyGadgetFromId(BENTLEY *p, int k) __asm__("GetBentleyGadgetFromId");
void SetJtThrowState(JTTHROW *pv, int n) __asm__("SetJtThrowState");
void SetJtGrabState(JTGRAB *pv, int n, void *pv2, int m) __asm__("SetJtGrabState");
struct O *AcquireProxyObject(PROXY *pv, int n) __asm__("AcquireProxyObject");
int IsThrowableGadget(struct O *po) __asm__("IsThrowableGadget");
void SetGadgetRecordObject(BENTLEY *p, JTPOCKET *pe, struct O *po) __asm__("SetGadgetRecordObject");
void GetGadgetButtonMask(BENTLEY *p, int k) __asm__("GetGadgetButtonMask");
void SetBentleyState(BENTLEY *p, int n) __asm__("SetBentleyState");
void ChargeGadgetMp(BENTLEY *p, int k) __asm__("ChargeGadgetMp");
void StartStepplayerGadget(BENTLEY *p, int k) __asm__("StartStepplayerGadget");
// BENTLEY slot 100: broadcasts game event 0x3C, then per gadget id starts the throw/grab/air action using its record.
// Also known as: StartBentleyAbility (upstream readable / Sly 1 name; StartBentleyAbility__FP5ACTORi).
extern "C" void StartBentleyGadget(BENTLEY *bentley, int k)
{
    g_game.idGadgetLast = k;
    BroadcastGameEvent(&g_game, 0x3C, bentley);
    switch (k) {
    case 4: case 5: case 7: case 9: case 11: case 13: {
        JTPOCKET *pe = GetBentleyGadgetFromId(bentley, k);
        if ((unsigned int)(bentley->jtthrow.state - 4) < 3) {
            SetJtThrowState(&bentley->jtthrow, 7);
            return;
        }
        if (pe->pcid100) {
            SetJtGrabState(&bentley->jtgrab, 0xB, pe->pcid100, 0);
            return;
        }
        bentley->ppocketCur = pe;
        if (k == 7 || k == 11) {
            struct O *object = AcquireProxyObject(pe->proxy, 0);
            if (!object)
                return;
            object->x15C = pe->idGadget;
            if (IsThrowableGadget(object)) {
                object->p16C = bentley;
                SetGadgetRecordObject(bentley, pe, object);
            }
            bentley->jtthrow.ploCarried = (LO *)object;
            GetGadgetButtonMask(bentley, k);
            SetBentleyState(bentley, BENTLEY_THROW);
            SetJtThrowState(&bentley->jtthrow, 4);
            return;
        }
        ChargeGadgetMp(bentley, k);
        SetBentleyState(bentley, BENTLEY_GADGET);
        return;
    }
    case 10:
        ChargeGadgetMp(bentley, k);
        SetBentleyState(bentley, BENTLEY_ADRENALINE);
        return;
    case 8:
        bentley->jtgrab.gadgetId = k;
        bentley->pasegGadget = bentley->pasegGadgetUse;
        SetJtGrabState(&bentley->jtgrab, 0xB, 0, 0);
        return;
    default:
        StartStepplayerGadget(bentley, k);
        return;
    }
}
} // namespace M_StartBentleyGadget

// BENTLEY slot 105: base, then enables feature 0x24D if he is the current player with gadget 0xC equipped, else
// disables it.
void BENTLEY::OnGadgetsChanged()
{
    BENTLEY *bentley = (BENTLEY *)this;
    OnStepplayerGadgetsChanged(bentley);
    if ((IsPoCurrent(bentley) != 0) && (GetGadgetButtonMask(bentley, 0xC) != 0)) {
        ShowSoGlobsById(bentley, 0x24D);
        return;
    }
    HideSoGlobsById(bentley, 0x24D);
}

// BENTLEY slot 103: returns "bentley", the save-variable namespace for his hpCur/mpCur/apukCur.
extern "C" char *GetBentleySaveVarRoot()
{
    return (char *)D_002A4AE8;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136CC0 {
int GetStepplayerGroundProbeFlags(BENTLEY *) __asm__("GetStepplayerGroundProbeFlags");
// BENTLEY slot 85: per-state ground-probe flags (0 on a pipe, 5 hovering, 0xF in adrenaline), else the STEPPLAYER base.
// Also known as: GrfBentleyXp (upstream readable / Sly 1 name; GrfBentleyXp__FP3ALO).
extern "C" int GetBentleyGroundProbeFlags(BENTLEY *bentley) {
    if (bentley->unknown204) return 0;
    switch (bentley->state) {
    case BENTLEY_PIPE: return 0;
    case BENTLEY_HOVER: return 5;
    case BENTLEY_ADRENALINE: return 0xf;
    }
    return GetStepplayerGroundProbeFlags(bentley);
}
} // namespace M_func_00136CC0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136D28 {
typedef int ti128 __attribute__((mode(TI)));
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(BENTLEY *, int, int, JTAIRBL *, int, int, int, ASEGBL **) __asm__("EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL");
// Initializes the six blended animation slots of the crosshair/aim helper (+0x1710).
// Also known as: InitBentleySub (upstream readable / Sly 1 name; InitBentleySub__FP10BENTLEYSUBP3ALOG9VU_VECTORff).
extern "C" void InitBentleyCrosshair(BTBLENDS *bentley, BENTLEY *a, ti128 v, float g0, float g1) {
    unsigned int i;
    bentley->pbentley = a;
    bentley->restPosition = v;
    bentley->unknown30 = g1;
    bentley->state = -1;
    for (i = 0; i < 6; i++) EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(bentley->pbentley, sizeof(bentley->abtblend[i].abl[0]), 2, bentley->abtblend[i].abl, 0, 0, 0, &bentley->abtblend[i].blendedAnimation);
}
} // namespace M_func_00136D28

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136DC0 {
int IsAsegaRetracted(void *p) __asm__("IsAsegaRetracted");
int GetButtonsPressedWithin__5POCTLUif(POCTL *pa, int grf, float g) __asm__("GetButtonsPressedWithin__5POCTLUif");
void ClearPoctlJoyPressed(POCTL *pa, int n) __asm__("ClearPoctlJoyPressed");
// Crosshair update: on button 0x80 when gadget 2 is usable, restarts it.
// Also known as: UpdateBentleySub (upstream readable / Sly 1 name; UpdateBentleySub__FP7BENTLEYi).
extern "C" void UpdateBentleyCrosshair(BTBLENDS *bentley, int k)
{
    if (bentley->pasega && IsAsegaRetracted(bentley->pasega))
        SetBentleyCrosshairState(bentley, -1);
    if (k != bentley->state && bentley->state != -1)
        SetBentleyCrosshairState(bentley, k);
    if (k != -1 && (bentley->state == -1 || bentley->unknown0C) &&
        GetButtonsPressedWithin__5POCTLUif(bentley->pbentley->controller, BUTTON_ATTACK, 0.1f)) {
        if (bentley->pbentley->CanUseGadget(2)) {
            ClearPoctlJoyPressed(bentley->pbentley->controller, BUTTON_ATTACK);
            SetBentleyCrosshairState(bentley, -1);
            SetBentleyCrosshairState(bentley, k);
        }
    }
    if (!(bentley->pbentley->controller->buttonsHeld & BUTTON_ATTACK))

        bentley->unknown14 = 1;
}
} // namespace M_func_00136DC0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136ED0 {
struct M { ASEGA *pasega; int f4; int f8; };
void DetachXfActById(ASEGA *, BENTLEY *, void *) __asm__("DetachXfActById");
// Crosshair message handler: message 9 kinds 0x16E/0x242.
extern "C" void HandleBentleyCrosshairMessage(BTBLENDS *bentley, int msg, struct M *pm) {
    if (msg != 9 || pm->pasega != bentley->pasega)
        return;
    switch (pm->f8) {
    case 0x16e: DetachXfActById(pm->pasega, bentley->pbentley, bentley->pbentley->unknown10B8); bentley->unknown10 = 1; break;
    case 0x242: bentley->unknown0C = 1; break;
    }
}
} // namespace M_func_00136ED0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00136F48 {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float a[4]; } QF;
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void ChooseJtAttackTarget(BENTLEY *, int, void *, int *, QF *, float, float, float) __asm__("ChooseJtAttackTarget");
qword ConvertPosBetweenLos(void *, BENTLEY *, qword) __asm__("ConvertPosBetweenLos");
void SetStepTargetHeadingFromVector(BENTLEY *, qword) __asm__("SetStepTargetHeadingFromVector");
void FixStepAngularVelocity(BENTLEY *) __asm__("FixStepAngularVelocity");
void RecalculateAsegBlend(void *) __asm__("RecalculateAsegBlend");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *, BENTLEY *, int, ASEGA **, float, float) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void DetachXfActById(ASEGA *, BENTLEY *, void *) __asm__("DetachXfActById");
void SetAsegaChannelWeight(ASEGA *, float) __asm__("SetAsegaChannelWeight");
// Crosshair state change.
// Also known as: SetBentleySubMode (upstream readable / Sly 1 name; SetBentleySubMode__FP7BENTLEYi).
extern "C" void SetBentleyCrosshairState(BTBLENDS *ps, int state)
{
    void *pv;
    float dt;
    if (state == ps->state)
        return;
    if (ps->state == -1) {
        int f;
        QF v;
        QF r;
        float g;
        ps->startTime = g_clock.time;
        ps->unknown0C = 0;
        ps->unknown10 = 0;
        ps->unknown14 = 0;
        g = 1.5f;
        if (state == 0)
            g = 3.15f;
        ChooseJtAttackTarget(ps->pbentley, 2, &ps->restPosition, &f, &v, ps->pbentley->targetHeading, 0.15f, g);
        if (f) {
            ps->blend = Clamp((v.a[2] - ps->unknown30) / (ps->unknown34 - ps->unknown30), 0.0f, 1.0f);
        } else {
            ps->blend = 0.5f;
        }
        r.q = ConvertPosBetweenLos(ps->ploRef, ps->pbentley, D_002A2DD0);
        ps->side = 0.0f < r.a[1] ? 1 : 0;
        if (state == 0 && f) {
            SetStepTargetHeadingFromVector(ps->pbentley, v.q);
            FixStepAngularVelocity(ps->pbentley);
        }
    }
    ps->state = state;
    dt = 0.0f;
    switch (state) {
    case -1:
        pv = 0;
        break;
    case 0:
    case 1:
    case 2: {
        BTBLEND *pe = &ps->abtblend[state * 2 + ps->side];
        pe->abl[0].weight = 1.0f - ps->blend;
        pv = pe->blendedAnimation;
        pe->abl[1].weight = ps->blend;
        dt = 0.06666667f;
        RecalculateAsegBlend(pv);
        break;
    }
    }
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(pv, ps->pbentley, 1, &ps->pasega, g_clock.time - ps->startTime, 1.0f);
    if (!ps->pasega)
        return;
    if (ps->unknown10)
        DetachXfActById(ps->pasega, ps->pbentley, ps->pbentley->unknown10B8);
    if (ps->pasega && dt != 0.0f)
        SetAsegaChannelWeight(ps->pasega, g_clock.time + dt);
}
} // namespace M_func_00136F48

// Initializes the blend/helper record (players.h BTBLENDS): owner and state -1. (inferred)
// Also known as: InitSlyDodge (upstream readable / Sly 1 name; InitSlyDodge__FP3SLYi).
extern "C" void InitBentleyHelper(BTBLENDS *blends, BENTLEY *bentley)
{
    blends->pbentley = bentley;
    blends->state = -1;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001371C8 {
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
// When the owner's current action has retracted, returns him to generic player state 0 (SetPlayerState).
// Also known as: UpdateSlyDodgeTransitions (upstream readable / Sly 1 name; UpdateSlyDodgeTransitions__FP3SLY).
extern "C" void EndBentleyHelperWhenRetracted(JTREC *prec) {
    if (IsAsegaRetracted(prec->player->activeAnimation)) {
        prec->player->SetPlayerState(0);
    }
}
} // namespace M_func_001371C8

// Turns Bentley toward his interact target, if any.
// Also known as: UpdateSlyDodgeInput (upstream readable / Sly 1 name; UpdateSlyDodgeInput__FP3SLY).
extern "C" void FaceBentleyInteractTarget(BTBLENDS *blends)
{
    BENTLEY *bentley;
    LO *lo;
    bentley = blends->pbentley;
    lo = bentley->interactTarget;
    if (lo != 0) {
        FaceStepToward(bentley, lo);
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00137248 {
// CorrectBentleyRideVelocity (Claude agent laneB24, 2026-10-23): matched. len as an mtc1 asm from the qword (gcc emits the qmfc2), cz via a pextuw asm with a float "=r" output (gcc emits the mtc1).
typedef int qword __attribute__((mode(TI)));
union UQ { qword q; };
struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
struct VU_FLOATM {
    qword q;
    // Broadcast the float f (read from memory) into a VU0 register.
    VU_FLOATM(const float &f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "rV"(*(int *)&f)); }
};
// Dot product of the xyz parts of a and b (x lane).
static inline qword Dot(qword a, qword b)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
        : "=j"(r), "=&j"(one) : "j"(a), "j"(b));
    return r;
}
// Length of the xyz part of a (x lane).
static inline qword Length(qword a)
{
    qword r, t, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
        : "=&j"(t), "=&j"(one) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(t));
    return r;
}
// Return v scaled by f.x.
static inline qword Scale(qword v, qword f)
{
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f), "j"(v));
    return r;
}
// Divide v by f.
static inline qword DivF(qword v, VU_FLOATM f)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(f.q));
    return r;
}
// Return a + v * f.
static inline qword AddScaled(qword a, qword v, VU_FLOAT f)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(v), "j"(f.q));
    return r;
}
// Rotate v by the 3x3 matrix m.
static inline qword MulM3(const MATRIX3Q &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(v), "j"(m.r[0].q)); /* extra operand: allocation only */
    return r;
}

// Divide v by len, or set it to vDef when len is below 1e-4.
static inline void SafeDiv(UQ &v, qword len, const qword &vDef)
{
    if (VuToFloat(len) < 0.0001f)
        v.q = vDef;
    else
        v.q = VuDivByX(v.q, len);
}


extern float D_002BC998 __asm__("D_002BC998");
extern float D_002BC99C __asm__("D_002BC99C");
extern float D_002BC9A0 __asm__("D_002BC9A0");
extern SMP D_002BC9A8 __asm__("D_002BC9A8");
extern qword g_normalZ __asm__("g_normalZ"), D_002A2DA0 __asm__("D_002A2DA0");
qword GetLoWorldPos(LO *p) __asm__("GetLoWorldPos");
float SmoothValue__FfffP3SMPPf(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext) __asm__("SmoothValue__FfffP3SMPPf");
float ClampAbs__Fff(float g, float gMax) __asm__("ClampAbs__Fff");
MATRIX3Q BuildAxisAngleRotationMatrix(qword axis, float g) __asm__("BuildAxisAngleRotationMatrix");
qword LimitVectorLength(qword v, float g) __asm__("LimitVectorLength");
void AddSoAcceleration(STEPPLAYER *p, qword v) __asm__("AddSoAcceleration");


// Corrects Bentley's relative velocity and position against the object he rides. (inferred)
// Also known as: UpdateSlyDodgeMotion (upstream readable / Sly 1 name; UpdateSlyDodgeMotion__FP3SLY).
extern "C" void CorrectBentleyRideVelocity(JTREC *s)
{
    STEPPLAYER *p = s->player;
    SO *py = (SO *)p->interactTarget;
    if (py == 0)
        return;
    float maxValue = D_002BC99C * D_002BC99C / D_002BC998 + D_002BC9A0;
    qword dv = VuSub(p->GetVelocity(), py->GetVelocity());
    UQ n;
    qword d0 = VuSub(GetLoWorldPos(s->player), GetLoWorldPos(py));
    n.q = VuSub(d0, Scale(g_normalZ, Dot(d0, g_normalZ)));
    qword qlen = Length(n.q);
    SafeDiv(n, qlen, D_002A2DA0);
    qword c = VuCross(n.q, dv);
    float len = QwordX(qlen);
    float cz = QwordZ(c);
    float dot = VuToFloat(Dot(dv, n.q));
    float w = cz / len;
    float dgNext;
    SmoothValue__FfffP3SMPPf(len, D_002BC998, g_clock.deltaTime, &D_002BC9A8, &dgNext);
    float g = ClampAbs__Fff(dgNext - dot, D_002BC9A0 * g_clock.deltaTime);
    qword t = AddScaled(dv, n.q, VU_FLOAT(g));
    MATRIX3Q mc;
    MATRIX3Q m = BuildAxisAngleRotationMatrix(g_normalZ, w * g_clock.deltaTime);
    mc = m;
    qword r = DivF(VuSub(MulM3(mc, t), dv), VU_FLOATM(g_clock.deltaTime));
    AddSoAcceleration(s->player, LimitVectorLength(r, maxValue));
}
} // namespace M_func_00137248

// Contact adjust hook: sets the contact's friction to D_002BC9B4.
extern "C" void SetBentleyXpFriction(void *pv, XP *xp)
{
    xp->gFriction = D_002BC9B4;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001374F0 {
typedef int qword __attribute__((mode(TI)));
qword SetVectorCylind(float a, float b, float c) __asm__("SetVectorCylind");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// With a target and a positive range: 1 if the target lies behind Bentley's facing, 0 if in front; otherwise returns
// the record's stored flag. (inferred)
// Also known as: SelectSlyDodgeSide (upstream readable / Sly 1 name; SelectSlyDodgeSide__FP3SLY).
extern "C" int IsBentleyHelperTargetBehind(BTBLENDS *bentley)
{
    float zero = 0.0f;
    POCTL *pq = bentley->pbentley->controller;
    if (zero < pq->leftStickDeflection && bentley->pbentley->interactTarget) {
        qword v = SetVectorCylind(pq->leftStickAngle - 1.5707964f, 1.0f, zero);
        qword a = GetLoWorldPos(bentley->pbentley->interactTarget);
        qword b, d, t;
        float g;
        __asm__("" : "+j"(a));
        b = GetLoWorldPos(bentley->pbentley);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        __asm__("vmul.xyz %0, %2, %3\n"
            "vaddw.x %1, $vf0, $vf0w\n"
            "vadday.x ACC, %0, %0y\n"
            "vmaddz.x %0, %1, %0z" : "=j"(d), "=j"(t) : "j"(v), "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
        if (g > zero) return 0; return 1;
    }
    return bentley->unknown0C;
}
} // namespace M_func_001374F0

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001375D0 {
// MATCH (Claude agent, laneB7, 2026-10-23), from fresh3 (2). One commented register pin: vel pinned to vf2 in the VU_FLOATM operator*, and the allocation-only duplicate g operand is "jm" (memory allowed, so it needs no second reload of the spilled g). The VU_FLOATM ctor is asm("lw") from "m"(f).
struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
struct VU_VECTOR {
    qword q;
    // Wrap a qword as a VU_VECTOR.
    VU_VECTOR(const qword &v) { q = v; }
};
struct VU_FLOATM {
    qword q;
    // Load a float's bits into a GPR qword (lw).
    VU_FLOATM(const float &f) { __asm__("lw %0, %1" : "=r"(q) : "m"(f)); }
};
// Return v scaled by f.
static inline qword operator*(VU_VECTOR v, VU_FLOATM f)
{
    qword r;
    register qword vv __asm__("vf2") = v.q; /* register pin: vel must be vf2 while g's reload and the output share vf1 */
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(vv), "j"(f.q), "jm"(f.q));
    return r;
}
// Return v scaled by f.
static inline qword operator*(VU_VECTOR v, VU_FLOAT f)
{
    qword r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v.q), "j"(f.q));
    return r;
}
/* The target orientation rows (STEP::aqwTarget) as the rotate operator reads them. */
struct M3Q { qword r[3]; };
// Rotate v by the 3x3 matrix m.
static inline qword operator*(const M3Q &m, VU_VECTOR v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(v.q));
    return r;
}

struct Q {
    virtual void v1();
    virtual void v2();
    virtual void Eval(int n, qword *pv, float g0, float g1);
};
struct R { int x0; Q *pq; };

extern float D_002BC9B4 __asm__("D_002BC9B4");   /* contact friction */
extern float D_002BC9B8 __asm__("D_002BC9B8");
void FaceStepToward(STEPPLAYER *p, LO *plo) __asm__("FaceStepToward");
CHN *FindAsegChannel(ASEG *px, STEPPLAYER *p, int a, int b) __asm__("FindAsegChannel");
void ReleaseSma(SMA **ppsma) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *px, STEPPLAYER *p, int n, ASEGA **ppasega, float g0, float g1) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");


// Helper state change: plays the state's clip and, on leaving, scales the velocity. (inferred)
// Also known as: SetSlyDodgeState (upstream readable / Sly 1 name; SetSlyDodgeState__FP3SLYi).
extern "C" void SetBentleyHelperState(JTREC *s, int k)
{
    ASEG *px;
    if (k == s->state)
        return;
    s->state = k;
    px = 0;
    s->stateStartTime = g_clock.time;
    if (k == -1) {
        VU_FLOATM g(D_002BC9B8);
        qword vel = s->player->GetVelocity();
        qword r = VU_VECTOR(vel) * g;
        s->player->SetVelocity(r);
        return;
    }
    if (k >= -1)
        if (k < 2) {
            union {
                qword q;
            } v;
            if (k == 0)
                px = s->apaseg[0];
            else
                px = s->apaseg[1];
            FaceStepToward(s->player, s->player->interactTarget);
            FindAsegChannel(px, s->player, 1, 1)->pac->Evaluate(0.0f, px->masterSpeed, 0, &v.q);
            s->player->SetVelocity(*(M3Q *)s->player->aqwTarget * VU_VECTOR(v.q));
            s->player->qwUnknown190 = VU_VECTOR(v.q) * VU_FLOAT(-1.0f);
            s->side = k;
        }
    ReleaseSma(&s->player->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(px, s->player, 1, &s->player->activeAnimation, 0.0f, 1.0f);
}
} // namespace M_func_001375D0


// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const char r_D_002A4AE8[] __asm__("D_002A4AE8") = "bentley"; }

// ---- bentley's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
extern "C" {
SNIP D_002BC390[0x76] = {
    {0x300, 0x1A0, SLY2_OFFSETOF(BENTLEY, gaitA.abl[0].animation)}, {0x300, 0x19E, SLY2_OFFSETOF(BENTLEY, gaitA.abl[1].animation)}, {0x300, 0x1A2, SLY2_OFFSETOF(BENTLEY, unknown1028)}, {0x300, 0x1BC, SLY2_OFFSETOF(BENTLEY, abldirA[0].animation)},
    {0x300, 0x1BD, SLY2_OFFSETOF(BENTLEY, abldirA[1].animation)}, {0x300, 0x1BE, SLY2_OFFSETOF(BENTLEY, abldirA[2].animation)}, {0x300, 0x1BF, SLY2_OFFSETOF(BENTLEY, abldirA[3].animation)}, {0x300, 0x1C0, SLY2_OFFSETOF(BENTLEY, abldirA[4].animation)},
    {0x300, 0x1C1, SLY2_OFFSETOF(BENTLEY, abldirA[5].animation)}, {0x300, 0x1C2, SLY2_OFFSETOF(BENTLEY, abldirA[6].animation)}, {0x300, 0x1C3, SLY2_OFFSETOF(BENTLEY, abldirA[7].animation)}, {0x300, 0x20A, SLY2_OFFSETOF(BENTLEY, pasegSubState0)},
    {0x300, 0x20B, SLY2_OFFSETOF(BENTLEY, pasegSubState2)}, {0x300, 0x20C, SLY2_OFFSETOF(BENTLEY, abldirB[0].animation)}, {0x300, 0x20D, SLY2_OFFSETOF(BENTLEY, abldirB[1].animation)}, {0x300, 0x20E, SLY2_OFFSETOF(BENTLEY, abldirB[2].animation)},
    {0x300, 0x20F, SLY2_OFFSETOF(BENTLEY, abldirB[3].animation)}, {0x300, 0x210, SLY2_OFFSETOF(BENTLEY, abldirB[4].animation)}, {0x300, 0x211, SLY2_OFFSETOF(BENTLEY, abldirB[5].animation)}, {0x300, 0x212, SLY2_OFFSETOF(BENTLEY, abldirB[6].animation)},
    {0x300, 0x213, SLY2_OFFSETOF(BENTLEY, abldirB[7].animation)}, {0x320, 0x215, SLY2_OFFSETOF(BENTLEY, unknown16B8)}, {0x300, 0x244, SLY2_OFFSETOF(BENTLEY, btblends.ploRef)}, {0x300, 0x230, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[0].abl[0].animation)},
    {0x300, 0x231, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[0].abl[1].animation)}, {0x300, 0x232, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[1].abl[0].animation)}, {0x300, 0x233, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[1].abl[1].animation)}, {0x300, 0x234, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[2].abl[0].animation)},
    {0x300, 0x235, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[2].abl[1].animation)}, {0x300, 0x236, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[3].abl[0].animation)}, {0x300, 0x237, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[3].abl[1].animation)}, {0x300, 0x238, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[4].abl[0].animation)},
    {0x300, 0x239, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[4].abl[1].animation)}, {0x300, 0x23A, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[5].abl[0].animation)}, {0x300, 0x23B, SLY2_OFFSETOF(BENTLEY, btblends.abtblend[5].abl[1].animation)}, {0x300, 0x22E, SLY2_OFFSETOF(BENTLEY, unknown16C4[0])},
    {0x300, 0x22F, SLY2_OFFSETOF(BENTLEY, unknown16C4[1])}, {0x300, 0x1E9, SLY2_OFFSETOF(BENTLEY, gaitB.abl[0].animation)}, {0x300, 0x1E8, SLY2_OFFSETOF(BENTLEY, gaitB.abl[1].animation)}, {0x300, 0x1DE, SLY2_OFFSETOF(BENTLEY, jtair.pasegLaunch)},
    {0x300, 0x1DF, SLY2_OFFSETOF(BENTLEY, apasegHover[0])}, {0x300, 0x1E0, SLY2_OFFSETOF(BENTLEY, apasegHover[1])}, {0x320, 0x25D, SLY2_OFFSETOF(BENTLEY, jtair.precAir)}, {0x320, 0x25E, SLY2_OFFSETOF(BENTLEY, jtair.precAirWallRun)},
    {0x300, 0x24E, SLY2_OFFSETOF(BENTLEY, jtair.apasegJump[0])}, {0x300, 0x24F, SLY2_OFFSETOF(BENTLEY, jtair.apasegJump[3])}, {0x300, 0x250, SLY2_OFFSETOF(BENTLEY, jtair.apasegJump[4])}, {0x300, 0x251, SLY2_OFFSETOF(BENTLEY, jtair.apasegJump[2])},
    {0x300, 0x252, SLY2_OFFSETOF(BENTLEY, jtair.apasegJump[1])}, {0x300, 0x253, SLY2_OFFSETOF(BENTLEY, jtair.apasegDoubleJump[0])}, {0x300, 0x254, SLY2_OFFSETOF(BENTLEY, jtair.apasegDoubleJump[3])}, {0x300, 0x255, SLY2_OFFSETOF(BENTLEY, jtair.apasegDoubleJump[4])},
    {0x300, 0x256, SLY2_OFFSETOF(BENTLEY, jtair.apasegDoubleJump[2])}, {0x300, 0x257, SLY2_OFFSETOF(BENTLEY, jtair.apasegDoubleJump[1])}, {0x300, 0x258, SLY2_OFFSETOF(BENTLEY, jtair.pasegEdge)}, {0x300, 0x259, SLY2_OFFSETOF(BENTLEY, jtair.pasegKindE)},
    {0x300, 0x25A, SLY2_OFFSETOF(BENTLEY, jtair.pasegKindEDouble)}, {0x320, 0x284, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown54)}, {0x320, 0x285, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown58)}, {0x320, 0x286, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown5C)},
    {0x320, 0x287, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown60)}, {0x300, 0x266, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[0].animation)}, {0x300, 0x262, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[1].animation)}, {0x300, 0x268, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[2].animation)},
    {0x300, 0x264, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[3].animation)}, {0x300, 0x269, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[4].animation)}, {0x300, 0x265, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[5].animation)}, {0x300, 0x267, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[6].animation)},
    {0x300, 0x263, SLY2_OFFSETOF(BENTLEY, jtgrab.abldirA[7].animation)}, {0x300, 0x25F, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown174[0])}, {0x300, 0x260, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown174[1])}, {0x300, 0x261, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown174[2])},
    {0x300, 0x27D, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown510[0])}, {0x300, 0x25B, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown4F0[0])}, {0x300, 0x25C, SLY2_OFFSETOF(BENTLEY, jtgrab.unknown4F0[1])}, {0x300, 0x622, SLY2_OFFSETOF(BENTLEY, jtgrab.pproxyMarker)},
    {0x320, 0x297, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[0])}, {0x320, 0x298, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[1])}, {0x320, 0x299, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[2])}, {0x320, 0x29A, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[3])},
    {0x320, 0x29B, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[4])}, {0x320, 0x29C, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[5])}, {0x320, 0x29D, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[6])}, {0x320, 0x29E, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[7])},
    {0x320, 0x29F, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[8])}, {0x320, 0x2A0, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[9])}, {0x320, 0x2A1, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[10])}, {0x320, 0x2A2, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[11])},
    {0x320, 0x2A3, SLY2_OFFSETOF(BENTLEY, jthurt.aprecHurt[12])}, {0x300, 0x289, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[0])}, {0x300, 0x28A, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[1])}, {0x300, 0x28B, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[2])},
    {0x300, 0x28C, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[3])}, {0x300, 0x28D, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[4])}, {0x300, 0x28E, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[5])}, {0x300, 0x28F, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[6])},
    {0x300, 0x290, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[7])}, {0x300, 0x291, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[8])}, {0x300, 0x292, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[9])}, {0x300, 0x293, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[10])},
    {0x300, 0x294, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[11])}, {0x300, 0x295, SLY2_OFFSETOF(BENTLEY, jthurt.unknownA0[12])}, {0x300, 0xFC, SLY2_OFFSETOF(BENTLEY, aploLandEffect[0])}, {0x300, 0xFD, SLY2_OFFSETOF(BENTLEY, aploLandEffect[1])},
    {0x100, 0x247, SLY2_OFFSETOF(BENTLEY, aproxyDart[0])}, {0x100, 0x248, SLY2_OFFSETOF(BENTLEY, aproxyDart[1])}, {0x100, 0x249, SLY2_OFFSETOF(BENTLEY, launcher)}, {0x300, 0x5E9, SLY2_OFFSETOF(BENTLEY, jtmode.pasegStart)},
    {0x300, 0x5EA, SLY2_OFFSETOF(BENTLEY, jtmode.pasegEnd)}, {0x300, 0x217, SLY2_OFFSETOF(BENTLEY, apocket[0].proxy)}, {0x300, 0x217, SLY2_OFFSETOF(BENTLEY, apocket[1].proxy)}, {0x300, 0x217, SLY2_OFFSETOF(BENTLEY, apocket[2].proxy)},
    {0x300, 0x21A, SLY2_OFFSETOF(BENTLEY, apocket[3].proxy)}, {0x300, 0x217, SLY2_OFFSETOF(BENTLEY, apocket[4].proxy)}, {0x300, 0x21F, SLY2_OFFSETOF(BENTLEY, apocket[5].proxy)}, {0x300, 0x137, SLY2_OFFSETOF(BENTLEY, unknown1820)},
    {0x300, 0x155, SLY2_OFFSETOF(BENTLEY, pasegGadgetUse)}, {0x300, 0x24C, SLY2_OFFSETOF(BENTLEY, pasegAdrenaline)},
};
} // extern "C"
namespace M_PostBentleyLoad {
VQ D_002BC920 __asm__("D_002BC920") = {{175.0f, 0.0f, 0.0f, 0.0f}};
} // namespace M_PostBentleyLoad extern "C"
namespace M_UpdateBentleyStateMachine {
float D_002BC930 __asm__("D_002BC930") = 2.0f;
} // namespace M_UpdateBentleyStateMachine extern "C"
namespace M_UpdateBentley {
float D_002BC934 __asm__("D_002BC934") = 0.5f;
struct POLY D_002BC940 __asm__("D_002BC940") __attribute__((aligned(16))) = {-1.0f, 2.0f, 0.0f};
struct POLY D_002BC950 __asm__("D_002BC950") = {1.0f, 0.0f, 0.0f};
float D_002BC960 __asm__("D_002BC960") __attribute__((aligned(16))) = 5.0f;
} // namespace M_UpdateBentley extern "C"
namespace M_func_001356E8 {
VQ D_002BC970 __asm__("D_002BC970") = {{0.0f, 0.0f, 10.0f, 0.0f}};
VQ D_002BC980 __asm__("D_002BC980") = {{0.0f, 0.0f, 40000.0f, 0.0f}};
} // namespace M_func_001356E8 extern "C"
namespace M_func_00135940 {
float D_002BC990 __asm__("D_002BC990") = 2.5f;
} // namespace M_func_00135940 extern "C"
namespace M_UpdateBentleyAim {
float D_002BC994 __asm__("D_002BC994") = 2.0f;
} // namespace M_UpdateBentleyAim extern "C"
namespace M_func_00137248 {
float D_002BC998 __asm__("D_002BC998") = 250.0f;
float D_002BC99C __asm__("D_002BC99C") = 1500.0f;
float D_002BC9A0 __asm__("D_002BC9A0") = 1500.0f;
SMP D_002BC9A8 __asm__("D_002BC9A8") = {500.0f, 0.0f, 0.5f};
} // namespace M_func_00137248 extern "C"
namespace M_func_001375D0 {
float D_002BC9B4 __asm__("D_002BC9B4") = 0.0f;
float D_002BC9B8 __asm__("D_002BC9B8") = 0.5f;
}
