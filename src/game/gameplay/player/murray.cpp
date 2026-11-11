// murray.cpp -- Murray (MURRAY : STEPPLAYER, players.h): init and load, the state machine (SetMurrayState), carrying
// and throwing, attacks, the per-frame update and the Splice-callable state setters. TU 0x1B4728..0x1B9F80
// (66 functions, address order).
// Entry points: InitMurray/LoadMurrayFromBrx/PostMurrayLoad, UpdateMurray (per frame), SetMurrayState,
// HandleMurrayMessage, the attack combo (SetMurrayAttack, UpdateMurrayAttack), lift/carry/throw (UpdateMurrayLift,
// ThrowMurrayCarried), the Splice setters and the STEPPLAYER virtual overrides.
//
// Types: sly2/players.h's MURRAY for the m2c bodies (one unnamed word, +0x1820, through a macro as stepbhv.cpp
// does). The hand-written per-function files keep their views in namespaces named after each function (as jt.cpp
// does), their C++-named definitions as C-linkage definitions of their literal symbols there.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_murray
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_murray

#include "sly2/basic.h"
#include "sly2/stepplayer.h"
#include "sly2/players.h"
#include "sly2/slotheap.h"
#include "sly2/cm.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
#include "sly2/act.h"
#include "player_input.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/game.h"
#include "sly2/clock.h"

// The embedded movement components of a player (JTAIR at +0x2C0, JTGRAB +0x4C0, ... JTTHROW +0x11B0) are taken from
// the real STEPPLAYER layout in sly2/stepplayer.h; the per-function views only declare the words they read.
static inline STEPPLAYER *AsStepplayer(void *player) { return (STEPPLAYER *)player; }

// Murray's state (MURRAY +0x13D0), switched by SetMurrayState and run by UpdateMurrayStateMachine. Meanings from
// runtime observation (docs/systems/player.md section 4) except where marked.
enum MurrayState {
    MURRAY_IDLE = 0,
    MURRAY_RUN = 1,              // run button held
    MURRAY_JUMP = 2,             // JTAIR
    MURRAY_FLOP = 3,             // air slam
    MURRAY_LIFT = 4,             // lifting an object or a stunned guard
    MURRAY_CARRY = 5,
    MURRAY_STRUGGLE = 6,         // scripted struggle
    MURRAY_THROW = 7,            // throw the carried object
    MURRAY_TOSS = 8,             // toss / Diablo Fire Slam
    MURRAY_STRAFE = 9,           // strafe / lock-on
    MURRAY_WALK = 10,            // full stick without the run button
    MURRAY_ATTACK = 11,          // punch combo, uppercut, takedown finisher
    MURRAY_POWER_MOVE = 12,      // Berserker Charge
    MURRAY_GRAB = 13,            // JTGRAB (wall press, ...)
    MURRAY_HURT = 14,            // JTHURT
    MURRAY_AIMED_THROW = 15,     // JTTHROW component at +0x1910 (inferred)
    MURRAY_PLAYER_STATE_4 = 16,  // entered for generic player state 4 (inferred; meaning unknown)
    MURRAY_SCRIPT_ANIM = 17,     // scripted animation, generic player state 5 (inferred)
    MURRAY_SCRIPT_SMA = 18,      // scripted state-machine animation, generic player state 6 (inferred)
    MURRAY_SCRIPT_THROW = 19,    // scripted throw, generic player state 7 (inferred)
    MURRAY_PIPE = 20,            // JTPIPE
    MURRAY_TURNBUCKLE = 21,      // Turnbuckle Launch gadget
};

// 0x1B9E28 StaticInit_murray and GlobalConstructors_murray: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

extern "C" {
extern const char D_002A75E0[];   /* "murray" */
void SetMurrayState(MURRAY *pmurray, int nState, void *pv);
}

/* The carry target MURRAY::ploCarried points at is a carry grab spot (classes.h CID36): unknown180 is the object it
   holds (the one Murray lifts), plo2 / plo3 the objects whose transforms are the hands' IK goals. */

namespace M_InitMurray {
/* InitMurray: actor defaults init. The int -1 stores come before the float stores in source; gcc schedules them so strafeSubstate lands right after baseRadius. (Claude agent) */
extern MURRAY *D_002F7900 __asm__("D_002F7900");
void InitStepplayer(MURRAY *p) __asm__("InitStepplayer");
void InitGait(GAIT *a, MURRAY *owner, int sfxidStep) __asm__("InitGait");
void InitAsegArc(void *a) __asm__("InitAsegArc");
void InitThrow(JTTHROW *a, MURRAY *owner) __asm__("InitThrow");
// Initialise the smoothing record: fast duration f, max speed change g, slow speed 0.
extern "C" { static inline void Init(SMPA *ps, float f, float g) { ps->maxSpeedChange = g; ps->fastDuration = f; ps->slowSpeed = 0.0f; } }
// MURRAY Init (vtable slot 7): STEPPLAYER init, then Murray's defaults (state fields -1, throw range 1500/3500) and his
// four locomotion blends.
// Also known as: InitMurray (upstream readable / Sly 1 name; InitMurray__FP6MURRAY).
extern "C" void InitMurray(MURRAY *murray)
{
    InitStepplayer(murray);
    murray->baseRadius = 50.0f;
    murray->flopSubstate = -1;
    murray->attackSubstate = -1;
    murray->subState = -1;
    murray->state = -1;
    murray->liftClip = -1;
    murray->liftSubstate = -1;
    murray->carrySubstate = -1;
    murray->strafeSubstate = -1;
    murray->throwRange = 1500.0f;
    murray->throwSpeed = 3500.0f;
    murray->grabRadius = 60.0f;
    murray->unknown1048 = 125.0f;
    InitGait(&murray->gaitA, murray, 0x15);
    InitGait(&murray->gaitB, murray, 0x15);
    InitGait(&murray->gaitD, murray, 0x15);
    InitGait(&murray->gaitC, murray, 0x15);
    InitAsegArc(&murray->arcLeap);
    {
    register float a __asm__("$f6") = -60.0f;
    register float b __asm__("$f4") = -100.0f;
    register float c __asm__("$f0") = 100.0f;
    register float d __asm__("$f1") = -10.0f;
    register float e __asm__("$f2") = 250.0f;
    register float f __asm__("$f5") = 1.0f;
    register float g __asm__("$f3") = 4500.0f;
    murray->unknown1AC8=a; murray->releaseTime=a; murray->waterProbe[0]=b; murray->waterProbe[1]=c; murray->waterProbe[2]=50.0f; murray->waterProbe[3]=d; murray->unknown10A8.fastSpeed=e;
    Init(&murray->unknown10A8, f, g);
    }
    InitThrow(&murray->jtthrowAim, murray);
    murray->grfGrabTarget = 4;
    D_002F7900 = murray;
}
} // namespace M_InitMurray

namespace M_LoadMurrayFromBrx {
extern char D_002C0270[] __asm__("D_002C0270");
void LoadStepFromBrx(MURRAY *p) __asm__("LoadStepFromBrx");
void BindLoOidTable(MURRAY *p, int n, void *q) __asm__("BindLoOidTable");
PARTLINK *AllocClearedFromWorldHeap(struct SWALLOC *r) __asm__("AllocClearedFromWorldHeap");
void *FindSoXf(MURRAY *p, int a, int b) __asm__("FindSoXf");
void InitIkRecord(PARTLINK *t) __asm__("InitIkRecord");
// Allocate a zeroed 0xE0-byte PARTLINK, 16-byte aligned, from the world heap.
extern "C" { static __inline__ PARTLINK *NewT(void) { struct SWALLOC req; req.cb = 0xE0; req.cbAlign = 0x10; return AllocClearedFromWorldHeap(&req); } }
// MURRAY LoadFromBrx (slot 14): STEP base, binds his object table (0x89 entries) and allocates the two hand/grip
// records. Layout parse-verified: a parser following it reads every byte of 4 levels.
extern "C" void LoadMurrayFromBrx(MURRAY *murray)
{
    LoadStepFromBrx(murray);
    BindLoOidTable(murray, 0x89, D_002C0270);
    murray->ikChainA = NewT();
    murray->ikChainA->precA = FindSoXf(murray, 0x200, 6);
    murray->ikChainA->precB = FindSoXf(murray, 0x201, 6);
    murray->ikChainA->precC = FindSoXf(murray, 0x202, 1);
    InitIkRecord(murray->ikChainA);
    murray->ikChainB = NewT();
    murray->ikChainB->precA = FindSoXf(murray, 0x203, 6);
    murray->ikChainB->precB = FindSoXf(murray, 0x204, 6);
    murray->ikChainB->precC = FindSoXf(murray, 0x205, 1);
    InitIkRecord(murray->ikChainB);
    murray->ikEndA = murray->ikChainA->precC;
    murray->unknown15F4 = FindSoXf(murray, 0x202, 6);
    murray->ikEndB = murray->ikChainB->precC;
    murray->unknown15FC = FindSoXf(murray, 0x205, 6);
}
} // namespace M_LoadMurrayFromBrx

namespace M_PostMurrayLoad {
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DE0 __asm__("D_002A2DE0");
void PostLoadStepplayer(MURRAY *p) __asm__("PostLoadStepplayer");
void PostJtLoadGait(void *p) __asm__("PostJtLoadGait");
void InitDirectionalBlend(MURRAY *p, void *a, void *b) __asm__("InitDirectionalBlend");
void PostAsegArcLoad(void *p, MURRAY *s) __asm__("PostAsegArcLoad");
void PostJtThrowLoad(void *p) __asm__("PostJtThrowLoad");
void AttachStepplayerToStartMarker(MURRAY *p, int n) __asm__("AttachStepplayerToStartMarker");
void *FindSoXf(MURRAY *p, int a, int b) __asm__("FindSoXf");
struct ACT1 *NewPositionAct(MURRAY *p, void *a, int b, int c, int d, qword q) __asm__("NewPositionAct");
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
// MURRAY post-load (slot 15): STEPPLAYER post-load, the four locomotion blends and the strafe directional blends
// (InitDirectionalBlend).
// Also known as: PostMurrayLoad (upstream readable / Sly 1 name; PostMurrayLoad__FP6MURRAY).
extern "C" void PostMurrayLoad(MURRAY *murray)
{
    void *v;
    PostLoadStepplayer(murray);
    PostJtLoadGait(&murray->gaitA);
    PostJtLoadGait(&murray->gaitB);
    PostJtLoadGait(&murray->gaitD);
    PostJtLoadGait(&murray->gaitC);
    InitDirectionalBlend(murray, murray->unknown16F0, &murray->unknown17F0);
    PostAsegArcLoad(&murray->arcLeap, murray);
    PostJtThrowLoad(&murray->jtthrowAim);
    AttachStepplayerToStartMarker(murray, 0x16);
    v = FindSoXf(murray, murray->oid, 0xC);
    if (v) {
        murray->pact1Unk1acc = NewPositionAct(murray, v, 0, 7, 2, D_002A2DE0);
    }
    SetMurrayState(murray, MURRAY_IDLE);
}
} // namespace M_PostMurrayLoad

namespace M_UpdateMurrayStateMachine {
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
extern float D_002C08DC __asm__("D_002C08DC"),D_002C08E0 __asm__("D_002C08E0");
extern void SetJtAirJump(JTAIR*,int,void*,void*) __asm__("SetJtAirJump");
extern void StartJtMode(JTMODE*,int,void*,void*) __asm__("StartJtMode");
extern int IsStepplayerNearInteractTarget(MURRAY*,int) __asm__("IsStepplayerNearInteractTarget");
extern void SetMurrayState(MURRAY*,int) __asm__("SetMurrayState");
extern int CanMurrayAttack(MURRAY*) __asm__("CanMurrayAttack");
extern void StartMurrayAltAttack(MURRAY*) __asm__("StartMurrayAltAttack");
extern void StartMurrayPunch(MURRAY*) __asm__("StartMurrayPunch");
extern int TryMurrayTakedown(MURRAY*) __asm__("TryMurrayTakedown");
extern int TryGrabJtNearestCandidate(JTGRAB*,unsigned,int) __asm__("TryGrabJtNearestCandidate");
extern void UpdateJtAirInput(JTAIR*) __asm__("UpdateJtAirInput");
extern int WillStepplayerLand(MURRAY*,float) __asm__("WillStepplayerLand");
extern void UpdateJtGrabInput(JTGRAB*) __asm__("UpdateJtGrabInput");
extern void UpdateJtHurtInput(JTHURT*) __asm__("UpdateJtHurtInput");
extern void UpdateMurrayFlop(MURRAY*) __asm__("UpdateMurrayFlop");
extern void UpdateMurrayCarry(MURRAY*) __asm__("UpdateMurrayCarry");
extern void UpdateMurrayStruggle(MURRAY*) __asm__("UpdateMurrayStruggle");
extern void UpdateMurrayLift(MURRAY*) __asm__("UpdateMurrayLift");
extern int IsAsegaRetracted(ASEGA*) __asm__("IsAsegaRetracted");
extern int IsSmaRetracted(SMA*) __asm__("IsSmaRetracted");
extern void UpdateMurrayAttack(MURRAY*) __asm__("UpdateMurrayAttack");
extern void UpdateMurrayStrafe(MURRAY*) __asm__("UpdateMurrayStrafe");
extern void UpdateMurrayPower(MURRAY*) __asm__("UpdateMurrayPower");
extern void UpdateJtThrow(JTTHROW*) __asm__("UpdateJtThrow");
extern void UpdateJtActivePipe(JTPIPE*) __asm__("UpdateJtActivePipe");
extern int GetGadgetButtonMask(MURRAY*,int) __asm__("GetGadgetButtonMask");
// Copy the PL pointer through a register move.
static inline POCTL *CopyPL(POCTL *source){POCTL *result;__asm__("daddu %0,%1,$zero":"=r"(result):"r"(source));return result;}
// Per-frame input-driven state machine (loops until the state is stable): idle/move, jump, attacks, lift/carry/throw,
// flop, strafe. Pad bit 0x400 is look mode; gadgets are the slot buttons 0x4/0x1/0x2.
// Also known as: UpdateMurrayTransitions (upstream readable / Sly 1 name; UpdateMurrayTransitions__FP6MURRAY).
extern "C" void UpdateMurrayStateMachine(MURRAY *murray)
{
    int state;
    do {
        state = murray->state;
        switch (state) {
        case MURRAY_JUMP:
        case MURRAY_FLOP:
        case MURRAY_LIFT:
        case MURRAY_STRUGGLE:
        case MURRAY_THROW:
        case MURRAY_TOSS:
        case MURRAY_HURT:
        case MURRAY_AIMED_THROW:
        case MURRAY_PLAYER_STATE_4:
        case MURRAY_SCRIPT_ANIM:
        case MURRAY_SCRIPT_SMA:
        case MURRAY_PIPE: break;
        default:
            if (g_clock.time - murray->unknown20C > 0.1f) {
                switch (murray->state) {
                case MURRAY_CARRY: SetJtAirJump(&murray->jtair, 16, 0, 0); break;
                case MURRAY_POWER_MOVE:
                    if (murray->subState == 3)
                        SetJtAirJump(&murray->jtair, 11, 0, 0);
                    break;
                case MURRAY_GRAB:
                    if (murray->jtgrab.state != 1 && murray->jtgrab.state != 12)
                        SetJtAirJump(&murray->jtair, 0, 0, 0);
                    break;
                default: SetJtAirJump(&murray->jtair, 0, 0, 0); break;
                }
            }
        }
        int finished;
        switch (murray->state) {
        case MURRAY_IDLE:
        case MURRAY_RUN:
        case MURRAY_WALK: {
            if (!(murray->controller->buttonsHeld & BUTTON_RUN) && IsStepplayerNearInteractTarget(murray, 0)) {
                SetMurrayState(murray, MURRAY_STRAFE);
                break;
            }
            if (murray->controller->ConsumeButtonsPressedWithin(BUTTON_JUMP, 0.1f)) {
                SetJtAirJump(&murray->jtair, 1, 0, 0);
                break;
            }
            if (murray->controller->GetButtonsPressedWithin(BUTTON_LOOK, 0.15f) && murray->CanUseGadget(3)) {
                murray->controller->ClearJoyPressed(BUTTON_LOOK);
                StartJtMode(&murray->jtmode, 0, 0, 0);
                break;
            }
            if (CanMurrayAttack(murray) && murray->controller->ConsumeButtonsPressedWithin(BUTTON_ALT_ATTACK, D_002C08DC)) {
                StartMurrayAltAttack(murray);
                break;
            }
            if (TryMurrayTakedown(murray))
                break;
            if (murray->controller->ConsumeButtonsPressedWithin(BUTTON_ACTION, 0.2f)) {
                if (!TryGrabJtNearestCandidate(&murray->jtgrab, 0xF8FA, 0) && murray->psoCarryRoot) {
                    murray->liftClip = 0;
                    SetMurrayState(murray, MURRAY_LIFT);
                }
                break;
            }
            if (CanMurrayAttack(murray) && murray->controller->ConsumeButtonsPressedWithin(BUTTON_ATTACK, D_002C08DC)) {
                StartMurrayPunch(murray);
                break;
            }
            POCTL *input = murray->controller;
            POCTL *flagsInput = CopyPL(input);
            register int current __asm__("$3") = murray->state;
            // From idle the stick needs a small dead zone before Murray starts moving.
            if (current == MURRAY_IDLE) {
                if (input->leftStickDeflection > 0.1f)
                    goto moving;
            } else if (input->leftStickDeflection > 0.0f)
                goto moving;
            goto idle;
        moving:
            if (flagsInput->buttonsHeld & BUTTON_RUN)

                SetMurrayState(murray, MURRAY_RUN);
            else
                SetMurrayState(murray, MURRAY_WALK);
            break;
        }
        case MURRAY_JUMP:
            UpdateJtAirInput(&murray->jtair);
            switch (murray->jtair.jumpKind) {
            case 0:
            case 11:
            case 15:
            case 16: break;
            default:
                if (murray->controller->GetButtonsPressedWithin(BUTTON_ALT_ATTACK | BUTTON_ATTACK, D_002C08DC) && murray->CanUseGadget(2) &&
                    murray->jtair.state < 6 && murray->jtair.state > 0 && !WillStepplayerLand(murray, D_002C08E0)) {
                    murray->controller->ClearJoyPressed(BUTTON_ALT_ATTACK | BUTTON_ATTACK);
                    murray->fFlopGadget = 0;
                    SetMurrayState(murray, MURRAY_FLOP);
                }
            }
            break;
        case MURRAY_GRAB: UpdateJtGrabInput(&murray->jtgrab); break;
        case MURRAY_HURT: UpdateJtHurtInput(&murray->jthurt); break;
        case MURRAY_FLOP: UpdateMurrayFlop(murray); break;
        case MURRAY_CARRY: UpdateMurrayCarry(murray); break;
        case MURRAY_STRUGGLE: UpdateMurrayStruggle(murray); break;
        case MURRAY_LIFT: UpdateMurrayLift(murray); break;
        case MURRAY_THROW:
        case MURRAY_TOSS: finished = IsAsegaRetracted(murray->activeAnimation); goto checkIdle;
        case MURRAY_ATTACK: UpdateMurrayAttack(murray); break;
        case MURRAY_STRAFE: UpdateMurrayStrafe(murray); break;
        case MURRAY_POWER_MOVE: UpdateMurrayPower(murray); break;
        case MURRAY_AIMED_THROW: UpdateJtThrow(&murray->jtthrowAim); break;
        case MURRAY_PIPE: UpdateJtActivePipe(&murray->jtpipe); break;
        case MURRAY_SCRIPT_ANIM:
            if (murray->activeAnimation && IsAsegaRetracted(murray->activeAnimation))
                goto idle;
            if (!murray->activeStateMachine)
                break;
            finished = IsSmaRetracted(murray->activeStateMachine);
        checkIdle:
            if (!finished)
                break;
        idle:
            SetMurrayState(murray, MURRAY_IDLE);
            break;
        case MURRAY_TURNBUCKLE:
            if (murray->controller->ConsumeButtonsPressedWithin(BUTTON_JUMP, 0.0f))
                SetJtAirJump(&murray->jtair, 13, 0, 0);
            else if (!(murray->controller->buttonsHeld & GetGadgetButtonMask(murray, 0x10)))
                SetMurrayState(murray, MURRAY_IDLE);
            break;
        default: break;
        }
    } while (murray->state != state);
}
} // namespace M_UpdateMurrayStateMachine
#undef FIELD_OFFSET

namespace M_func_001B5038 {
int IsStepplayerInJtMode(MURRAY *p) __asm__("IsStepplayerInJtMode");
int ConsumeButtonsPressedWithin__5POCTLUif(void *p, int n, float g) __asm__("ConsumeButtonsPressedWithin__5POCTLUif");
void EndJtMode(void *p) __asm__("EndJtMode");
void UpdateJtMode(void *p) __asm__("UpdateJtMode");
// While a look mode is active: in mode 1 any face button (mask 0x4F0) cancels it; then updates the mode.
// Also known as: UpdateMurrayBinoc (upstream readable / Sly 1 name; UpdateMurrayBinoc__FP6MURRAY).
extern "C" void UpdateMurrayJtModeInput(MURRAY *murray)
{
    if (IsStepplayerInJtMode(murray)) {
        if (murray->jtmode.state == 1 && !murray->jtmode.kind && ConsumeButtonsPressedWithin__5POCTLUif(murray->controller, BUTTON_FACE | BUTTON_LOOK, 0.25f)) EndJtMode(&murray->jtmode);
        UpdateJtMode(&murray->jtmode);
    }
}
} // namespace M_func_001B5038

namespace M_UpdateMurray {
extern float D_002C08E4 __asm__("D_002C08E4");
extern qword D_002C08F0 __asm__("D_002C08F0");
extern qword D_002A2DE0 __asm__("D_002A2DE0");
void UpdateStepplayer(MURRAY *p) __asm__("UpdateStepplayer");
int IsAsegaRetracted(ASEGA *p) __asm__("IsAsegaRetracted");
void DropAsegaRef(ASEGA **pp) __asm__("DropAsegaRef");
void GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi(SW *pg, MURRAY *p, int n) __asm__("GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi");
void UpdateMurrayJtModeInput(MURRAY *p) __asm__("UpdateMurrayJtModeInput");
void UpdateMurrayStateMachine(MURRAY *p) __asm__("UpdateMurrayStateMachine");
int GetStepplayerLeanFlags(MURRAY *p) __asm__("GetStepplayerLeanFlags");
void SetSmaMask(SMA *p, int n) __asm__("SetSmaMask");
void SetStepTargetHeading(MURRAY *p, float g) __asm__("SetStepTargetHeading");
qword GetStepAsegaTranslation(MURRAY *p, ASEGA *pl, float g) __asm__("GetStepAsegaTranslation");
void UpdateGaitBlend(GAIT *p, float g) __asm__("UpdateGaitBlend");
void UpdateGaitFootsteps(GAIT *p) __asm__("UpdateGaitFootsteps");
void UpdateJtActiveJump(JTAIR *p) __asm__("UpdateJtActiveJump");
void UpdateJtActiveHide(JTGRAB *p) __asm__("UpdateJtActiveHide");
void UpdateMurrayAttackVelocity(MURRAY *p) __asm__("UpdateMurrayAttackVelocity");
void UpdateMurrayPowerVelocity(MURRAY *p) __asm__("UpdateMurrayPowerVelocity");
void SetAsegaSpeed__FP5ASEGAf(ASEGA *p,float g) __asm__("SetAsegaSpeed__FP5ASEGAf");
void UpdateMurrayPlaces(MURRAY *p) __asm__("UpdateMurrayPlaces");
void UpdateJtActiveZap(JTHURT *p) __asm__("UpdateJtActiveZap");
int ShouldSwPairObjects(SW *pg,SOTREE *a,SOTREE *b) __asm__("ShouldSwPairObjects");
void RefreshSwObjectCollisionFilter(SW *pg, MURRAY *p) __asm__("RefreshSwObjectCollisionFilter");
void SetActPositionGoal(ACT1 *p, qword v) __asm__("SetActPositionGoal");
// Store -v (v scaled by a broadcast -1) into the actor's qwUnknown190.
extern "C" { static inline void Neg(MURRAY *p, qword v)
{
    union { qword q; float f[4]; } u;
    qword b = VuFromFloat<qword>(-1.0f);
    u.q = b;
    {
        qword qwpv190Out;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qwpv190Out) : "j"(v), "j"(u.q));
        p->qwUnknown190 = qwpv190Out;
    }
} }
// Store -v into the actor's qwUnknown190 and set the action's speed from the stick deflection.
extern "C" { static inline void Neg2(MURRAY *p, qword v)
{
    ASEGA *pl = p->activeAnimation;
    volatile qword u; /* scalar stack copy of the -1 broadcast (target stores it to 0(sp) last) */
    qword b = VuFromFloat<qword>(-1.0f); qword r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(b));
    p->qwUnknown190 = r;
    u = b;
    SetAsegaSpeed__FP5ASEGAf(pl,p->controller->leftStickDeflection);
} }
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char Size_6[sizeof(qword)==16?1:-1];
typedef char Size_7[sizeof(unsigned long)==8?1:-1];
typedef char Size_8[sizeof(float)==4?1:-1];
typedef char Size_9[sizeof(int)==4?1:-1];
typedef char Size_10[sizeof(void*)==4?1:-1];
// MURRAY Update (slot 16): STEPPLAYER update, grab candidates, the state machine, then per-state updates.
// Also known as: UpdateMurray (upstream readable / Sly 1 name; UpdateMurray__FP2LO).
extern "C" void UpdateMurray(MURRAY *murray)
{
    UpdateStepplayer(murray);
    if ((((int)murray->isInWorld & 1) == 0))
        return;
    if (murray->unknown190C && IsAsegaRetracted(murray->unknown190C))
        DropAsegaRef(&murray->unknown190C);
    GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi(&g_sw, murray, 0xF8FA);
    UpdateMurrayJtModeInput(murray);
    UpdateMurrayStateMachine(murray);
    switch (murray->state) {
    case MURRAY_IDLE:
        SetSmaMask(murray->activeStateMachine, GetStepplayerLeanFlags(murray));
        break;
    case MURRAY_RUN:
        SetStepTargetHeading(murray, murray->controller->leftStickAngle);
        UpdateGaitBlend(&murray->gaitA, 1.0f);
        UpdateGaitFootsteps(&murray->gaitA);
        break;
    case MURRAY_WALK:
        SetStepTargetHeading(murray, murray->controller->leftStickAngle);
        UpdateGaitBlend(&murray->gaitC, murray->controller->leftStickDeflection);
        UpdateGaitFootsteps(&murray->gaitC);
        break;
    case MURRAY_JUMP:
        UpdateJtActiveJump(&murray->jtair);
        break;
    case MURRAY_GRAB:
        UpdateJtActiveHide(&murray->jtgrab);
        break;
    case MURRAY_ATTACK:
        UpdateMurrayAttackVelocity(murray);
        break;
    case MURRAY_POWER_MOVE:
        UpdateMurrayPowerVelocity(murray);
        break;
    case MURRAY_SCRIPT_ANIM:
        Neg(murray, GetStepAsegaTranslation(murray, murray->activeAnimation, murray->activeAnimation->time));
        break;
    case MURRAY_SCRIPT_SMA:
        Neg2(murray, GetStepAsegaTranslation(murray, murray->activeAnimation, murray->activeAnimation->time));
        SetStepTargetHeading(murray, murray->controller->leftStickAngle);
        break;
    }
    UpdateMurrayPlaces(murray);
    UpdateJtActiveZap(&murray->jthurt);
    if (murray->ploReleased && murray->releaseTime + 0.1f < g_clock.time) {
        if (!((SO *)murray->ploReleased)->physicsTree || !ShouldSwPairObjects(&g_sw, murray->physicsTree, ((SO *)murray->ploReleased)->physicsTree))
            murray->ploReleased = 0;
        if (!murray->ploReleased)
            RefreshSwObjectCollisionFilter(&g_sw, murray);
    }
    if (!murray->pact1Unk1acc)
        return;
    if (g_clock.time - murray->unknown1AC8 < D_002C08E4)
        SetActPositionGoal(murray->pact1Unk1acc, D_002C08F0);
    else
        SetActPositionGoal(murray->pact1Unk1acc, D_002A2DE0);
}
} // namespace M_UpdateMurray
#undef FIELD_OFFSET

namespace M_UpdateMurrayCarry {
void DetachMurrayCarried(MURRAY *p) __asm__("DetachMurrayCarried");
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
int IsPowerupAcquired(int n) __asm__("IsPowerupAcquired");
void SetJtAirJump(JTAIR *p, int a, int b, int c) __asm__("SetJtAirJump");
void StartJtMode(JTMODE *p, int a, int b, int c) __asm__("StartJtMode");
void SetMurrayCarrySubstate(MURRAY *p, int n) __asm__("SetMurrayCarrySubstate");
void SetStepTargetHeading(MURRAY *p, float g) __asm__("SetStepTargetHeading");
qword GetStepAsegaTranslation(MURRAY *p,ASEGA *pasega,float g) __asm__("GetStepAsegaTranslation");
void UpdateGaitBlend(void *p, float g) __asm__("UpdateGaitBlend");
void UpdateGaitFootsteps(void *p) __asm__("UpdateGaitFootsteps");
// Store -value into *velocity, keeping the -1 broadcast in *scalar (register-pinned).
extern "C" { static inline void NegateSource(qword value,qword*velocity,QWROW*scalar){
    register qword input __asm__("vf3"),neg __asm__("vf1"),result __asm__("vf2");
    register int bits __asm__("$3");
    float negative=-1.0f;
    __asm__("qmtc2.ni %1,%0":"=j"(input):"r"(value));
    __asm__("mfc1 %0,%2\n\tqmtc2.ni %0,%1":"=&r"(bits),"=j"(neg):"f"(negative));
    __asm__("vmulx.xyzw %0,%1,%2x":"=j"(result):"j"(input),"j"(neg));
    scalar->q=neg;
    *velocity=result;
} }
// Copy the PL pointer through pinned registers.
extern "C" { static inline POCTL*CopyPL(POCTL*p){register POCTL*source __asm__("$2")=p;register POCTL*copy __asm__("$4");__asm__("daddu %0,%1,$zero":"=r"(copy):"r"(source));return copy;} }
// State 5 (carrying ploCarried): drops it if it is no longer active; jump with power-up 0x12, throw/toss buttons, carry
// locomotion. Power-up 0x12 is Atlas Strength.
// Also known as: UpdateMurrayCarryHeld (upstream readable / Sly 1 name; UpdateMurrayCarryHeld__FP6MURRAY).
extern "C" void UpdateMurrayCarry(MURRAY *murray)
{
    int s;
    for (;;) {
        s = murray->carrySubstate;
        switch (s) {
        case 0: case 1: case 2:
            if ((((int)((CID36 *)murray->ploCarried)->unknown180->isInWorld & 1) == 0)) {
                DetachMurrayCarried(murray);
                murray->ploCarried = 0;
                SetMurrayState(murray, MURRAY_IDLE);
            } else if (murray->controller->GetButtonsPressedWithin(BUTTON_JUMP, 0.1f) && IsPowerupAcquired(0x12)) {
                murray->controller->ClearJoyPressed(BUTTON_JUMP);
                SetJtAirJump(&murray->jtair, 0xF, 0, 0);
            } else if (murray->controller->ConsumeButtonsPressedWithin(BUTTON_ACTION | BUTTON_ATTACK, 0.0f)) {
                SetMurrayState(murray, MURRAY_THROW);
            } else if (murray->controller->ConsumeButtonsPressedWithin(BUTTON_ALT_ATTACK, 0.0f) && murray->pasegToss) {
                murray->fTossGadget = 0;
                SetMurrayState(murray, MURRAY_TOSS);
            } else if (murray->controller->GetButtonsPressedWithin(BUTTON_LOOK, 0.15f) && murray->CanUseGadget(3)) {
                murray->controller->ClearJoyPressed(BUTTON_LOOK);
                StartJtMode(&murray->jtmode, 0, 0, 0);
            } else {
                POCTL*source=murray->controller;
                POCTL*pl=CopyPL(source);
                float g=source->leftStickDeflection;
                if (murray->carrySubstate == 0 ? 0.1f < g : 0.0f < g) {
                    if ((pl->buttonsHeld & 8) && IsPowerupAcquired(0x12))
                        SetMurrayCarrySubstate(murray, 2);
                    else
                        SetMurrayCarrySubstate(murray, 1);
                } else {
                    SetMurrayCarrySubstate(murray, 0);
                }
            }
            break;
        }
        if (murray->carrySubstate == s || murray->state != MURRAY_CARRY)
            break;
    }
    switch (murray->carrySubstate) {
    case 0:
        break;
    case 1:
        SetStepTargetHeading(murray, murray->controller->leftStickAngle);
        UpdateGaitBlend(&murray->gaitB, murray->controller->leftStickDeflection);
        UpdateGaitFootsteps(&murray->gaitB);
        break;
    case 2: {
        QWROW u;
        qword v;
        SetStepTargetHeading(murray, murray->controller->leftStickAngle);
        v = GetStepAsegaTranslation(murray,murray->activeAnimation,murray->activeAnimation->time);
        NegateSource(v,&murray->qwUnknown190,&u);
        break;
    }
    }
}
} // namespace M_UpdateMurrayCarry

namespace M_SetMurrayCarrySubstate {
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void SetXfRole(MURRAY *p, XF *pxf, int k) __asm__("SetXfRole");
void ResetGaitFeet(void *p) __asm__("ResetGaitFeet");
void ReleaseAsega(ASEGA **ppasega) __asm__("ReleaseAsega");
void ReleaseSma(SMA **ppsma) __asm__("ReleaseSma");
void ReplaceCurrentAseg(ASEG *pr, MURRAY *p, float f, float g, int k, ASEGA **ppasega) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void ReplaceSma(void *pv, MURRAY *p, int a, int k, SMA **ppsma) __asm__("ReplaceSma");

// Carry sub-state: 0 stand, 1 walk (gaitB), 2 run (pasegCarryRun); restarts the animation.
// Also known as: SetMurrayCarryMove (upstream readable / Sly 1 name; SetMurrayCarryMove__FP6MURRAYi).
extern "C" void SetMurrayCarrySubstate(MURRAY *murray, int k)
{
    ASEG *pr;
    void *pv;
    int n;
    float f, now;

    if (k == murray->carrySubstate)
        return;
    switch (murray->carrySubstate) {
    case 0:
        SetXfRole(murray, murray->positionXf, 2);
        SetXfRole(murray, murray->rotationXf, 2);
        murray->carrySubstate = k;
        break;
    case 1:
    case 2: murray->gaitBlend = murray->activeAnimation->time / murray->activeAnimation->animation->duration;
    default: murray->carrySubstate = k; break;
    }
    __asm__("daddu %0,$%5,$zero\n\tdaddu %1,$%5,$zero\n\tlwc1 %2,%6\n\taddiu %3,$zero,%7\n\tmtc1 $%5,%4"
            : "=&r"(pr), "=&r"(pv), "=f"(now), "=r"(n), "=f"(f)
            : "J"(0), "m"(g_clock.time), "I"(1));
    murray->carrySubstateTime = now;
    if (k == n)
        goto state1;
    if (k < 2) {
        if (k == 0)
            goto state0;
        goto done;
    }
    if (k == 2)
        goto state2;
    goto done;
state0:
    SetXfRole(murray, murray->positionXf, 1);
    SetXfRole(murray, murray->rotationXf, 1);
    pv = murray->psmCarryStand;
    murray->qwUnknown190 = D_002A2DD0;
    goto done;
state1:
    pr = murray->gaitB.blendedAnimation;
    f = murray->gaitBlend * pr->duration;
    ResetGaitFeet(&murray->gaitA);
    n = 9;
    goto done;
state2:
    pr = murray->pasegCarryRun;
    n = 9;
    f = murray->gaitBlend * pr->duration;
done:
    ReleaseAsega(&murray->activeAnimation);
    ReleaseSma(&murray->activeStateMachine);
    ReplaceCurrentAseg(pr, murray, f, 1.0f, n, &murray->activeAnimation);
    ReplaceSma(pv, murray, -1, n, &murray->activeStateMachine);
}
} // namespace M_SetMurrayCarrySubstate
#undef FIELD_OFFSET

namespace M_func_001B5820 {
extern int D_002C0908[][5] __asm__("D_002C0908"), D_002C0948[][5] __asm__("D_002C0948");
extern float D_002C0900 __asm__("D_002C0900");
STEPPLAYER *GetHeroPo(void *g) __asm__("GetHeroPo");
int IsSwHandsOff(void *g) __asm__("IsSwHandsOff");
int GetSmaCurrentState(SMA *psma) __asm__("GetSmaCurrentState");
int GetSmaGoal(SMA *psma) __asm__("GetSmaGoal");
int ConsumeButtonsPressedWithin__5POCTLUif(POCTL *pctl, int b, float f) __asm__("ConsumeButtonsPressedWithin__5POCTLUif");
void SetSmaGoal(SMA *psma, int b) __asm__("SetSmaGoal");
// State 6 (scripted struggle with a partner state machine): only while Murray is the current player and not hands-off;
// face-button mashing advances it.
// Also known as: UpdateMurrayHold (upstream readable / Sly 1 name; UpdateMurrayHold__FP6MURRAY).
extern "C" void UpdateMurrayStruggle(MURRAY *murray)
{
    int a;
    if (GetHeroPo(&g_sw) != murray) return;
    if (IsSwHandsOff(&g_sw)) return;
    a = GetSmaCurrentState(murray->activeStateMachine);
    if (a != GetSmaGoal(murray->activeStateMachine)) return;
    if (a >= 0x1D0) return;
    if (a < 0x1CB) return;
    murray->struggleTimeLeft -= g_clock.deltaTime;
    if (ConsumeButtonsPressedWithin__5POCTLUif(murray->controller, BUTTON_FACE, 0.0f)) {
        murray->mashesLeft--;
        murray->lastMashTime = murray->struggleTimeLeft;
    }
    if (murray->struggleTimeLeft <= 0.0f) {
        if (murray->mashesLeft <= 0) a = D_002C0908[murray->workAction][a - 0x1CB];
        else a = D_002C0948[murray->workAction][a - 0x1CB];
    } else if (murray->lastMashTime - murray->struggleTimeLeft > D_002C0900) {
        a = 0x1CA;
    }
    if (murray->psmaWork) SetSmaGoal(murray->psmaWork, a);
    SetSmaGoal(murray->activeStateMachine, a);
}
} // namespace M_func_001B5820

namespace M_func_001B5990 {
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;

extern float D_002C08DC __asm__("D_002C08DC");
int IsStepplayerNearInteractTarget(MURRAY*,int) __asm__("IsStepplayerNearInteractTarget");
void SetMurrayState(MURRAY*,int) __asm__("SetMurrayState");
void SetJtAirJump(JTAIR*,int,int,void*) __asm__("SetJtAirJump");
void StartJtMode(JTMODE*,int,MURRAY*,void*) __asm__("StartJtMode");
int TryMurrayTakedown(MURRAY*) __asm__("TryMurrayTakedown");
int TryGrabJtNearestCandidate(JTGRAB*,int,int) __asm__("TryGrabJtNearestCandidate");
int CanMurrayAttack(MURRAY*) __asm__("CanMurrayAttack");
void StartMurrayPunch(MURRAY*) __asm__("StartMurrayPunch");
void StartMurrayAltAttack(MURRAY*) __asm__("StartMurrayAltAttack");
void SetMurrayStrafeSubstate(MURRAY*,int) __asm__("SetMurrayStrafeSubstate");
void FaceStepToward(MURRAY*,void*) __asm__("FaceStepToward");
void SetStepTargetHeading(MURRAY*,float) __asm__("SetStepTargetHeading");
void UpdateGaitBlend(GAIT*,float) __asm__("UpdateGaitBlend");
void UpdateGaitFootsteps(GAIT*) __asm__("UpdateGaitFootsteps");
qword SetVectorCylind(float,float,float) __asm__("SetVectorCylind");
void UpdateDirectionalBlend(MURRAY*,BLDIR*,ASEGBL*) __asm__("UpdateDirectionalBlend");

// Load a vector into a VU0 register.
static inline qword Load(const QWROW*p){qword q;__asm__("lqc2 %0,%1":"=j"(q):"m"(*p));return q;}
// XY length of v as a float.
static inline float LengthXY(qword v){qword q;float f;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(q):"j"(v));__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
// Move a 128-bit GPR value into a VU0 register.
static inline qword Vector(qword q){qword v;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(q));return v;}
// Rotate vector by the 3x3 matrix m.
static inline qword Rotate(MATRIX3Q *m, qword vector)
{
    qword result = Load(&m->r[0]), row1 = m->r[1].q, row2 = m->r[2].q;
    qword a, b, c, one;
    __asm__("vmul.xyzw %2,%0,%7\n\tvmaxw.xyzw %1,$vf0,$vf0w\n\tvmul.xyzw %3,%5,%7\n\tvmul.xyzw %4,%6,%7\n\tvadday.x "
            "$ACC,%2,%2y\n\tvmaddz.x %0,%1,%2z\n\tvaddax.y $ACC,%3,%3x\n\tvmaddz.y %0,%1,%3z\n\tvaddax.z "
            "$ACC,%4,%4x\n\tvmaddy.z %0,%1,%4y"
            : "+j"(result), "=&j"(one), "=&j"(a), "=&j"(b), "=&j"(c)
            : "j"(row1), "j"(row2), "j"(vector));
    return result;
}
// State 9 (locked onto the nearby target interactTarget): exits when the lock button is released or the target is lost; allows
// jump and takedown.
// Also known as: UpdateMurrayBehavior (upstream readable / Sly 1 name; UpdateMurrayBehavior__FP6MURRAY).
extern "C" void UpdateMurrayStrafe(MURRAY *murray)
{
    int before;
    for (;;) {
        before = murray->strafeSubstate;
        switch (before) {
        case 0:
        case 1:
        case 2: {
            if ((murray->controller->buttonsHeld & BUTTON_RUN) || !IsStepplayerNearInteractTarget(murray, 1))
                SetMurrayState(murray, MURRAY_IDLE);
            else if (murray->controller->ConsumeButtonsPressedWithin(BUTTON_JUMP, 0.1f))
                SetJtAirJump(&murray->jtair, 1, 0, 0);
            else if (TryMurrayTakedown(murray)) {
            } else if (murray->controller->ConsumeButtonsPressedWithin(BUTTON_ACTION, 0.2f)) {
                if (!TryGrabJtNearestCandidate(&murray->jtgrab, 0x80, 0) && murray->psoCarryRoot) {
                    murray->liftClip = 0;
                    SetMurrayState(murray, MURRAY_LIFT);
                }
            } else if (CanMurrayAttack(murray) && murray->controller->ConsumeButtonsPressedWithin(BUTTON_ATTACK, D_002C08DC))
                StartMurrayPunch(murray);
            else if (murray->controller->GetButtonsPressedWithin(BUTTON_LOOK, 0.15f) &&
                     murray->CanUseGadget(3)) {
                murray->controller->ClearJoyPressed(BUTTON_LOOK);
                StartJtMode(&murray->jtmode, 0, 0, 0);
            } else if (CanMurrayAttack(murray) && murray->controller->ConsumeButtonsPressedWithin(BUTTON_ALT_ATTACK, D_002C08DC))
                StartMurrayAltAttack(murray);
            else {
                POCTL *pl = murray->controller;
                float strength = pl->leftStickDeflection;
                if (murray->state == MURRAY_IDLE ? 0.1f < strength : 0.0f < strength)
                    SetMurrayStrafeSubstate(murray, 2);
                else
                    SetMurrayStrafeSubstate(murray, 0);
            }
        } break;
        }
        if (murray->strafeSubstate == before || murray->state != MURRAY_STRAFE)
            break;
    }
    switch (murray->strafeSubstate) {
    case 0: FaceStepToward(murray, murray->interactTarget); break;
    case 2: {
        FaceStepToward(murray, murray->interactTarget);
        float length = LengthXY(murray->unknown16F0[1].vRoot);
        POCTL *input = murray->controller;
        qword direction = Vector(
            SetVectorCylind(input->leftStickAngle + 3.1415927f, (input->leftStickDeflection * length) * murray->activeAnimation->speed, 0.0f));
        murray->qwUnknown190 = direction;
        qword world = Rotate(&murray->matqTarget, direction);
        murray->blendSide = -1;
        murray->qwUnknown190 = world;
        UpdateDirectionalBlend(murray, murray->unknown16F0, murray->unknown17F0);
        break;
    }
    case 1:
        SetStepTargetHeading(murray, murray->controller->leftStickAngle);
        UpdateGaitBlend(&murray->gaitD, murray->controller->leftStickDeflection);
        UpdateGaitFootsteps(&murray->gaitD);
        break;
    }
}
} // namespace M_func_001B5990
#undef FIELD_OFFSET

namespace M_func_001B5D30 {
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void SetXfRole(MURRAY *p, XF *pxf, int b) __asm__("SetXfRole");
void ResetGaitFeet(void *p) __asm__("ResetGaitFeet");
void ReleaseAsega(void *p) __asm__("ReleaseAsega");
void ReleaseSma(void *p) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *a, MURRAY *p, int n, void *q, float f1, float f2) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void ReplaceSma(SM *psm, MURRAY *p, int b, int n, void *q) __asm__("ReplaceSma");
// Strafe sub-state (x17F4): 0 stand, 1 blend x1630, 2 clip x17F0; restarts the animation.
// Also known as: SetMurrayBehaviorAnim (upstream readable / Sly 1 name; SetMurrayBehaviorAnim__FP6MURRAYi).
extern "C" void SetMurrayStrafeSubstate(MURRAY *murray, int st)
{
    void *o;
    SM *x;
    int n;
    if (st == murray->strafeSubstate) return;
    if (murray->strafeSubstate == 0) {
        SetXfRole(murray, murray->positionXf, 2);
        SetXfRole(murray, murray->rotationXf, 2);
    }
    murray->strafeSubstate = st;
    murray->strafeStartTime = g_clock.time;
    o = 0;
    x = 0;
    n = 1;
    switch (st) {
    case 0:
        SetXfRole(murray, murray->positionXf, 1);
        SetXfRole(murray, murray->rotationXf, 1);
        x = murray->psmStrafeIdle;
        murray->qwUnknown190 = D_002A2DD0;
        break;
    case 2:
        o = murray->unknown17F0;
        n = 9;
        break;
    case 1:
        o = murray->gaitD.blendedAnimation;
        ResetGaitFeet(&murray->gaitD);
        n = 9;
        break;
    }
    ReleaseAsega(&murray->activeAnimation);
    ReleaseSma(&murray->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(o, murray, n, &murray->activeAnimation, 0.0f, 1.0f);
    ReplaceSma(x, murray, -1, n, &murray->activeStateMachine);
}
} // namespace M_func_001B5D30

namespace M_ChooseMurrayThrowTarget {
struct O { char pad[0xE0]; struct O *pnext; int x4; int xE8; };
struct G { char pad[0x13B4]; struct O *pFirst; };
extern float D_002C0984 __asm__("D_002C0984"), D_002C0988 __asm__("D_002C0988");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
int ShouldTriggerGrabspotSkipTarget(void *a, struct O *po) __asm__("ShouldTriggerGrabspotSkipTarget");
float atan2f(float y, float x) __asm__("atan2f");
float GetStepHeading(MURRAY *p) __asm__("GetStepHeading");
float NormalizeAngle__Ff(float g) __asm__("NormalizeAngle__Ff");
qword SetVectorCylind(float a, float b, float c) __asm__("SetVectorCylind");
// z lane of t as a float.
extern "C" { static __inline__ float GetZ2(qword t) { __asm__("pextuw %0,$0,%0" : "+r"(t)); return QwordX(t); } }
// y lane of t as a float.
extern "C" { static __inline__ float GetY2(qword t) { __asm__("prot3w %0,%0" : "+r"(t)); return QwordX(t); } }
// Move a VU0 register to a 128-bit GPR value.
extern "C" { static inline qword ToGpr(qword v) { qword r; __asm__ __volatile__("qmfc2.ni %0, %1" : "=r"(r) : "j"(v)); return r; } }
// XY length of v as a float.
extern "C" { static inline float LenXY(qword v) {
    qword d;
    float f;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }
// Picks the throw goal: the nearest object with flag 0x10000 (not the carried one) within range and view cone.
// Without one, the goal is 1500 units ahead.
// Also known as: ChooseMurrayThrowTarget (upstream readable / Sly 1 name; ChooseMurrayThrowTarget__FP6MURRAY).
extern "C" void ChooseMurrayThrowTarget(MURRAY *murray)
{
    struct { int a[4]; } unused;
    struct O *object, *poBest = 0;
    float maxValue = murray->throwRange * D_002C0984;
    for (object = (*(struct G *)&g_sw.aslPxrOid[410]).pFirst; object; object = object->pnext) {
        qword d;
        float dist, g;
        if (!(object->xE8 & 0x10000))
            continue;
        if (ShouldTriggerGrabspotSkipTarget(murray->ploCarried, object))
            continue;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(GetLoWorldPos(object)), "j"(GetLoWorldPos(murray)));
        dist = LenXY(d);
        if (maxValue < dist)
            continue;
        if (dist < 150.0f)
            continue;
        if (500.0f < GetZ2(ToGpr(d)))
            continue;
        { qword t = ToGpr(d); float x = QwordX(t); __asm__ __volatile__("prot3w %0,%0" : "+r"(t)); g = atan2f(QwordX(t), x); }
        if (D_002C0988 < __builtin_fabsf(NormalizeAngle__Ff(g - GetStepHeading(murray))))
            continue;
        poBest = object;
        maxValue = dist;
        murray->throwGoal = GetLoWorldPos(poBest);
    }
    if (poBest)
        return;
    qword a, b, r;
    a = GetLoWorldPos(murray);
    b = SetVectorCylind(murray->targetHeading, murray->throwRange, 0.0f);
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    murray->throwGoal = r;
}
} // namespace M_ChooseMurrayThrowTarget

namespace M_SetMurrayState {
// SetMurrayState(murray, next): Murray's main state transition (MURRAY::state).
// Exit work for the old state, then entry work for `next` (clip/state-machine selection), then the shared
// tail that restarts the active animation (activeAnimation) and state machine (activeStateMachine).
// Lane 4 (Claude agent): hacks of the earlier drafts (xori/mtc1/addiu asm, 6 pins) are gone; this has ONE pin
// (the zero-vector temp in case 7) and one VU vsub.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };

extern U D_002A2DD0 __asm__("D_002A2DD0");
extern BENTLEY *D_002DD5BC __asm__("D_002DD5BC");
void SetXfRole(MURRAY *, XF *, int) __asm__("SetXfRole");
void ScheduleIdleTimer(MURRAY *) __asm__("ScheduleIdleTimer");
void SetMurrayFlopSubstate(MURRAY *, int) __asm__("SetMurrayFlopSubstate");
void ExplodeWithDefaults(EXPL *) __asm__("ExplodeWithDefaults");
void SetJtAirState(void *, int, int) __asm__("SetJtAirState");
void SetJtGrabKind(void *, int) __asm__("SetJtGrabKind");
void SetJtHurtState(void *, int) __asm__("SetJtHurtState");
void SetMurrayAttack(MURRAY *, int) __asm__("SetMurrayAttack");
void SetMurrayPowerSubstate(MURRAY *, int) __asm__("SetMurrayPowerSubstate");
void SetMurrayLiftSubstate(MURRAY *, int) __asm__("SetMurrayLiftSubstate");
void SetMurrayCarrySubstate(MURRAY *, int) __asm__("SetMurrayCarrySubstate");
void SetMurrayStrafeSubstate(MURRAY *, int) __asm__("SetMurrayStrafeSubstate");
void SetJtThrowState(void *, int) __asm__("SetJtThrowState");
void SetSoCnstrForce(MURRAY *, int) __asm__("SetSoCnstrForce");
void SetJtPipeState(void *, int) __asm__("SetJtPipeState");
int FindSmStateOptional__FP2SM3OID(SM *, int) __asm__("FindSmStateOptional__FP2SM3OID");
int GetStepplayerLeanFlags(MURRAY *) __asm__("GetStepplayerLeanFlags");
void InitIdleTimer(MURRAY *, int) __asm__("InitIdleTimer");
void ResetGaitFeet(void *) __asm__("ResetGaitFeet");
void SetJtModeState__FP6JTMODEi(void *, int) __asm__("SetJtModeState__FP6JTMODEi");
int IsPowerupAcquired(int) __asm__("IsPowerupAcquired");
void ChooseMurrayThrowTarget(MURRAY *) __asm__("ChooseMurrayThrowTarget");
qword GetLoWorldPos(MURRAY *) __asm__("GetLoWorldPos");
void SetStepTargetHeadingFromVector(MURRAY *, qword) __asm__("SetStepTargetHeadingFromVector");
void ReleaseAsega(ASEGA **) __asm__("ReleaseAsega");
void ReleaseSma(SMA **) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *, MURRAY *, float, float, int, ASEGA **) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void ReplaceSmaEx(SM *, MURRAY *, int, int, int, SMA **) __asm__("ReplaceSmaEx");
EA *FindAsegLabel(ASEG *, int) __asm__("FindAsegLabel");
void SetAsegaChannelWeight(ASEGA *, float) __asm__("SetAsegaChannelWeight");

// Murray state switch (MURRAY::state, 0..21): exit work for the old state, entry work (clip selection, velocity reset,
// component states) for the new one.
void SetMurrayState(MURRAY *murray, int next) __asm__("SetMurrayState"); void SetMurrayState(MURRAY *murray, int next)
{
    if (next == murray->state)
        return;
    // leave the old state
    switch (murray->state) {
    case MURRAY_IDLE:
        SetXfRole(murray, murray->positionXf, 2);
        SetXfRole(murray, murray->rotationXf, 2);
        ScheduleIdleTimer(murray);
        break;
    case MURRAY_PLAYER_STATE_4:
    case MURRAY_SCRIPT_ANIM:
    case MURRAY_SCRIPT_SMA:
        SetXfRole(murray, murray->positionXf, 2);
        SetXfRole(murray, murray->rotationXf, 2);
        break;
    case MURRAY_FLOP: SetMurrayFlopSubstate(murray, -1); break;
    case MURRAY_JUMP: {
        if (!murray->pwater) {
            EXPL *object = 0;
            switch (next) {
            case MURRAY_IDLE:
            case MURRAY_RUN:
            case MURRAY_LIFT:
            case MURRAY_CARRY:
            case MURRAY_STRAFE:
            case MURRAY_ATTACK:
            case MURRAY_POWER_MOVE: object = murray->pexplDustLand; break;
            case MURRAY_JUMP:
            case MURRAY_FLOP:
            case MURRAY_STRUGGLE:
            case MURRAY_THROW:
            case MURRAY_TOSS:
            case MURRAY_WALK: break;
            case MURRAY_GRAB: {
                switch (murray->jtgrab.state) {
                case 1: break;
                case 3:
                case 4: object = murray->pexplDustGrab; break;
                default: object = murray->pexplDustLand; break;
                }
                break;
            }
            }
            if (object) {
                object->Add();
                ExplodeWithDefaults(object);
                object->Remove();
            }
        }
        SetJtAirState(&murray->jtair, -1, 0);
        break;
    }
    case MURRAY_GRAB: SetJtGrabKind(&murray->jtgrab, -1); break;
    case MURRAY_HURT: SetJtHurtState(&murray->jthurt, -1); break;
    case MURRAY_ATTACK: SetMurrayAttack(murray, -1); break;
    case MURRAY_POWER_MOVE: SetMurrayPowerSubstate(murray, -1); break;
    case MURRAY_LIFT: SetMurrayLiftSubstate(murray, -1); break;
    case MURRAY_CARRY: SetMurrayCarrySubstate(murray, -1); break;
    case MURRAY_STRAFE: SetMurrayStrafeSubstate(murray, -1); break;
    case MURRAY_AIMED_THROW:
        murray->jtthrowAim.ploHoldParent->Remove();
        SetJtThrowState(&murray->jtthrowAim, -1);
        SetSoCnstrForce(murray, 0);
        break;
    case MURRAY_RUN:
    case MURRAY_WALK: murray->gaitBlend = murray->activeAnimation->time / murray->activeAnimation->animation->duration; break;
    case MURRAY_STRUGGLE: murray->psmaWork = 0; break;
    case MURRAY_PIPE: SetJtPipeState(&murray->jtpipe, -1); break;
    case MURRAY_THROW:
    case MURRAY_TOSS:
    case MURRAY_SCRIPT_THROW: break;
    }
    int old = murray->state;
    murray->state = next;
    ASEG *clip = 0;
    SM *source = 0;
    int flags = 1, id = -1, mode = 4;
    void *activation;
    float duration = 0.0f;
    murray->stateTime = g_clock.time;
    // enter the new state
    switch (next) {
    case MURRAY_IDLE:
        SetXfRole(murray, murray->positionXf, 1);
        SetXfRole(murray, murray->rotationXf, 1);
        source = murray->unknown1028;
        murray->qwUnknown190 = D_002A2DD0.q;
        if (old != 2) {
            id = (old != 10) ? 0x175 : 0x176;
        } else
            id = 0x178;
        if (FindSmStateOptional__FP2SM3OID(source, id) < 0)
            id = -1;
        mode = GetStepplayerLeanFlags(murray);
        InitIdleTimer(murray, 1);
        break;
    case MURRAY_RUN:
        clip = murray->gaitA.blendedAnimation;
        activation = &murray->gaitA;
        goto activate;
    case MURRAY_HURT: SetJtModeState__FP6JTMODEi(&murray->jtmode, -1); break;
    case MURRAY_FLOP: {
        SetMurrayFlopSubstate(murray, 0);
        ASEG *a = murray->pasegFlopGadget;
        if (a && murray->fFlopGadget)
            clip = a;
        else
            clip = murray->pasegFlop;
        break;
    }
    case MURRAY_CARRY:
        SetMurrayCarrySubstate(murray, 0);
        murray->ApplyStateStepParams();
        return;
    case MURRAY_STRUGGLE: source = murray->psmStruggle; break;
    case MURRAY_LIFT:
        switch (murray->liftClip) {
        case 0: clip = murray->apasegLift[0]; break;
        case 1: clip = murray->apasegLift[1]; break;
        }
        murray->qwUnknown190 = D_002A2DD0.q;
        SetMurrayLiftSubstate(murray, 0);
        break;
    case MURRAY_THROW: {
        ASEG *a = murray->pasegThrowPowered;
        if (a && IsPowerupAcquired(0x11))
            clip = murray->pasegThrowPowered;
        else
            clip = murray->pasegThrow;
        {
            register qword zero __asm__("$3") = D_002A2DD0.q;
            murray->qwUnknown190 = zero;
        } // pin: the zero vector goes through $v1 here
        ChooseMurrayThrowTarget(murray);
        U point;
        point.q = murray->throwGoal;
        qword current = GetLoWorldPos(murray);
        qword difference;
        __asm__("vsub.xyzw %0,%0,%2" : "=j"(difference) : "0"(point.q), "j"(current));
        SetStepTargetHeadingFromVector(murray, difference);
        break;
    }
    case MURRAY_TOSS: {
        ASEG *a = murray->pasegTossGadget;
        if (a && murray->fTossGadget)
            clip = a;
        else
            clip = murray->pasegToss;
        goto clear_velocity;
    }
    case MURRAY_STRAFE:
        SetMurrayStrafeSubstate(murray, 0);
        murray->ApplyStateStepParams();
        return;
    case MURRAY_WALK:
        clip = murray->gaitC.blendedAnimation;
        activation = &murray->gaitC;
    activate:
        flags = 9;
        ResetGaitFeet(activation);
        break;
    case MURRAY_AIMED_THROW:
        murray->jtthrowAim.ploHoldParent->Add();
        murray->jtthrowAim.ploCarried = D_002DD5BC;
        SetJtThrowState(&murray->jtthrowAim, 0);
        SetSoCnstrForce(murray, 1);
        murray->SetVelocity(D_002A2DD0.q);
        return;
    case MURRAY_PLAYER_STATE_4: source = murray->unknown102C;
    case MURRAY_SCRIPT_ANIM:
    case MURRAY_SCRIPT_SMA:
        SetXfRole(murray, murray->positionXf, 1);
        SetXfRole(murray, murray->rotationXf, 1);
    clear_velocity:
        murray->qwUnknown190 = D_002A2DD0.q;
        break;
    case MURRAY_PIPE: SetJtPipeState(&murray->jtpipe, 0); return;
    case MURRAY_TURNBUCKLE: {
        clip = murray->pasegTurnbuckle;
        U *velocity = (U *)&murray->qwUnknown190;
        velocity->i[2] = 0;
        flags = 9;
        velocity->i[1] = 0;
        ((U *)&murray->qwUnknown190)->i[0] = 0;
        break;
    }
    case MURRAY_JUMP:
    case MURRAY_ATTACK:
    case MURRAY_POWER_MOVE:
    case MURRAY_GRAB:
    case MURRAY_SCRIPT_THROW: break;
    }
    {
        ASEGA **animation = &murray->activeAnimation;
        SMA **component = &murray->activeStateMachine;
        float position = 0.0f;
        ReleaseAsega(animation);
        ReleaseSma(component);
        ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(clip, murray, position, 1.0f, flags, animation);
        ReplaceSmaEx(source, murray, id, flags, mode, component);
        if (clip || source) {
            if (clip && duration == position) {
                EA *e = FindAsegLabel(clip, 0x1AC);
                if (e)
                    duration = e->time;
            }
            if (duration != 0.0f)
                SetAsegaChannelWeight(murray->activeAnimation, g_clock.time + duration / murray->activeAnimation->speed);
        }
    }
    murray->ApplyStateStepParams();
}
} // namespace M_SetMurrayState

namespace M_GetMurrayCpdefi {
void GetStepplayerCpdefi(MURRAY *p, int *pf) __asm__("GetStepplayerCpdefi");
void UpdateJtThrowHoldPosition(void *p, int *pf) __asm__("UpdateJtThrowHoldPosition");
void GetJtPipeVelocity(void *p, int *pf) __asm__("GetJtPipeVelocity");
// MURRAY camera definition (slot 50): stamps x1080 while moving, STEPPLAYER base, then flag bit 1 for flop/strafe.
// Also known as: GetMurrayCpdefi (upstream readable / Sly 1 name; GetMurrayCpdefi__FP6MURRAYP9ACTORPOSE).
extern "C" void GetMurrayCpdefi(MURRAY *murray, int *pf)
{
    if (murray->state != MURRAY_IDLE && murray->state != MURRAY_PLAYER_STATE_4 && murray->jtgrab.kind) murray->unknown1080 = g_clock.time;
    GetStepplayerCpdefi(murray, pf);
    switch (murray->state) {
    case MURRAY_ATTACK:
        if (murray->nUnk10e4 != 4) break;
    case MURRAY_FLOP: case MURRAY_STRAFE: case MURRAY_POWER_MOVE:
        *pf |= 1;
        break;
    case MURRAY_AIMED_THROW: UpdateJtThrowHoldPosition(&murray->jtthrowAim, pf); break;
    case MURRAY_PIPE: GetJtPipeVelocity(&murray->jtpipe, pf); break;
    }
}
} // namespace M_GetMurrayCpdefi

namespace M_PresetMurrayAccel {
void PresetSoAccel(MURRAY *p) __asm__("PresetSoAccel");
void PresetStepAccel(MURRAY *p) __asm__("PresetStepAccel");
void PresetJtAccelJump(void *p) __asm__("PresetJtAccelJump");
void PresetJtAccelHide(void *p) __asm__("PresetJtAccelHide");
void PresetJtAccelZap(void *p) __asm__("PresetJtAccelZap");
void PresetMurrayFlopAccel(MURRAY *p) __asm__("PresetMurrayFlopAccel");
void PresetJtAccelPipe(void *p) __asm__("PresetJtAccelPipe");
// MURRAY physics preset (slot 70): the base preset, then the per-state component presets.
// Also known as: UpdateMurrayMotion (upstream readable / Sly 1 name; UpdateMurrayMotion__FP3ALO).
extern "C" void PresetMurrayAccel(MURRAY *murray)
{
    if (murray->state == MURRAY_PIPE) PresetSoAccel(murray);
    else PresetStepAccel(murray);
    switch (murray->state) {
    case MURRAY_JUMP: PresetJtAccelJump(&murray->jtair); break;
    case MURRAY_GRAB: PresetJtAccelHide(&murray->jtgrab); break;
    case MURRAY_HURT: PresetJtAccelZap(&murray->jthurt); break;
    case MURRAY_FLOP: PresetMurrayFlopAccel(murray); break;
    case MURRAY_PIPE: PresetJtAccelPipe(&murray->jtpipe); break;
    }
}
} // namespace M_PresetMurrayAccel

namespace M_HandleMurrayMessage {
typedef struct MSG { void *x0; int x4; int x8; } MSG;
extern float D_002C0990[] __asm__("D_002C0990");
extern float D_002C099C __asm__("D_002C099C");
void HandleStepplayerMessage(MURRAY *p, int msg, MSG *pm) __asm__("HandleStepplayerMessage");
void SetMurrayFlopSubstate(MURRAY *p, int k) __asm__("SetMurrayFlopSubstate");
qword GetLoWorldPos(MURRAY *p) __asm__("GetLoWorldPos");
void EmitSwNoise(void *g, int z, qword q, MURRAY *p) __asm__("EmitSwNoise");
void ThrowMurrayCarried(MURRAY *p) __asm__("ThrowMurrayCarried");
void TossMurrayCarried(MURRAY *p) __asm__("TossMurrayCarried");
void ReleaseJtThrowObject(void *p) __asm__("ReleaseJtThrowObject");
void SetMurrayLiftSubstate(MURRAY *p, int k) __asm__("SetMurrayLiftSubstate");
void SetMurrayState(MURRAY *p, int s) __asm__("SetMurrayState");

// MURRAY message handler (slot 5): base, then animation events from his own animations (0x1A4 flop dive, ...).
// Also known as: HandleMurrayMessage (upstream readable / Sly 1 name; HandleMurrayMessage__FP6MURRAY5MSGIDPv).
extern "C" void HandleMurrayMessage(MURRAY *murray, int msg, MSG *pm)
{
    HandleStepplayerMessage(murray, msg, pm);
    switch (msg) {
    case 9:
        if (pm->x0 != murray->activeAnimation && pm->x0 != murray->unknown190C) return;
        switch (pm->x8) {
        case 0x1A4:
            SetMurrayFlopSubstate(murray, 1);
            break;
        case 0x1A5:
            EmitSwNoise(&g_sw, 0, GetLoWorldPos(murray), murray);
            break;
        case 0x1EB:
            switch (murray->state) {
            case MURRAY_THROW:
                ThrowMurrayCarried(murray);
                break;
            case MURRAY_TOSS:
                TossMurrayCarried(murray);
                break;
            case MURRAY_AIMED_THROW:
                ReleaseJtThrowObject(&murray->jtthrowAim);
                break;
            }
            break;
        case 0x1C6:
            murray->fLiftGrabbed = 1;
            break;
        case 0x1C8:
            if (!murray->liftSubstate) {
                EmitSwNoise(&g_sw, 0, GetLoWorldPos(murray), murray);
                SetMurrayLiftSubstate(murray, 1);
            }
            break;
        case 0x1C7:
            if (murray->liftSubstate == 1) SetMurrayLiftSubstate(murray, 2);
            break;
        case 0x240:
            murray->unknown1818 = 1;
            break;
        case 0x241:
            murray->unknown181C = 1;
            break;
        case 0x242:
            murray->comboWindow = 1;
            break;
        }
        break;
    case 10:
        if (pm->x0 != murray->activeStateMachine) return;
        if (murray->state != MURRAY_STRUGGLE) return;
        if (pm->x8 == pm->x4) return;
        switch (pm->x8) {
        case 0x1CA:
        case 0x1D0:
            SetMurrayState(murray, MURRAY_IDLE);
            break;
        case 0x1CB:
        case 0x1CC:
        case 0x1CD:
        case 0x1CE:
        case 0x1CF: {
            float f = D_002C0990[murray->workAction];
            murray->struggleTimeLeft = murray->lastMashTime = f;
            murray->mashesLeft = (int)(D_002C099C * f);
            break;
        }
        }
        break;
    }
}
} // namespace M_HandleMurrayMessage

namespace M_func_001B6BA8 {
typedef int qword __attribute__((mode(TI)));
extern float D_002C09A0 __asm__("D_002C09A0"), D_002BF730 __asm__("D_002BF730");
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
void SetMurrayFlopSubstate(MURRAY *p, int n) __asm__("SetMurrayFlopSubstate");
int IsJtStuck(MURRAY *p, void *q, float f) __asm__("IsJtStuck");
void ReturnJtToSafePlace(MURRAY *p, int n) __asm__("ReturnJtToSafePlace");
qword SetVectorCylind(float a, float b, float c) __asm__("SetVectorCylind");
// State 3 (air slam): ends when the animation is done; sub-state 1 waits for landing then the impact sub-state 2;
// steering.
// Also known as: UpdateMurrayFlop (upstream readable / Sly 1 name; UpdateMurrayFlop__FP6MURRAY).
extern "C" void UpdateMurrayFlop(MURRAY *murray)
{
    int s;
    for (;;) {
        s = murray->flopSubstate;
        if (!murray->activeAnimation || IsAsegaRetracted(murray->activeAnimation)) SetMurrayState(murray, MURRAY_IDLE);
        else if (murray->flopSubstate == 1) {
            if (murray->pxpFlop) SetMurrayFlopSubstate(murray, 2);
            else if (IsJtStuck(murray, &murray->unknown131C, murray->unknown13DC)) ReturnJtToSafePlace(murray, 0);
        }
        if (murray->flopSubstate == s || murray->flopSubstate == -1) break;
    }
    switch (murray->flopSubstate) {
    case 0:
    case 1:
        murray->qwUnknown190 = SetVectorCylind(murray->controller->leftStickAngle + 3.14159265f, D_002C09A0 * D_002BF730 * murray->controller->leftStickDeflection, 0.0f);
        break;
    }
}
} // namespace M_func_001B6BA8

namespace M_func_001B6CC0 {
extern qword g_normalZ __asm__("g_normalZ");
enum { FACTOR_ADDRESS = 0x002C09A4 };
extern void AddSoAcceleration(MURRAY *p, qword v) __asm__("AddSoAcceleration");
// Return a scaled by g.
static inline qword Scale(qword a, float g) { QWROW u; qword k, r; __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(g)); u.q = k; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(a), "j"(u.q)); return r; }
// Store k to u, return a scaled by k, then store negative to u (register-pinned).
static inline qword Factor(qword a,qword k,qword negative,QWROW *pu) {
    QWROW &u=*pu; register qword r __asm__("vf3");
    __asm__("sqc2 %2,%0\n\tvmulx.xyzw %1,%3,%2x\n\tsqc2 %4,%0":"=m"(u.q),"=&j"(r):"j"(k),"j"(a),"j"(negative));
    return r;
}
// Projection of a onto b: (a.b) b (register-pinned).
static inline qword Projection(qword a,qword b) {
    register qword r __asm__("vf3"),one __asm__("vf4");
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvmulx.xyzw %0,%3,%0x":"=&j"(r),"=&j"(one):"j"(a),"j"(b));
    return r;
}
/* The load replaces its address carrier with the actual scalar word. */
static inline int ReadFactor(void){unsigned int bits=FACTOR_ADDRESS & ~0xFFFF;__asm__("lw %0,%1":"+r"(bits):"m"(((int*)bits)[(FACTOR_ADDRESS&0xFFFF)/4]));return bits;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char U_Size[sizeof(QWROW)==16?1:-1];
typedef char int_Size[sizeof(int)==4?1:-1];
typedef char U_q_Offset[((unsigned int)&(((QWROW*)0x10000)->q)-0x10000)==0?1:-1];
typedef char Factor_Address[FACTOR_ADDRESS==0x002C09A4?1:-1];
// State 3 physics helper: sub-state 0 cancels ground velocity, sub-state 1 adds horizontal correction toward the
// desired point.
// Also known as: UpdateMurrayFlopMotion (upstream readable / Sly 1 name; UpdateMurrayFlopMotion__FP6MURRAY).
extern "C" void PresetMurrayFlopAccel(MURRAY *murray)
{
    switch (murray->flopSubstate) {
    case 0:
        AddSoAcceleration(murray, Scale(*(qword *)&murray->physics->gravity, -1.0f));
    case 1: {
        qword v = murray->GetVelocity();
        QWROW u; register qword kn __asm__("vf5"); register qword n __asm__("vf1");
        __asm__("qmtc2.ni %1,%0":"=j"(kn):"r"(-1.0f));
        __asm__("vmulx.xyzw %0,%1,%2x":"=j"(n):"j"(murray->qwUnknown190),"j"(kn));
        int bits = ReadFactor();
        qword d = VuSub(n, v);
        qword up = g_normalZ;
        qword projected = Projection(d,up);
        /* Projection has retired vf4; both outputs feed the final scale. */
        register qword factor __asm__("vf4"); register qword difference __asm__("vf1");
        __asm__("qmtc2.ni %2,%1\n\tvsub.xyzw %0,%3,%4":"=j"(difference),"=&j"(factor):"r"(bits),"j"(d),"j"(projected));
        d=difference;
        qword f = Factor(d, factor, kn, &u);
        AddSoAcceleration(murray, f);
        break;
    }
    case 2:
        break;
    }
}
} // namespace M_func_001B6CC0
#undef FIELD_OFFSET

namespace M_SetMurrayFlopSubstate {
// Update attachment state and projected velocity.
extern qword g_normalZ __asm__("g_normalZ");
union WORD{int bits;float value;};extern union WORD D_002C09A8 __asm__("D_002C09A8");
struct SIDE { int x0; int x4; void *x8; void *xC; qword v10; char pad[0x30 - 0x20]; qword v30; };
struct LINK { struct SIDE as[2]; char pad[0xAC - 0x80]; struct LINK *pnext; };
/* Any BASIC as the register-pinned dive code reads it: the g++ vtable pointer at +0xC. */
struct VOBJ { char pad[0xC]; VTE *vt; };
void FreeSwXpList__FP2SWP2XP(void *pg, XP *pl) __asm__("FreeSwXpList__FP2SWP2XP");
void *FindZapResponder(void *p, int n) __asm__("FindZapResponder");
void *func_002333B4(void *p, int c, int cb) __asm__("memset");
void *DeliverZapToSo(ZPR *pzpr) __asm__("DeliverZapToSo");
EA *FindAsegLabel(void *p, int n) __asm__("FindAsegLabel");
void SeekAsega(ASEGA *p, int n, float g, float h) __asm__("SeekAsega");
void RetractAsega(ASEGA *p) __asm__("RetractAsega");
// Zero the xyz components of pv.
extern "C" { static inline void Zero(VECTOR *pv) { pv->z = 0.0f; pv->y = 0.0f; pv->x = 0.0f; } }
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char WORD_Size[sizeof(union WORD)==4?1:-1];
typedef char SIDE_Size[sizeof(struct SIDE)==64?1:-1];
typedef char LINK_Size[sizeof(struct LINK)==176?1:-1];
typedef char SIDE_x0_Offset[FIELD_OFFSET(SIDE,x0)==0?1:-1];
typedef char SIDE_x4_Offset[FIELD_OFFSET(SIDE,x4)==4?1:-1];
typedef char SIDE_x8_Offset[FIELD_OFFSET(SIDE,x8)==8?1:-1];
typedef char SIDE_xC_Offset[FIELD_OFFSET(SIDE,xC)==12?1:-1];
typedef char SIDE_v10_Offset[FIELD_OFFSET(SIDE,v10)==16?1:-1];
typedef char SIDE_v30_Offset[FIELD_OFFSET(SIDE,v30)==48?1:-1];
typedef char LINK_as_Offset[FIELD_OFFSET(LINK,as)==0?1:-1];
typedef char LINK_pnext_Offset[FIELD_OFFSET(LINK,pnext)==172?1:-1];
// Flop sub-states: 1 dive (sets downward velocity), 2 impact (zaps every contact and shakes the camera).
// Also known as: SetMurrayFlopPhase (upstream readable / Sly 1 name; SetMurrayFlopPhase__FP6MURRAYi).
extern "C" void SetMurrayFlopSubstate(MURRAY *murray, int s)
{
    if (murray->flopSubstate == s)
        return;
    if (murray->flopSubstate == -1)
        Zero(&murray->unknown190);
    murray->flopSubstate = s;
    murray->unknown13DC = g_clock.time;
    switch (s) {
    case 0:
        break;
    case -1:
        FreeSwXpList__FP2SWP2XP(&g_sw, murray->pxpFlop);
        murray->pxpFlop = 0;
        break;
    case 1: {
        union { qword q; float f[4]; } u;
        register qword v __asm__("vf3"),up __asm__("vf2"),d __asm__("vf1"),r __asm__("vf1"),k __asm__("vf4");
        register unsigned int bits __asm__("$2");
        register VTE*vt __asm__("$5");
        register int delta __asm__("$4");
        register void(*setter)(void*,qword) __asm__("$2");
        register qword argument __asm__("$5");
        v=murray->GetVelocity();
        __asm__("lw %0,%3\n\tlqc2 %1,%4\n\tlui %2,%5":"=&r"(vt),"=j"(up),"=r"(bits):"m"(((VOBJ*)murray)->vt),"m"(g_normalZ),"I"(0x2C));
        __asm__("vmul.xyz %0,%3,%4\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tlw %2,%5":"=&j"(d),"=&j"(k),"+r"(bits):"j"(v),"j"(up),"m"(((int*)bits)[0x9A8/4]));
        __asm__("vmulx.xyzw %0,%2,%0x\n\tlh %1,%3":"+j"(d),"=r"(delta):"j"(up),"m"(vt[35].d));
        __asm__("qmtc2.ni %5,%1\n\tvsub.xyzw %0,%0,%6\n\tlw %2,%8\n\tvadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %3,%7,%1x\n\tqmfc2.ni %4,%3":"+&j"(v),"=&j"(k),"=r"(setter),"=j"(r),"=r"(argument):"r"(bits),"j"(d),"j"(up),"m"(vt[35].pfn));
        u.q=k;
        setter((char*)murray+delta,argument);
        murray->unknown131C = g_clock.time;
        break;
    }
    case 2: {
        struct LINK *pl;
        Zero(&murray->unknown190);
        for (pl = (struct LINK *)murray->pxpFlop; pl; pl = pl->pnext) {
            int i = pl->as[0].x8 != murray;
            struct SIDE *so = &pl->as[1 - i];
            struct SIDE *stateMachine = &pl->as[i];
            void *object = FindZapResponder(so->xC, 4);
            if (object) {
                ZPR msg;
                func_002333B4(&msg, 0, sizeof(msg));
                msg.ploSource = murray;
                msg.part = (SOPART *)stateMachine->x0;
                msg.unknown08 = (LO *)object;
                msg.zpk = 4;
                msg.dhp = -20;
                msg.position = so->v10;
                msg.velocity = stateMachine->v30;
                (void)DeliverZapToSo(&msg);
            }
        }
        FreeSwXpList__FP2SWP2XP(&g_sw, murray->pxpFlop);
        murray->pxpFlop = 0;
        {
            EA *pea = FindAsegLabel(murray->activeAnimation->animation, 0x1A5);
            if (pea)
                SeekAsega(murray->activeAnimation, 0, pea->time, 1.0f);
            else
                RetractAsega(murray->activeAnimation);
        }
        break;
    }
    }
}

typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char WORD_bits_Offset[((unsigned int)&(((union WORD*)0)->bits))==0?1:-1];
typedef char WORD_value_Offset[((unsigned int)&(((union WORD*)0)->value))==0?1:-1];
} // namespace M_SetMurrayFlopSubstate
#undef FIELD_OFFSET

namespace M_StartMurrayLiftGrabspot {
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
void FaceStepToward(MURRAY *p, LO *plo) __asm__("FaceStepToward");
// If lifting is allowed: the grab spot becomes the carried target, lift clip 0, state 4, face the object it holds.
extern "C" void StartMurrayLiftGrabspot(MURRAY *murray, LO *pspot)
{
    if (!murray->psoCarryRoot)
        return;
    murray->ploCarried = pspot;
    murray->liftClip = 0;
    SetMurrayState(murray, MURRAY_LIFT);
    FaceStepToward(murray, ((CID36 *)murray->ploCarried)->unknown180);
}
} // namespace M_StartMurrayLiftGrabspot

namespace M_StartMurrayLiftGuard {
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
void FaceStepToward(MURRAY *p, LO *plo) __asm__("FaceStepToward");
// Same as StartMurrayLiftGrabspot with lift clip 1 (the guard-lift clip x1444).
extern "C" void StartMurrayLiftGuard(MURRAY *murray, LO *pspot)
{
    if (!murray->psoCarryRoot)
        return;
    murray->ploCarried = pspot;
    murray->liftClip = 1;
    SetMurrayState(murray, MURRAY_LIFT);
    FaceStepToward(murray, ((CID36 *)murray->ploCarried)->unknown180);
}
} // namespace M_StartMurrayLiftGuard

namespace M_FTryMurrayTakedown {
/* The interaction target is a guard (STEPGUARD): its behaviour stack and knockdown helper; the behaviour found is a
   BHVJUGGLED. */
extern float D_002C09AC __asm__("D_002C09AC"), D_002C09B8 __asm__("D_002C09B8");
extern char D_002C09B0[] __asm__("D_002C09B0"), D_002C09C0[] __asm__("D_002C09C0");
int StepguardHasSgf(LO *po, int n) __asm__("StepguardHasSgf");
BHVJUGGLED *FindBhvInBhvstk(void *p, int n) __asm__("FindBhvInBhvstk");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
int IsWithinLimits__FP2LMf(void *p, float g) __asm__("IsWithinLimits__FP2LMf");
void SetBhvjuggledState(BHVJUGGLED *p, int n) __asm__("SetBhvjuggledState");
void StartMurrayLiftGuard(MURRAY *p, void *q) __asm__("StartMurrayLiftGuard");
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
void SetMurrayAttack(MURRAY *p, int n) __asm__("SetMurrayAttack");
// XY length of v as a float.
extern "C" { static inline float LenXY(qword v) {
    qword d;
    float f;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }
// z lane of t as a float.
extern "C" { static __inline__ float GetZ2(qword t) { __asm__("pextuw %0,$0,%0" : "+r"(t)); return QwordX(t); } }
// Move a VU0 register to a 128-bit GPR value.
extern "C" { static inline qword ToGpr(qword v) { qword r; __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(v)); return r; } }
// If the target (interactTarget) is a stunned guard within reach and the attack button is pressed, lifts the guard; returns
// nonzero if it did.
// Also known as: FMurrayGrab (upstream readable / Sly 1 name; FMurrayGrab__FP6MURRAY).
extern "C" int TryMurrayTakedown(MURRAY *murray)
{
    union { qword q; float f[4]; } u;
    BHVJUGGLED *ps;
    if (!murray->interactTarget)
        return 0;
    if (!StepguardHasSgf(murray->interactTarget, 0x17) ||
        !murray->controller->GetButtonsPressedWithin(BUTTON_ACTION | BUTTON_ATTACK, 0.15f))
        goto ret0;
    ps = FindBhvInBhvstk(&((STEPGUARD *)murray->interactTarget)->behaviors, 0xB1);
    if (ps->juggleState >= 3)
        return 0;
    if (ps->holder != murray) {
    ret0:
        return 0;
    }

    int f = 0, g;
    qword a = GetLoWorldPos(murray->interactTarget);
    qword d = GetLoWorldPos(murray);
    __asm__("vsub.xyzw %0, %1, %0" : "+j"(d) : "j"(a));
    u.q = d;
    if (LenXY(d) < D_002C09AC && IsWithinLimits__FP2LMf(D_002C09B0, QwordZ(d)) && ((STEPGUARD *)murray->interactTarget)->knockdownHelper)
        f = murray->controller->ConsumeButtonsPressedWithin(BUTTON_ACTION, 0.15f) != 0;
    if (f) {
        SetBhvjuggledState(ps, 2);
        StartMurrayLiftGuard(murray, ((STEPGUARD *)murray->interactTarget)->knockdownHelper);
        return 1;
    }
    d = u.q;
    g = 0;
    if (LenXY(d) < D_002C09B8 && IsWithinLimits__FP2LMf(D_002C09C0, QwordZ(d)) && murray->apasegAttack[4])
        g = murray->controller->ConsumeButtonsPressedWithin(BUTTON_ATTACK, 0.15f) != 0;
    if (!g)
        goto ret0;
    SetMurrayState(murray, MURRAY_ATTACK);
    SetMurrayAttack(murray, 4);
    return 1;
}
} // namespace M_FTryMurrayTakedown

namespace M_func_001B72B0 {
// GetMurrayGripFrame: rotation (quaternion) and position of `stop` relative to the owner's two end nodes of p->link.
//  - No pose: transform the input basis into the right parent's frame (ConvertXfMat) and take its rotation
//    (DecomposeRotationMatrixToEuler); transform the input position through the left parent (TransformXfPos).
//  - With a pose: collect the parent chain from pose->first up to `stop` (at most 16 nodes) and the indices of the
//    owner's left/right nodes in it, build the relative frame replace = inverse(from * pose * inverse(input) * to)
//    (rotation re-orthonormalised through the quaternion, position = the right node's local position), then
//    accumulate the chain with `replace` substituted for the right node and output the final position.
//
// How the match was found (wave-4 lane 3, from w3lane6.v2 at aligned 16):
//  - Matrix products by value with const-reference operands, nested as written in one expression
//    (`M4 result = Mul(Mul(from, inverse), to)`): the outer call's return slot address is put in a register before the
//    inner call is expanded, so gcse meets &result before the inner temporary (the game's PRE copy order).
//  - The inverse helper uses an output pointer (`void InvertMatrix4Rigid(M4 *, const M4 *)`), so `replace` / `product` are
//    declared first and land in the slots the game uses.
//  - VU_VECTOR by-value parameters/results with an implicit qword constructor: their 16-byte register copies survive cse1
//    and grow the RTL to 272 insns before gcse (137 hash buckets instead of 131), which gives the game's spill-slot order of
//    the PRE address pseudos; they vanish from the final code.
//  - `P *const p`: the spilled pseudo is RTX_UNCHANGING, so reorg sees the reload of p after the chain loop as redundant and
//    the loop guard branches past it, and sched2 hoists the then-branch reload above the rotation store.
//  - LoadRotateMatrixEuler returns void (its result is never used); M4 *pmat = &replace for the basis copy and position.w gives the
//    mixed sp/s4 addressing (cse1 folds (mem (reg)) of a frame address but not (mem (plus reg c))).
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; int i[4]; } U;
typedef struct { U row[3]; } M3;
typedef struct { M3 basis; U position; } M4;
// Wrap a qword as a VU_VECTOR (convertible back to qword).
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } operator qword() const { return q; } };
typedef struct NODE { M4 local, world; char pad80[0x20]; struct NODE *parent; } NODE;
typedef struct { char pad[0x58]; NODE *left, *right; } OWNER;
typedef struct { char pad[0x50]; NODE *first; char pad54[0xC]; M4 transform; } POSE;
typedef struct { char pad[0x180]; OWNER *owner; int pad184; POSE *pose; } LINK;
typedef struct { char pad[0x13F0]; LINK *link; } P;
extern M4 D_002A2D30 __asm__("D_002A2D30");   // identity matrix
extern M3 ConvertXfMat(NODE *, NODE *, const M3 &) __asm__("ConvertXfMat");
extern M4 GetRelativeXfMatrix(NODE *, NODE *, const M4 &) __asm__("GetRelativeXfMatrix");
extern VU_VECTOR TransformXfPos(NODE *, NODE *, VU_VECTOR) __asm__("TransformXfPos");
extern VU_VECTOR DecomposeRotationMatrixToEuler(const M3 *) __asm__("DecomposeRotationMatrixToEuler");
extern void LoadRotateMatrixEuler(M3 *, VU_VECTOR) __asm__("LoadRotateMatrixEuler");
extern qword *VECTOR_opAssign(qword *, VU_VECTOR) __asm__("VECTOR_opAssign");
extern void InvertMatrix4Rigid(M4 *, const M4 *) __asm__("InvertMatrix4Rigid");
extern M4 __ml__FRC7MATRIX4T0(const M4 &, const M4 &) __asm__("__ml__FRC7MATRIX4T0");

// Rotation and position of the carried object relative to Murray's two grip nodes (with an optional pose change).
extern "C" void GetMurrayGripFrame(P *const murray, NODE *stop, const M4 *input, VU_VECTOR *outRotation, VU_VECTOR *outPosition)
{
    if (!murray->link->pose) {
        M3 basis;
        basis = ConvertXfMat(stop, murray->link->owner->right->parent, input->basis);
        *outRotation = DecomposeRotationMatrixToEuler(&basis);
        *outPosition = TransformXfPos(stop, murray->link->owner->left->parent, input->position.q);
        return;
    }
    {
        NODE *chain[16];
        U rotation, finalPosition;

        OWNER *owner = murray->link->owner;
        NODE *node;
        int count = 0, left = -1, right = -1, j;
        M4 inverse;
        InvertMatrix4Rigid(&inverse, input);
        for (node = murray->link->pose->first; node != stop; node = node->parent) {
            chain[count] = node;
            if (node == owner->left) left = count;
            if (node == owner->right) right = count;
            count++;
        }
        M4 from = GetRelativeXfMatrix(murray->link->pose->first, owner->right, D_002A2D30);
        M4 to = GetRelativeXfMatrix(owner->right->parent, stop, D_002A2D30);
        from = __ml__FRC7MATRIX4T0(from, murray->link->pose->transform);
        M4 replace;
        M4 result = __ml__FRC7MATRIX4T0(__ml__FRC7MATRIX4T0(from, inverse), to);
        InvertMatrix4Rigid(&replace, &result);
        rotation.q = DecomposeRotationMatrixToEuler(&replace.basis);
        LoadRotateMatrixEuler(&result.basis, rotation.q);
        VECTOR_opAssign(&replace.position.q, chain[right]->local.position.q);
        M4 *matrix = &replace;
        matrix->basis = result.basis;
        replace.basis.row[0].i[3] = replace.basis.row[1].i[3] = replace.basis.row[2].i[3] = 0;
        matrix->position.f[3] = 1.0f;
        result = D_002A2D30;
        M4 upper = D_002A2D30;
        for (int i = count - 1; i > left; i--) {
            if (i == right) result = __ml__FRC7MATRIX4T0(result, replace);
            else result = __ml__FRC7MATRIX4T0(result, chain[i]->local);
        }
        for (j = left - 1; j >= 0; j--) {
            if (j == right) upper = __ml__FRC7MATRIX4T0(upper, replace);
            else upper = __ml__FRC7MATRIX4T0(upper, chain[j]->local);
        }
        upper = __ml__FRC7MATRIX4T0(upper, murray->link->pose->transform);
        M4 product;
        M4 intermediate = __ml__FRC7MATRIX4T0(__ml__FRC7MATRIX4T0(upper, inverse), result);
        InvertMatrix4Rigid(&product, &intermediate);
        finalPosition = product.position;
        if (outRotation) *outRotation = rotation.q;
        if (outPosition) *outPosition = finalPosition.q;
    }
}
} // namespace M_func_001B72B0

namespace M_func_001B7700 {
/* A transform (XF) as read here: position, parent and velocity. */
struct XFV{char pad[0x30];qword pos;char pad40[0x88 - 0x40]; void *timeSource; void *speedSource; void *firstActuator; char pad94[0xA0 - 0x94];struct XFV*parent;char padA4[0xA8 - 0xA4]; void *unknownA8; char padAC[0xB0 - 0xAC];qword velocity;};
extern qword g_normalZ __asm__("g_normalZ");
extern float D_002C09C8 __asm__("D_002C09C8");
qword ConvertXfVec(struct XFV*,struct XFV*,qword) __asm__("ConvertXfVec");
int SolveQuadratic__FfffPf(float,float,float,float*) __asm__("SolveQuadratic__FfffPf");
void SeekAsega(struct ASEGA*,int,float,float) __asm__("SeekAsega");
// Clamp inputValue to [inputLower, *upper] (register-pinned).
extern "C" { static inline float Clamp(float inputValue,float inputLower,const float*upper){register float value __asm__("$f0")=inputValue;register float lower __asm__("$f12")=inputLower;float limit=*upper;if(value<lower)return lower;if(limit<value)return limit;return value;} }
// Return v scaled by f (broadcast through u).
extern "C" { static inline qword Scale(qword v,float f,QWROW*u){qword s,r;__asm__("qmtc2.ni %1,%0":"=j"(s):"r"(f));u->q=s;__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(r):"j"(v),"j"(u->q));return r;} }
// Final launch scale: the up vector scaled by the bits-as-float factor (register-pinned).
extern "C" { static inline qword FinalLaunchScale(const qword*up,int bits,QWROW*scalar){register qword factor __asm__("vf2"),source __asm__("vf1"),result __asm__("vf3");__asm__("qmtc2.ni %4,%0\n\tlqc2 %1,%5\n\tvmulx.xyzw %2,%1,%0x\n\tsqc2 %0,%3":"=j"(factor),"=j"(source),"=j"(result),"=m"(scalar->q):"r"(bits),"m"(*up));return result;} }
// Return a + b * s, storing s to u.
extern "C" { static inline qword AddScaled(qword a,qword b,qword s,QWROW*u){qword r;u->q=s;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(a),"j"(b),"j"(u->q));return r;} }
// Return *p + b * s.
extern "C" { static inline qword AddScaledMemory(const QWROW*p,qword b,qword s){qword a,r;__asm__("lqc2 %1,%2\n\tvadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%3,%4x":"=j"(r),"=&j"(a):"m"(p->q),"j"(b),"j"(s));return r;} }
// Divide v by s, storing s to u.
extern "C" { static inline qword Div(qword v,qword s,QWROW*u){qword r;u->q=s;__asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(u->q),"j"(v));return r;} }
// Store a VU0 register to *p.
extern "C" { static inline void Store(qword*p,qword q){__asm__("sqc2 %1,%0":"=m"(*p):"j"(q));} }
// Copy the actor pointer into callee-saved $16.
extern "C" { static inline MURRAY*CaptureActor(MURRAY*input){register MURRAY*output __asm__("$16");__asm__("daddu %0,%1,$zero":"=r"(output):"r"(input));return output;} }
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char U_q_Offset[FIELD_OFFSET(QWROW,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(QWROW,f)==0?1:-1];
typedef char qword_PrefixSize[sizeof(qword)==16?1:-1];
typedef char U_PrefixSize[sizeof(QWROW)==16?1:-1];
// Start/end velocities for the lift: mode 0 straight (2500/-2000), mode 1 a ballistic solve toward the hands.
// Also known as: PickupVelocities (upstream readable / Sly 1 name;
//   PickupVelocities__FP6MURRAYG9VU_VECTORPfT2P7VECTOR4T4).
extern "C" void ComputeMurrayLiftLaunch(MURRAY*murray,qword goal,float*animation,float*duration,qword*outStart,qword*outEnd){
 MURRAY*owner=CaptureActor(murray);
 QWROW roots;QWROW u;QWROW start;
 qword end;
 switch(owner->liftClip){
 case 0:{
  struct XFV*parent=((XFV*)((SO*)((CID36*)owner->ploCarried)->unknown180)->positionXf)->parent;
  qword first=ConvertXfVec(0,parent,Scale(g_normalZ,2500.0f,&roots));
  {
   register struct XFV*space __asm__("$4");register int bits __asm__("$2");
   __asm__("sq %3,%0\n\tdaddu %1,%z5,$zero\n\tmfc1 %2,%4":"=m"(start.q),"=r"(space),"=r"(bits):"r"(first),"f"(-2000.0f),"rJ"((struct XFV*)0));
   end=ConvertXfVec(space,parent,FinalLaunchScale(&g_normalZ,bits,&roots));
  }
  break;
 }
 case 1:{
  QWROW v;
  register qword pos __asm__("$18");
  qword acceleration;
  float t;
  int count;
  {
   CID36*x=(CID36*)owner->ploCarried;
   register SO*object __asm__("$3");
   __asm__("lw %0,%1":"=r"(object):"m"(x->unknown180));
   {
    register struct XFV*lo __asm__("$2");register SOPHYS*source __asm__("$5");
    register qword velocity __asm__("$3"),argument __asm__("$6");
    __asm__("lw %0,%2\n\tlw %1,%3":"=&r"(lo),"=r"(source):"m"(object->positionXf),"m"(object->physics));
    __asm__("lq %0,%3\n\tlq %1,%4\n\tlq %2,%5":"=&r"(velocity),"=&r"(argument),"=r"(pos):"m"(lo->velocity),"m"(*(qword*)&source->gravity),"m"(lo->pos));
    v.q=velocity;
    acceleration=ConvertXfVec(0,lo->parent,argument);
   }
  }
  {
   register float accelerationZ __asm__("$f12"),positionZ __asm__("$f0"),goalZ __asm__("$f14");
   register int positionWord __asm__("$3"),accelerationWord __asm__("$2");
   register float half __asm__("$f1");register float*outputRoots __asm__("$4");
   register int goalWord __asm__("$2");register qword velocityWord __asm__("$3");float velocityZ;
   __asm__("pextuw %0,$zero,%5\n\tpextuw %1,$zero,%6\n\tli.s %2,%7\n\tmtc1 %1,%3\n\tdaddu %4,%8,$zero":"=&r"(positionWord),"=&r"(accelerationWord),"=f"(half),"=f"(accelerationZ),"=r"(outputRoots):"r"(pos),"r"(acceleration),"F"(0.5f),"r"(roots.f));
   __asm__("pextuw %0,$zero,%2\n\tmtc1 %3,%1":"=&r"(goalWord),"=f"(positionZ):"r"(goal),"r"(positionWord));
   __asm__("mtc1 %2,%0\n\tlq %1,%3":"=f"(goalZ),"=r"(velocityWord):"r"(goalWord),"m"(v.q));
   __asm__("mul.s %0,%0,%2\n\tpextuw %1,$zero,%3":"+f"(accelerationZ),"=r"(velocityZ):"f"(half),"1"(velocityWord));
   count=SolveQuadratic__FfffPf(accelerationZ,velocityZ,positionZ-goalZ,outputRoots);
  }
  if(!count)t=D_002C09C8;
  else t=Clamp(roots.f[count-1],D_002C09C8,duration);
  SeekAsega(owner->activeAnimation,0,*animation-t*owner->activeAnimation->speed,owner->activeAnimation->speed);
  {
   register qword actualAcceleration __asm__("vf3");register qword qt __asm__("vf1");
   register float averageTime __asm__("$f0")=0.5f;register int bits __asm__("$2");
   qword predictedEnd;
   __asm__("qmtc2.ni %4,%0\n\tmfc1 %3,%5\n\tmul.s %2,%5,%2\n\tqmtc2.ni %3,%1":"=j"(actualAcceleration),"=j"(qt),"+f"(averageTime),"=&r"(bits):"r"(acceleration),"f"(t));
   predictedEnd=AddScaledMemory(&v,actualAcceleration,qt);
   {
    register qword mean __asm__("vf2"),actualPosition __asm__("vf5"),averageScalar __asm__("vf3");
    qword predicted;
    __asm__("lqc2 %1,%4\n\tsqc2 %5,%3\n\tvadd.xyzw %0,%1,%6\n\tmfc1 %2,%7\n\tqmtc2.ni %8,%1":"=j"(mean),"=&j"(actualPosition),"=&r"(bits),"=m"(u.q):"m"(v.q),"j"(qt),"j"(predictedEnd),"f"(averageTime),"r"(pos));
    __asm__("swc1 %2,%0\n\tqmtc2.ni %3,%1":"=m"(*duration),"=j"(averageScalar):"f"(t),"r"(bits));
    predicted=AddScaled(actualPosition,mean,averageScalar,&u);
  {
   qword correction=Div(VuSub(goal,predicted),qt,&u);
   register qword initial __asm__("vf5");register qword final __asm__("vf2");
   __asm__("lqc2 %0,%2\n\tvadd.xyzw %0,%0,%3\n\tvadd.xyzw %1,%4,%3":"=&j"(initial),"=j"(final):"m"(v.q),"j"(correction),"j"(predictedEnd));
   start.q=initial;end=final;
  }}}
  break;
 }
 default:break;
 }
 *outStart=start.q;
 Store(outEnd,end);
}

typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char U_Alignment[__alignof__(QWROW)==16?1:-1];
} // namespace M_func_001B7700
#undef FIELD_OFFSET

namespace M_ThrowMurrayCarried {
void DetachMurrayCarried(MURRAY *p) __asm__("DetachMurrayCarried");
int IsBasicDerivedFrom__FP5BASIC3CID(struct LO *p, int k) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void StunStepguardThrownOp3(struct LO *p) __asm__("StunStepguardThrownOp3");
qword GetLoWorldPos(struct LO *p) __asm__("GetLoWorldPos");
qword GetSoGravity(struct LO *p) __asm__("GetSoGravity");
qword CalculateVelocityToReachPos(qword a, qword b, qword c, float t) __asm__("CalculateVelocityToReachPos");
void SetSoZapMode(struct LO *p, int n) __asm__("SetSoZapMode");
// Distance between a and b as a float.
extern "C" { static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
} }
// Release on animation event 0x1EB in state 7: detaches the carried object and gives it the velocity to reach the
// throw goal.
// Also known as: ReleaseMurrayThrow (upstream readable / Sly 1 name; ReleaseMurrayThrow__FP6MURRAY).
extern "C" void ThrowMurrayCarried(MURRAY *murray)
{
    struct LO *object;
    DetachMurrayCarried(murray);
    object = ((CID36 *)murray->ploCarried)->unknown180;
    murray->ploCarried = 0;
    if ((int)object->isInWorld & 1) {
        float t;
        qword v;
        if (IsBasicDerivedFrom__FP5BASIC3CID(object, 0x38))
            StunStepguardThrownOp3(object);
        t = Dist(GetLoWorldPos(object), murray->throwGoal) / murray->throwSpeed;
        v = CalculateVelocityToReachPos(GetLoWorldPos(object), murray->throwGoal, GetSoGravity(object), t);
        object->SetVelocity(v);
        SetSoZapMode(object, 2);
    }
}
} // namespace M_ThrowMurrayCarried

namespace M_func_001B7A78 {
typedef int qword __attribute__((mode(TI)));
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void SetStepTargetHeadingFromVector(MURRAY *p, qword q) __asm__("SetStepTargetHeadingFromVector");
// Splice-callable: state 7 with the throw goal overridden by the given object's position, facing it.
// Also known as: StartMurrayThrow (upstream readable / Sly 1 name; StartMurrayThrow__FP6MURRAYP2LO).
extern "C" void ThrowMurrayCarriedAt(MURRAY *murray, void *pv)
{
    qword a, b, d;
    SetMurrayState(murray, MURRAY_THROW);
    if (!pv)
        return;
    a = GetLoWorldPos(pv);
    murray->throwGoal = a;
    b = GetLoWorldPos(murray);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    SetStepTargetHeadingFromVector(murray, d);
}
} // namespace M_func_001B7A78

namespace M_func_001B7AE8 {
extern qword D_002C09D0 __asm__("D_002C09D0"), D_002A2DD0 __asm__("D_002A2DD0");
void DetachMurrayCarried(MURRAY *p) __asm__("DetachMurrayCarried");
int IsBasicDerivedFrom__FP5BASIC3CID(SO *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void StunStepguardThrownOp3(SO *p) __asm__("StunStepguardThrownOp3");
qword ConvertVecBetweenLos(MURRAY *p, int n, qword q) __asm__("ConvertVecBetweenLos");
void SetSoZapMode(SO *p, int n) __asm__("SetSoZapMode");
// Release in state 8: detaches, fixed local velocity D_002C09D0, no angular velocity, zap mode 3.
// Also known as: PutDownMurrayCarry (upstream readable / Sly 1 name; PutDownMurrayCarry__FP6MURRAY).
extern "C" void TossMurrayCarried(MURRAY *murray)
{
    SO *object;
    qword q;
    DetachMurrayCarried(murray);
    object = (SO *)((CID36 *)murray->ploCarried)->unknown180;
    murray->ploCarried = 0;
    if ((int)object->isInWorld & 1) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(object, 0x38)) StunStepguardThrownOp3(object);
        q = ConvertVecBetweenLos(murray, 0, D_002C09D0);
        object->SetVelocity(q);
        object->SetAngularVelocity(D_002A2DD0);
        SetSoZapMode(object, 3);
    }
}
} // namespace M_func_001B7AE8

namespace M_DropMurrayCarried {
extern qword D_002C09E0 __asm__("D_002C09E0"), D_002A2DD0 __asm__("D_002A2DD0");
void SetSmaGoal(SMA *psma, int oid) __asm__("SetSmaGoal");
void DetachMurrayCarried(MURRAY *p) __asm__("DetachMurrayCarried");
int IsBasicDerivedFrom__FP5BASIC3CID(SO *x, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void StunStepguardThrownOp2(SO *x) __asm__("StunStepguardThrownOp2");
void SetSoZapMode(SO *x, int n) __asm__("SetSoZapMode");
qword ConvertVecBetweenLos(MURRAY *p, int n, qword q) __asm__("ConvertVecBetweenLos");
// Aborts carrying on a forced state change: detaches the carried/lifted object and sets its velocity for the current
// state.
// Also known as: CancelMurrayCarry (upstream readable / Sly 1 name; CancelMurrayCarry__FP6MURRAY).
extern "C" void DropMurrayCarried(MURRAY *murray)
{
    LO *o;
    SO *x;
    if (murray->state == MURRAY_STRUGGLE) {
        if (murray->psmaWork) SetSmaGoal(murray->psmaWork, 0x1CA);
        return;
    }
    o = murray->ploCarried;
    if (!o) return;
    if (murray->state == MURRAY_LIFT && murray->liftSubstate == 0) {
        o->HandleSpliceEvent(6, 0, 0, 0, 0);
        murray->ploCarried = 0;
        return;
    }
    DetachMurrayCarried(murray);
    x = (SO *)((CID36 *)murray->ploCarried)->unknown180;
    if (IsBasicDerivedFrom__FP5BASIC3CID(x, 0x38)) StunStepguardThrownOp2(x);
    else SetSoZapMode(x, 1);
    switch (murray->state) {
    case MURRAY_CARRY:
        x->SetVelocity(ConvertVecBetweenLos(murray, 0, D_002C09E0));
        break;
    case MURRAY_JUMP: case MURRAY_LIFT: case MURRAY_THROW: case MURRAY_TOSS:
        x->SetVelocity(D_002A2DD0);
        break;
    case MURRAY_FLOP: case MURRAY_STRUGGLE:
        break;
    }
    murray->ploCarried = 0;
}
} // namespace M_DropMurrayCarried

namespace M_DetachMurrayCarried {
void ReleaseAct(ACT **ppact) __asm__("ReleaseAct");
void SetXfRole(SO *pso, XF *pxf, int n) __asm__("SetXfRole");
void SetSoTreeFlag2(SO *pso, int a) __asm__("SetSoTreeFlag2");
void SetLoParent(SO *pso, int a, int b) __asm__("SetLoParent");
void RefreshSwObjectCollisionFilter(void *g, MURRAY *p) __asm__("RefreshSwObjectCollisionFilter");
// Frees the 12 grip constraints, notifies the object (message 6), restores its transform roles and tree flags.
extern "C" void DetachMurrayCarried(MURRAY *murray)
{
    LO *o;
    SO *x;
    ReleaseAct(&murray->apactCarryA[0]);
    ReleaseAct(&murray->apactCarryA[1]);
    ReleaseAct(&murray->apactCarryA[2]);
    ReleaseAct(&murray->apactCarryA[3]);
    ReleaseAct(&murray->apactCarryB[0]);
    ReleaseAct(&murray->apactCarryB[1]);
    ReleaseAct(&murray->apactCarryB[2]);
    ReleaseAct(&murray->apactCarryB[3]);
    ReleaseAct(&murray->apactCarryC[0]);
    ReleaseAct(&murray->apactCarryC[1]);
    ReleaseAct(&murray->apactCarryC[2]);
    ReleaseAct(&murray->apactCarryC[3]);
    o = murray->ploCarried;
    if (!o) return;
    o->HandleSpliceEvent(6, 0, 0, 0, 0);
    x = (SO *)((CID36 *)murray->ploCarried)->unknown180;
    SetXfRole(x, x->positionXf, murray->xfRoleA);
    SetXfRole(x, x->rotationXf, murray->xfRoleB);
    SetSoTreeFlag2(x, 0);
    SetLoParent(x, 0, 0xF);
    murray->ploReleased = x;
    murray->releaseTime = g_clock.time;
    RefreshSwObjectCollisionFilter(&g_sw, murray);
}
} // namespace M_DetachMurrayCarried

namespace M_SetMurrayPlayerState {
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
void DropMurrayCarried(MURRAY *p) __asm__("DropMurrayCarried");
// MURRAY slot 88: maps a generic player state 0..8 to Murray states (0 -> carry 5 or idle 0, 1 -> jump 2, ...).
// Also known as: SetMurrayJumpState (upstream readable / Sly 1 name; SetMurrayJumpState__FP2LOi).
extern "C" void SetMurrayPlayerState(MURRAY *murray, unsigned int k)
{
    switch (k) {
    case 0:
        if (murray->ploCarried) SetMurrayState(murray, MURRAY_CARRY);
        else SetMurrayState(murray, MURRAY_IDLE);
        break;
    case 1:
        switch (murray->jtair.jumpKind) { case 15: case 16: break; default: DropMurrayCarried(murray); }
        SetMurrayState(murray, MURRAY_JUMP);
        break;
    case 2: SetMurrayState(murray, MURRAY_GRAB); break;
    case 3: DropMurrayCarried(murray); SetMurrayState(murray, MURRAY_HURT); break;
    case 4: SetMurrayState(murray, MURRAY_PLAYER_STATE_4); break;
    case 5: DropMurrayCarried(murray); SetMurrayState(murray, MURRAY_SCRIPT_ANIM); break;
    case 6: DropMurrayCarried(murray); SetMurrayState(murray, MURRAY_SCRIPT_SMA); break;
    case 7: DropMurrayCarried(murray); SetMurrayState(murray, MURRAY_SCRIPT_THROW); break;
    case 8: DropMurrayCarried(murray); SetMurrayState(murray, MURRAY_PIPE); break;
    }
}
} // namespace M_SetMurrayPlayerState

namespace M_GetMurrayPlayerState {
// MURRAY slot 89: Murray state -> generic player state 0..8 or -1.
// Also known as: MurrayJumpKind (upstream readable / Sly 1 name; MurrayJumpKind__FP6MURRAY).
extern "C" int GetMurrayPlayerState(MURRAY *murray)
{
    switch (murray->state) {
    case MURRAY_IDLE: case MURRAY_CARRY: return 0;
    case MURRAY_JUMP: return 1;
    case MURRAY_GRAB: return 2;
    case MURRAY_HURT: return 3;
    case MURRAY_PLAYER_STATE_4: return 4;
    case MURRAY_SCRIPT_ANIM: return 5;
    case MURRAY_SCRIPT_SMA: return 6;
    case MURRAY_SCRIPT_THROW: return 7;
    case MURRAY_PIPE: return 8;
    }
    return -1;
}
} // namespace M_GetMurrayPlayerState

namespace M_func_001B8008 {
SOPART *GetJtGrabAdjustRecord(JTGRAB *p, int *pk) __asm__("GetJtGrabAdjustRecord");
SOPART *GetJtHurtAdjustRecord(JTHURT *p, int *pk) __asm__("GetJtHurtAdjustRecord");
SOPART *GetJtAirAdjustRecord(JTAIR *p, int *pk) __asm__("GetJtAirAdjustRecord");
void SetStepPhys(MURRAY *p, SOPART *v, int k) __asm__("SetStepPhys");
// MURRAY slot 90: chooses the per-state STEP parameter block (component-supplied for 2/13/14, carry table ...).
extern "C" void ApplyMurrayStateStepParams(MURRAY *murray)
{
    int k = 0;
    SOPART *v;
    switch (murray->state) {
    case MURRAY_GRAB: v = GetJtGrabAdjustRecord(&murray->jtgrab, &k); break;
    case MURRAY_HURT: v = GetJtHurtAdjustRecord(&murray->jthurt, &k); break;
    case MURRAY_JUMP:
        if (murray->ploCarried) v = murray->aprecCarry[3];
        else v = GetJtAirAdjustRecord(&murray->jtair, &k);
        break;
    case MURRAY_CARRY: v = murray->aprecCarry[murray->carrySubstate]; break;
    default: v = murray->aprecState[murray->state]; break;
    }
    if (!v) v = murray->ppartPhysMesh;
    SetStepPhys(murray, v, k);
}
} // namespace M_func_001B8008

/* MURRAY slot 91 (empty). */
// MURRAY slot 91 (end the melee attack, see JT): empty.
extern "C" void EndMurrayMeleeAttack()
{
}

/* MURRAY slot 92. */
// MURRAY slot 92 (face the attached object, see JT): returns 0.
extern "C" int TryFaceMurrayAttachedObject()
{
    return 0;
}

namespace M_func_001B8100 {
int GetStepplayerMovementNoise(MURRAY *p) __asm__("GetStepplayerMovementNoise");
// MURRAY slot 98, movement-noise class (inferred; see GetStepplayerMovementNoise): the base result unless it is -1;
// then 1 (noisy) in MURRAY_RUN/POWER_MOVE or when strafing with a sub-state, else -1.
extern "C" int GetMurrayMovementNoise(MURRAY *murray)
{
    int v = GetStepplayerMovementNoise(murray);
    if (v != -1) return v;
    switch (murray->state) {
    case MURRAY_RUN: case MURRAY_POWER_MOVE:
        return 1;
    case MURRAY_STRAFE:
        if (murray->strafeSubstate) v = 1;
        return v;
    }
    return -1;
}
} // namespace M_func_001B8100

// MURRAY slot 94: 1 while Murray is in a free-movement state (idle, run, strafe, walk), else 0.
int MURRAY::IsFreeMoving()
{
    switch (state) {
    case MURRAY_IDLE: case MURRAY_RUN: case MURRAY_STRAFE: case MURRAY_WALK:
        return 1;
    }
    return 0;
}

namespace M_func_001B81B8 {
void IgnoreStepSlot87(MURRAY *p, int b) __asm__("IgnoreStepSlot87");
int IsBasicDerivedFrom__FP5BASIC3CID(LO *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// MURRAY slot 87 (playable flag): STEP base, then propagates to the carried object.
extern "C" void SetMurrayPlayable(MURRAY *p, int b)
{
    LO *o;
    IgnoreStepSlot87(p, b);
    if (b) {
        if (!p->ploCarried) return;
        o = ((CID36 *)p->ploCarried)->unknown180;
        if (!o) return;
        p->jtmode.ploHidden = o;
        o->SetFlag8(0);
        if (IsBasicDerivedFrom__FP5BASIC3CID(p->jtmode.ploHidden, 0x33)) ((STEP *)p->jtmode.ploHidden)->SetPlayable(b);
    } else {
        o = p->jtmode.ploHidden;
        if (!o) return;
        o->SetFlag8(1);
        if (IsBasicDerivedFrom__FP5BASIC3CID(p->jtmode.ploHidden, 0x33)) ((STEP *)p->jtmode.ploHidden)->SetPlayable(0);
        p->jtmode.ploHidden = 0;
    }
}
} // namespace M_func_001B81B8
#undef VCALL

namespace M_func_001B82A0 {
/* The floor contact (STEP::floorContact; XP has no shared definition yet) and its surface material flags. */
struct M { char pad[0x148]; unsigned long f148; };
struct Q { char pad[0x40]; int x40; char pad2[8]; struct M *pm; char pad3[0x10]; qword q60; char pad4[0x18]; float f88; };
void AddJtPlaceAtPosition(void *p, int a, qword q) __asm__("AddJtPlaceAtPosition");
void RecordJtGrabPlace(void *p) __asm__("RecordJtGrabPlace");
void RecordJtPipePlace(void *p) __asm__("RecordJtPipePlace");
// Per frame: on flat ground (normal z > 0.7, material flag clear) records a safe place; component hooks otherwise.
extern "C" void UpdateMurrayPlaces(MURRAY *murray)
{
    struct Q *pq;
    switch (murray->state) {
    case MURRAY_IDLE: case MURRAY_RUN: case MURRAY_LIFT: case MURRAY_CARRY: case MURRAY_THROW: case MURRAY_TOSS: case MURRAY_STRAFE: case MURRAY_WALK: case MURRAY_SCRIPT_SMA: case MURRAY_TURNBUCKLE:
        pq = (Q *)murray->floorContact;
        if (pq && pq->f88 > 0.7f && !(pq->pm->f148 & 0x20000000) && !murray->pwater)
            AddJtPlaceAtPosition(&murray->jtplaces, pq->x40, pq->q60);
        break;
    case MURRAY_GRAB: RecordJtGrabPlace(&murray->jtgrab); break;
    case MURRAY_PIPE: RecordJtPipePlace(&murray->jtpipe); break;
    }
}
} // namespace M_func_001B82A0

namespace M_func_001B8368 {
int GetStepplayerGroundProbeFlags(MURRAY *p) __asm__("GetStepplayerGroundProbeFlags");
// MURRAY slot 85: 0 if x204 is set, in mode 1 or in state 20, else the STEPPLAYER base.
// Also known as: GrfMurrayXp (upstream readable / Sly 1 name; GrfMurrayXp__FP3ALO).
extern "C" int GetMurrayGroundProbeFlags(MURRAY *murray)
{
    if (murray->unknown204) return 0;
    if (murray->flopSubstate == 1 || murray->state == MURRAY_PIPE) return 0;
    return GetStepplayerGroundProbeFlags(murray);
}
} // namespace M_func_001B8368

namespace M_func_001B83B8 {
int IsJtGrabBusy(JTGRAB *p, int k) __asm__("IsJtGrabBusy");
int IsStepplayerInvulnerable(MURRAY *p, int k) __asm__("IsStepplayerInvulnerable");
// MURRAY slot 76 (zap kind): true in state 15, in state 20 for kind 0x4000, the component check, else the base.
extern "C" int IsMurrayInvulnerable(MURRAY *murray, int k)
{
    switch (murray->state) {
    case MURRAY_PIPE:
        if (k == 0x4000)
            return 1;
        break;
    case MURRAY_AIMED_THROW:
        return 1;
    case MURRAY_GRAB:
        if (IsJtGrabBusy(&murray->jtgrab, k))
            return 1;
        break;
    }
    return IsStepplayerInvulnerable(murray, k);
}
} // namespace M_func_001B83B8

namespace M_MURRAY_FIgnoreIntersection {
int ShouldStepplayerIgnoreIntersection__10STEPPLAYERP2SO(MURRAY *p, void *pv) __asm__("ShouldStepplayerIgnoreIntersection__10STEPPLAYERP2SO");
int IsLoSelfOrDescendantOf(void *pv, void *p1) __asm__("IsLoSelfOrDescendantOf");
int MaxInt(int n, int k) __asm__("MaxInt");
int AdjustJtThrowFindFlags(JTTHROW *pjtthrow, void *pv, int n) __asm__("AdjustJtThrowFindFlags");
// MURRAY slot 61: base flags, plus bit 3 for an object that is (part of) the carried object.
// Also known as: MurrayCarryOverlapKind (upstream readable / Sly 1 name; MurrayCarryOverlapKind__FP3ALOT0).
extern "C" int ShouldMurrayIgnoreIntersection(MURRAY *murray, void *pv)
{
    int n = ShouldStepplayerIgnoreIntersection__10STEPPLAYERP2SO(murray, pv);
    if (murray->ploReleased && IsLoSelfOrDescendantOf(pv, murray->ploReleased))
        n = MaxInt(n, 3);
    else if (murray->state == MURRAY_AIMED_THROW)
        n = AdjustJtThrowFindFlags(&murray->jtthrowAim, pv, n);
    return n;
}
} // namespace M_MURRAY_FIgnoreIntersection

// True if no attack overlay is running and no attack sub-state, else the combo-window flag.
extern "C" int CanMurrayAttack(MURRAY *murray)
{
    if (murray->unknown190C != 0 || murray->attackSubstate != -1) {
        return murray->comboWindow;
    }
    return 1;
}

namespace M_StartMurrayPunch {
typedef int qword __attribute__((mode(TI)));
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword ConvertVecBetweenLos(void *a, void *b, qword q) __asm__("ConvertVecBetweenLos");
void SetMurrayAttack(MURRAY *p, int k) __asm__("SetMurrayAttack");
// Square attack: state 11 with attack kind 0/1 (alternating hand, chosen by the target's side).
// Also known as: StartMurrayPunch (upstream readable / Sly 1 name; StartMurrayPunch__FP6MURRAY).
extern "C" void StartMurrayPunch(MURRAY *murray)
{
    int k;
    SetMurrayState(murray, MURRAY_ATTACK);
    if (murray->unknown190C) {
        ASEG *v = murray->unknown190C->animation;
        if (v == murray->apasegAttack[0] || v == murray->apasegAttack[2]) k = 1;
        else k = 0;
    } else if (murray->interactTarget) {
        if (QwordY(ConvertVecBetweenLos(0, murray, VuSub(GetLoWorldPos(murray->interactTarget), GetLoWorldPos(murray)))) > 0.0f) k = 0;
        else k = 1;
    } else
        k = 1;
    SetMurrayAttack(murray, k);
}
} // namespace M_StartMurrayPunch

namespace M_StartMurrayAltAttack {
typedef int qword __attribute__((mode(TI)));
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword ConvertVecBetweenLos(void *a, void *b, qword q) __asm__("ConvertVecBetweenLos");
void SetMurrayAttack(MURRAY *p, int k) __asm__("SetMurrayAttack");
// The 0x10-button attack (needs clips 2 and 3): state 11 with kind 2/3. (0x10 is triangle.)
extern "C" void StartMurrayAltAttack(MURRAY *murray)
{
    int k;
    if (!murray->apasegAttack[2] || !murray->apasegAttack[3]) return;
    SetMurrayState(murray, MURRAY_ATTACK);
    if (murray->unknown190C) {
        ASEG *v = murray->unknown190C->animation;
        if (v == murray->apasegAttack[0] || v == murray->apasegAttack[2]) k = 3;
        else k = 2;
    } else if (murray->interactTarget) {
        if (QwordY(ConvertVecBetweenLos(0, murray, VuSub(GetLoWorldPos(murray->interactTarget), GetLoWorldPos(murray)))) > 0.0f) k = 2;
        else k = 3;
    } else
        k = 3;
    SetMurrayAttack(murray, k);
}
} // namespace M_StartMurrayAltAttack

namespace M_SetMurrayAttack {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
extern float D_002C09F0 __asm__("D_002C09F0");
void ReleaseAsega(void *pp) __asm__("ReleaseAsega");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
float atan2f(float y, float x) __asm__("atan2f");
float NormalizeAngle__Ff(float g) __asm__("NormalizeAngle__Ff");
void SetStepTargetHeading(MURRAY *p, float g) __asm__("SetStepTargetHeading");
void FixStepAngularVelocity(MURRAY *p) __asm__("FixStepAngularVelocity");
void ReleaseSma(void *p) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *q, MURRAY *p, float g, float h, int n, struct ASEGA **ppanim) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
EA *FindAsegLabel(void *q, int n) __asm__("FindAsegLabel");
void SetAsegaChannelWeight(struct ASEGA *pa, float t) __asm__("SetAsegaChannelWeight");
CHN *FindAsegChannel(struct ASEG *p, MURRAY *pp, int a, int b) __asm__("FindAsegChannel");
void ReleaseInteractTargetMark(MURRAY *p) __asm__("ReleaseInteractTargetMark");
// Rotate v by the 3x3 matrix rows m[0..2].
extern "C" { static inline qword MulM3(qword *m, qword v)
{
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z" : "+j"(v) : "j"(m[0]), "j"(m[1]), "j"(m[2]));
    return v;
} }
// Attack sub-state (x1810, clip table x17FC[kind]): resets event flags, auto-faces a target within the cone, starts the
// clip and predicts its end point.
// Also known as: SetMurrayPunch (upstream readable / Sly 1 name; SetMurrayPunch__FP6MURRAYi).
extern "C" void SetMurrayAttack(MURRAY *murray, int k)
{
    void *q;
    float g;
    if (k == murray->attackSubstate)
        return;
    murray->attackSubstate = k;
    murray->attackStartTime = g_clock.time;
    if (k == -1)
        return;
    murray->comboWindow = 0;
    murray->unknown1818 = 0;
    murray->unknown181C = 0;
    if (murray->unknown190C)
        ReleaseAsega(&murray->unknown190C);
    q = murray->apasegAttack[k];
    murray->qwUnknown190 = D_002A2DD0;
    if (murray->interactTarget && murray->nUnk10e4 >= 2 && !(murray->controller->buttonsHeld & 8)) {
        qword d;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(GetLoWorldPos(murray->interactTarget)), "j"(GetLoWorldPos(murray)));
        g = atan2f(QwordY(d), QwordX(d));
        if (__builtin_fabsf(NormalizeAngle__Ff(g - murray->targetHeading)) < D_002C09F0) {
            SetStepTargetHeading(murray, g);
            FixStepAngularVelocity(murray);
        }
    }
    g = 0.0f;
    ReleaseSma(&murray->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(q, murray, g, 1.0f, 1, &murray->activeAnimation);
    if (q) {
        EA *pea = FindAsegLabel(q, 0x1AC);
        if (pea)
            g = pea->time;
    }
    if (g != 0.0f)
        SetAsegaChannelWeight(murray->activeAnimation, g_clock.time + g / murray->activeAnimation->speed);
    if (murray->activeAnimation) {
        CHN *r = FindAsegChannel(murray->activeAnimation->animation, murray, 1, 1);
        if (r) {
            qword a, b, pos, t, v;
            r->pac->Evaluate(0.0f, 0.0f, &a, 0);
            r->pac->Evaluate(murray->activeAnimation->animation->duration, 0.0f, &b, 0);
            pos = GetLoWorldPos(murray);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(t) : "j"(b), "j"(a));
            t = MulM3(murray->aqwTarget, t);
            __asm__("vadd.xyzw %0, %1, %2" : "=j"(v) : "j"(pos), "j"(t));
            murray->unknown1830 = v;
        } else {
            murray->unknown1830 = GetLoWorldPos(murray);
        }
    }
    ReleaseInteractTargetMark(murray);
}
} // namespace M_SetMurrayAttack

namespace M_func_001B8938 {
extern char D_002C09F8[] __asm__("D_002C09F8");
void DetachSoActsMatching(void *a, MURRAY *p, int n, void *s) __asm__("DetachSoActsMatching");
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
// Fades the attack overlay, clears it and returns to idle.
// Also known as: EndMurrayPunch (upstream readable / Sly 1 name; EndMurrayPunch__FP6MURRAY).
extern "C" void CancelMurrayAttack(MURRAY *murray)
{
    DetachSoActsMatching(murray->activeAnimation, murray, 5, D_002C09F8);
    murray->unknown190C = murray->activeAnimation;
    murray->activeAnimation = 0;
    SetMurrayState(murray, MURRAY_IDLE);
}
} // namespace M_func_001B8938

namespace M_UpdateMurrayAttack {
int IsAsegaRetracted(void *p) __asm__("IsAsegaRetracted");
void SetMurrayState(MURRAY *p, int s) __asm__("SetMurrayState");
void SetMurrayAttack(MURRAY *p, int k) __asm__("SetMurrayAttack");
int TryMurrayTakedown(MURRAY *p) __asm__("TryMurrayTakedown");
void CancelMurrayAttack(MURRAY *p) __asm__("CancelMurrayAttack");

// State 11 combo: chains square (0 <-> 1) or 0x10 (2 <-> 3) within the combo window; takedown check; movement cancel.
// Also known as: UpdateMurrayPunchTransitions (upstream readable / Sly 1 name;
//   UpdateMurrayPunchTransitions__FP6MURRAY).
extern "C" void UpdateMurrayAttack(MURRAY *murray)
{
    int s;
    for (;;) {
        s = murray->attackSubstate;
        if (!murray->activeAnimation || IsAsegaRetracted(murray->activeAnimation)) {
            SetMurrayState(murray, MURRAY_IDLE);
        } else {
            switch (murray->attackSubstate) {
            case 0:
            case 1:
                if (CanMurrayAttack(murray) && murray->controller->ConsumeButtonsPressedWithin(BUTTON_ATTACK, 0.25f)) {
                    SetMurrayAttack(murray, murray->attackSubstate == 0);
                } else if (!CanMurrayAttack(murray) || !TryMurrayTakedown(murray)) {
                    if (murray->unknown181C && 0.0f < murray->controller->leftStickDeflection) CancelMurrayAttack(murray);
                }
                break;
            case 2:
            case 3:
                if (CanMurrayAttack(murray) && murray->controller->ConsumeButtonsPressedWithin(BUTTON_ALT_ATTACK, 0.25f)) {
                    SetMurrayAttack(murray, (murray->attackSubstate == 2) ? 3 : 2);
                } else if (!CanMurrayAttack(murray) || !TryMurrayTakedown(murray)) {
                    if (murray->unknown181C && 0.0f < murray->controller->leftStickDeflection) CancelMurrayAttack(murray);
                }
                break;
            }
        }
        if (murray->attackSubstate == s || murray->attackSubstate == -1) break;
    }
}
} // namespace M_UpdateMurrayAttack

namespace M_func_001B8B70 {
struct LIMIT { float lo,hi; };
struct POLY { float a,b,c; };
extern struct POLY D_002C0A20 __asm__("D_002C0A20");
extern struct LIMIT g_lmZeroOne __asm__("g_lmZeroOne");
qword GetStepAsegaTranslation(MURRAY*,ASEGA*,float) __asm__("GetStepAsegaTranslation");
struct EA *FindAsegLabel(void*,int) __asm__("FindAsegLabel");
qword SetVectorCylind(float,float,float) __asm__("SetVectorCylind");
qword ConvertVecBetweenLos(void*,void*,qword) __asm__("ConvertVecBetweenLos");
void FaceStepToward(MURRAY*,void*) __asm__("FaceStepToward");
// Evaluate the quadratic a + b*t + c*t^2.
extern "C" { static inline float Polynomial(struct POLY*p,float t){return p->a+t*(p->b+t*p->c);} }
// Blend value into *velocity by strength g (and its complement), keeping the scalars in *scalar (register-pinned).
extern "C" { static inline void Blend(qword value,float g,qword*velocity,QWROW*scalar){
    register qword input __asm__("vf1"),strength __asm__("vf5"),negative __asm__("vf2"),other __asm__("vf4"),complement __asm__("vf3"),current __asm__("vf1");
    register int bits __asm__("$3"),rest __asm__("$2");
    register float neg __asm__("$f0")=-1.0f;register float one __asm__("$f1")=1.0f;float remain;
    __asm__("mfc1 %0,%2\n\tqmtc2.ni %3,%1":"=r"(bits),"=j"(input):"f"(g),"r"(value));
    __asm__("qmtc2.ni %1,%0":"=j"(strength):"r"(bits));
    __asm__("mfc1 %0,%1":"=r"(bits):"f"(neg));
    __asm__("qmtc2.ni %1,%0":"=j"(negative):"r"(bits));
    remain=one-g;
    scalar->q=negative;
    __asm__("" : "+f"(remain) : : "memory");   /* matching only: keeps this store before the mfc1 below */
    __asm__("vmulx.xyzw %0,%1,%2x":"=j"(other):"j"(input),"j"(negative));
    __asm__("mfc1 %0,%1":"=r"(rest):"f"(remain));
    __asm__("lqc2 %0,%2\n\tqmtc2.ni %3,%1":"=j"(current),"=j"(complement):"m"(*velocity),"r"(rest));
    scalar->q=complement;   /* C: *scalar=(U){complement} (C++ would build the literal as a temporary) */
    __asm__("vmulax.xyzw $ACC,%0,%2x\n\tvmaddx.xyzw %0,%1,%3x":"+j"(current):"j"(other),"j"(complement),"j"(strength));
    *velocity=current;
    scalar->q=strength;
} }
// Store -value into *velocity, keeping the -1 broadcast in *scalar (register-pinned).
extern "C" { static inline void NegateSource(qword value,qword*velocity,QWROW*scalar){
    register qword input __asm__("vf3"),neg __asm__("vf1"),result __asm__("vf2");
    register int bits __asm__("$3");
    float negative=-1.0f;
    __asm__("qmtc2.ni %1,%0":"=j"(input):"r"(value));
    __asm__("mfc1 %0,%2\n\tqmtc2.ni %0,%1":"=&r"(bits),"=j"(neg):"f"(negative));
    __asm__("vmulx.xyzw %0,%1,%2x":"=j"(result):"j"(input),"j"(neg));
    scalar->q=neg;
    *velocity=result;
} }
// Orbit offset vector: XY direction length and the orbit's time, angle and radius loaded for the caller
// (register-pinned).
extern "C" { static inline qword OrbitVector(MURRAY*p){
    register POCTL*orbit __asm__("$2");
    register qword d __asm__("vf1");
    register qword bits __asm__("$2");
    register float magnitude __asm__("$f0"),angle __asm__("$f12"),radius __asm__("$f13"),zero __asm__("$f14");
    __asm__("lw %0,%2\n\tlqc2 %1,%3":"=r"(orbit),"=j"(d):"m"(p->controller),"m"(p->unknown16F0[0].vRoot));
    __asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y":"+j"(d));
    magnitude=orbit->leftStickDeflection;
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));
    angle=orbit->leftStickAngle;
    __asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(d));
    zero=0.0f;
    __asm__("mtc1 %1,%0":"=f"(radius):"r"(bits));
    return SetVectorCylind(angle,magnitude*radius,zero);
} }
// State 11 velocity: animation root motion, blended toward the stick direction between two animation parameters.
// Also known as: UpdateMurrayPunch (upstream readable / Sly 1 name; UpdateMurrayPunch__FP6MURRAY).
extern "C" void UpdateMurrayAttackVelocity(MURRAY*murray){
    QWROW scalar;
    if(murray->activeAnimation){
        qword value=GetStepAsegaTranslation(murray,murray->activeAnimation,murray->activeAnimation->time);
        NegateSource(value,&murray->qwUnknown190,&scalar);
        {
            register int active __asm__("$3")=murray->unknown1818;
            if(active){
            struct EA *start=FindAsegLabel(murray->activeAnimation->animation,0x240);
            struct EA *end=FindAsegLabel(murray->activeAnimation->animation,0x242);
            float t=(murray->activeAnimation->time-start->time)/(end->time-start->time);
            float blend=Clamp(Polynomial(&D_002C0A20,t),g_lmZeroOne.lo,g_lmZeroOne.hi);
            qword desired=OrbitVector(murray);
            desired=ConvertVecBetweenLos(0,murray,desired);
            Blend(desired,blend,&murray->qwUnknown190,&scalar);
            }
        }
    }
    if(murray->interactTarget&&murray->nUnk10e4==4)FaceStepToward(murray,murray->interactTarget);
}
} // namespace M_func_001B8B70

namespace M_func_001B8D30 {
struct M3 { qword row[3]; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");
/* A LEAP (leap.h, no fields yet) as written here: +0x40 its goal position. */
struct LEAPV { char pad[0x40]; qword posGoal; };
// Zero the xyz components of pv.
extern "C" { static __inline__ void Zero(VECTOR *pv) { pv->z = 0; pv->y = 0; pv->x = 0; } }
void StopLeap(LEAP *o) __asm__("StopLeap");
void StartLeap(void *parc, MURRAY *p, int n, LEAP *b) __asm__("StartLeap");
qword GetLoWorldPos(MURRAY *p) __asm__("GetLoWorldPos");
struct M3 *GetLoWorldMatrix(MURRAY *p) __asm__("GetLoWorldMatrix");
void ReleaseSma(SMA **ppsma) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *a, MURRAY *p, float time, float rate, int n, ASEGA **q) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
// State 12 (gadget 20 power move) sub-states: 0 wind-up clip, 1 clip, 2 spawn/launch a script object.
// Also known as: SetMurrayBerserk (upstream readable / Sly 1 name; SetMurrayBerserk__FP6MURRAYi).
extern "C" void SetMurrayPowerSubstate(MURRAY *murray, int st)
{
    ASEG *o;
    int n;
    if (st == murray->subState) return;
    if (murray->subState == 2) {
        StopLeap(murray->pleapPower);
        murray->pleapPower->Release();
        murray->pleapPower = 0;
    }
    murray->subState = st;
    murray->subStateTime = g_clock.time;
    o = 0;
    n = 0;
    switch (st) {
    case 0:
        o = murray->pasegPowerA;
        Zero(&murray->unknown190);
        n = 1;
        break;
    case 1:
        o = murray->pasegPowerB;
        n = 9;
        break;
    case 2: {
        LEAP *q;
        qword b;
        struct M3 *px;
        union { qword q; float f[4]; } u, uv, u2;
        murray->powerContactTrigger = 0;
        murray->pleapPower = &murray->leap;
        StartLeap(&murray->arcLeap, murray, 1, &murray->leap);
        q = murray->pleapPower;
        uv.q = GetLoWorldPos(murray);
        __asm__("mfc1 %0, %1" : "=r"(b) : "f"(-murray->arcLeap.sLeap));
        u.q = b;
        u2.q = u.q;
        px = GetLoWorldMatrix(murray);
        {
            register qword axis __asm__("vf1") = px->row[0];
            register qword origin __asm__("vf2") = uv.q;
            __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %0, %2x"
                    : "+j"(axis) : "j"(origin), "j"(u2.q));
            ((LEAPV *)q)->posGoal = axis;
        }
        murray->qwUnknown190 = D_002A2DD0;
        break;
    }
    case 3:
        o = murray->pasegPowerEnd;
        n = 1;
        break;
    case -1:
        murray->powerContactTrigger = 0;
        return;
    }
    ReleaseSma(&murray->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(o, murray, 0.0f, 1.0f, n, &murray->activeAnimation);
}
} // namespace M_func_001B8D30
#undef FIELD_OFFSET

namespace M_func_001B8EC0 {
extern float D_002C0A30 __asm__("D_002C0A30");
extern float D_002C0A34 __asm__("D_002C0A34");
int IsAsegaRetracted(ASEGA *p) __asm__("IsAsegaRetracted");
void SetMurrayState(MURRAY *p, int s) __asm__("SetMurrayState");
void SetMurrayPowerSubstate(MURRAY *p, int s) __asm__("SetMurrayPowerSubstate");
int IsLeapDone(LEAP *p) __asm__("IsLeapDone");
void UpdateLeap(LEAP *p) __asm__("UpdateLeap");

// State 12 update: advances sub-states on animation end, the contact trigger x1908 (set in AdjustMurrayNewXp) or a
// timeout.
// Also known as: UpdateMurrayBerserkTransitions (upstream readable / Sly 1 name;
//   UpdateMurrayBerserkTransitions__FP6MURRAY).
extern "C" void UpdateMurrayPower(MURRAY *murray)
{
    int s;
    for (;;) {
        s = murray->subState;
        switch (s) {
        case 0:
            if (!murray->activeAnimation || IsAsegaRetracted(murray->activeAnimation)) SetMurrayPowerSubstate(murray, 1);
            break;
        case 1:
            if (!murray->activeAnimation || IsAsegaRetracted(murray->activeAnimation)) {
                SetMurrayState(murray, MURRAY_IDLE);
            } else if (murray->powerContactTrigger) {
                SetMurrayPowerSubstate(murray, 2);
            } else if (!murray->isBehaviorActive && g_clock.time - murray->subStateTime > D_002C0A30) {
                SetMurrayPowerSubstate(murray, 3);
            }
            break;
        case 2:
            if (!murray->pleapPower || IsLeapDone(murray->pleapPower)) {
                if (murray->isBehaviorActive) SetMurrayPowerSubstate(murray, 0);
                else SetMurrayState(murray, MURRAY_IDLE);
            } else {
                UpdateLeap(murray->pleapPower);
            }
            break;
        case 3:
            if (!murray->activeAnimation || IsAsegaRetracted(murray->activeAnimation)) {
                SetMurrayState(murray, MURRAY_IDLE);
            } else if (g_clock.time - murray->subStateTime > D_002C0A34 && murray->controller->state == 1 && 0.0f < murray->controller->leftStickDeflection) {
                SetMurrayState(murray, MURRAY_IDLE);
            }
            break;
        }
        if (murray->subState == s || murray->subState == -1) break;
    }
}
} // namespace M_func_001B8EC0

namespace M_func_001B90B8 {
typedef int qword __attribute__((mode(TI)));
qword GetStepAsegaTranslation(MURRAY *p, ASEGA *pasega, float g) __asm__("GetStepAsegaTranslation");
// State 12 sub-states 1/3: velocity from the animation root motion.
// Also known as: UpdateMurrayBerserk (upstream readable / Sly 1 name; UpdateMurrayBerserk__FP6MURRAY).
extern "C" void UpdateMurrayPowerVelocity(MURRAY *murray)
{
    if (!(murray->subState == 1 || murray->subState == 3))
        return;
    qword v = GetStepAsegaTranslation(murray, murray->activeAnimation, murray->activeAnimation->time);
    qword r;
    union { qword q; float f; } s;
    {
        qword qwsqOut;
        __asm__("qmtc2.ni %1, %0" : "=j"(qwsqOut) : "r"(-1.0f));
        s.q = qwsqOut;
    }
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
    murray->qwUnknown190 = r;
    murray->unknown180 = 1.0f;
}
} // namespace M_func_001B90B8

namespace M_AdjustMurrayNewXp {
typedef union U { int i; float f; } U;
typedef struct Q { char pad[0xC]; VTE *vt; } Q;
typedef struct E { char pad0[0xC]; struct Q *pq; char pad[0x30]; } E;
typedef struct X {
    char pad0[0xC];
    Q *pq;
    char pad10[0x80 - 0x10];
    qword x80;
    char pad90[0x88 - 0x90 + 0x10];
} X;
extern float D_002C3C60[] __asm__("D_002C3C60");
extern float D_002C0A38 __asm__("D_002C0A38");
extern void AdjustStepNewXp(MURRAY *p, X *px, int i) __asm__("AdjustStepNewXp");
extern void AdjustJtHurtXp(void *p, X *px, int i) __asm__("AdjustJtHurtXp");
extern void ResetJtPipeAdjust(void *p, X *px, int i) __asm__("ResetJtPipeAdjust");
extern qword GetCosineBetweenVectors(qword a, qword b) __asm__("GetCosineBetweenVectors");
extern int CloneXpList(void *g, X *px) __asm__("CloneXpList");
extern void AppendXpList(void *g, int r, void *p) __asm__("AppendXpList");

// MURRAY contact adjust (slot 55): STEP base, then per-state contact handling (power-move trigger, flop impact).
// Also known as: OnMurrayXpNew (upstream readable / Sly 1 name; OnMurrayXpNew__FP3ALOP2XPi).
extern "C" void AdjustMurrayNewXp(MURRAY *murray, X *px, int i)
{
    int n;
    AdjustStepNewXp(murray, px, i);
    switch (murray->state) {
    case MURRAY_HURT: AdjustJtHurtXp(&murray->jthurt, px, i); break;
    case MURRAY_POWER_MOVE:
        n = murray->subState;
        if (n == 1) {
            SO *pq = *(SO **)((char *)px + ((n - i) << 6) + 0xC);
            if (pq->IsFixed() && *(float *)((char *)px + 0x88) < 0.7f) {
                qword v = murray->GetVelocity();
                qword r = GetCosineBetweenVectors(v, px->x80);
                float g;
                __asm__("mtc1 %1,%0" : "=f"(g) : "r"(r));
                int f = g < D_002C0A38;
                if (f)
                    murray->powerContactTrigger = n;
            }
        }
        break;
    case MURRAY_FLOP:
        if (murray->flopSubstate == 1 && D_002C3C60[i] * *(float *)((char *)px + 0x88) > 0.7f) {
            AppendXpList(&g_sw, CloneXpList(&g_sw, px), &murray->pxpFlop);
        }
        break;
    case MURRAY_PIPE: ResetJtPipeAdjust(&murray->jtpipe, px, i); break;
    }
}
} // namespace M_AdjustMurrayNewXp

namespace M_UpdateMurrayLift {
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
// State 4: waits for the grab event (fLiftGrabbed), aborts if the object is gone; after attaching goes to carry (state 5)
// when the animation ends.
// Also known as: UpdateMurrayCarryTransitions (upstream readable / Sly 1 name;
//   UpdateMurrayCarryTransitions__FP6MURRAY).
extern "C" void UpdateMurrayLift(MURRAY *murray)
{
    int s;
    for (;;) {
        s = murray->liftSubstate;
        switch (s) {
        case 0: break;
        case 1:
            if (murray->fLiftGrabbed) {
                if (!murray->ploCarried) SetMurrayState(murray, MURRAY_IDLE);
                else if ((((int)((CID36 *)murray->ploCarried)->unknown180->isInWorld & 1) == 0)) {
                    SetMurrayState(murray, MURRAY_IDLE);
                    murray->ploCarried = 0;
                }
            }
            break;
        case 2:
            if (IsAsegaRetracted(murray->activeAnimation)) SetMurrayState(murray, MURRAY_CARRY);
            break;
        }
        if (murray->liftSubstate == s || murray->liftSubstate == -1) break;
    }
}
} // namespace M_UpdateMurrayLift

namespace M_SetMurrayLiftSubstate {
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef unsigned long ulong;
struct M4 { QWROW row[4]; };
struct EVENT { float time; int kind,tag,pad; };
/* STEPPLAYER::unknown1088 as written here: +0x1C an active flag. */
struct PROXYV { char pad[0x1C]; int active; };
struct MOTION;
extern M4 D_002A2D30 __asm__("D_002A2D30");
extern float D_002C0A3C __asm__("D_002C0A3C"),D_002C0A40 __asm__("D_002C0A40");
extern void ReleaseAct(ACT**) __asm__("ReleaseAct");
extern void SetSoCnstrForce(SO*,int) __asm__("SetSoCnstrForce");
extern void SetLoParent(SO*,SO*,int) __asm__("SetLoParent");
extern void SetSoTreeFlag2(SO*,int) __asm__("SetSoTreeFlag2");
extern EA*FindAsegLabel(ASEG*,int) __asm__("FindAsegLabel");
extern void LoadIdentityMatrix4(void*,void*,M4*,M4*,float) __asm__("LoadIdentityMatrix4");
extern M4 ConvertMat4BetweenLos(void*,void*,M4*) __asm__("ConvertMat4BetweenLos");
extern void GetMurrayGripFrame(MURRAY*,XF*,const M4*,qword*,qword*) __asm__("GetMurrayGripFrame");
extern int IsBasicDerivedFrom__FP5BASIC3CID(SO*,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
extern void SendStepguardRootBhvMode6(SO*,int) __asm__("SendStepguardRootBhvMode6");
extern void StunStepguardThrownOp1(SO*) __asm__("StunStepguardThrownOp1");
extern void ComputeMurrayLiftLaunch(MURRAY*,qword,float*,float*,qword*,qword*) __asm__("ComputeMurrayLiftLaunch");
extern ACT*NewBezierAct(void*,void*,void*,float,qword,qword) __asm__("NewBezierAct");
extern qword ConvertXfVec(XF*,XF*,qword) __asm__("ConvertXfVec");
extern ACT*NewPositionAct(void*,void*,void*,int,int,qword) __asm__("NewPositionAct");
extern ACT*NewIkAct(void*,void*,void*,PARTLINK*) __asm__("NewIkAct");
extern ACT*NewMatrixAct(void*,void*,void*,void*,int,M4*) __asm__("NewMatrixAct");
extern void DetachMurrayCarried(MURRAY*) __asm__("DetachMurrayCarried");
extern void SetMurrayState(MURRAY*,int) __asm__("SetMurrayState");
// Zero the xyz words of v.
static inline void ClearXYZ(VECTOR*v){v->x=v->y=v->z=0.0f;}
// Lift sub-states: 1 launches the object toward the hands (constraints, velocity), 2 attaches it.
// Also known as: SetMurrayCarry (upstream readable / Sly 1 name; SetMurrayCarry__FP6MURRAYi).
extern "C" void SetMurrayLiftSubstate(MURRAY *murray, int state)
{
    if (murray->liftSubstate == state)
        return;
    if (murray->liftSubstate == 1) {
        ReleaseAct(&murray->apactCarryA[2]);
        ReleaseAct(&murray->apactCarryA[3]);
    }
    murray->liftSubstate = state;
    murray->liftSubstateTime = g_clock.time;
    SO *owner;
    switch (state) {
    case 0: murray->fLiftGrabbed = 0; break;
    case 1: {
        if (!murray->ploCarried)
            goto failed;
        owner = (SO *)((CID36 *)murray->ploCarried)->unknown180;
        if ((int)owner->isInWorld & 1)
            goto attach;
        else
            goto failed;
    attach:
        SetSoCnstrForce(owner, 0);
        murray->xfRoleA = owner->positionXf->role;
        murray->xfRoleB = owner->rotationXf->role;
        SetLoParent(owner, 0, 15);
        SetSoTreeFlag2(owner, 1);
        M4 predicted;
        float duration;
        EA *event = FindAsegLabel(murray->activeAnimation->animation, 0x1C7);
        duration = (event->time - murray->activeAnimation->time) / murray->activeAnimation->speed;
        LoadIdentityMatrix4(murray->psoCarryRoot->rootTransform, murray->rootTransform, &predicted, 0, duration);
        M4 work = ConvertMat4BetweenLos(murray, 0, &predicted);
        predicted = work;
        GetMurrayGripFrame(murray, 0, &predicted, &work.row[0].q, &work.row[1].q);
        if (IsBasicDerivedFrom__FP5BASIC3CID(owner, 0x38))
            SendStepguardRootBhvMode6(owner, (int)murray);
        ComputeMurrayLiftLaunch(murray, work.row[1].q, &event->time, &duration, &work.row[2].q, &work.row[3].q);
        murray->apactCarryA[2] =
            NewBezierAct(owner, owner->positionXf, (void *)5, g_clock.time + duration, work.row[1].q, work.row[3].q);
        owner->SetVelocity(ConvertXfVec(owner->positionXf->parent, 0, work.row[2].q));
        murray->apactCarryA[3] = NewPositionAct(owner, owner->rotationXf, (void *)5, 7, 2, work.row[0].q);
        break;
    }
    case 2: {
        if (!murray->ploCarried)
            goto failed;
        owner = (SO *)((CID36 *)murray->ploCarried)->unknown180;
        if ((int)owner->isInWorld & 1)
            goto valid;
    failed:
        DetachMurrayCarried(murray);
        murray->ploCarried = 0;
        SetMurrayState(murray, MURRAY_IDLE);
        break;
    valid:
        SO *root = murray->psoCarryRoot;
        SetLoParent(owner, root, 15);
        SetSoTreeFlag2(owner, 1);
        QWROW position, rotation;
        M4 frame;
        GetMurrayGripFrame(murray, root->rootTransform, &D_002A2D30, &rotation.q, &position.q);
        murray->apactCarryA[1] = NewPositionAct(owner, owner->rotationXf, (void *)5, 7, 2, rotation.q);
        murray->apactCarryA[0] = NewPositionAct(owner, owner->positionXf, (void *)5, 7, 2, position.q);
        if ((SO *)((CID36 *)murray->ploCarried)->plo2) {
            SO *goal = (SO *)((CID36 *)murray->ploCarried)->plo2;
            PARTLINK *stretch = murray->ikChainA;
            stretch->pxfGoal = goal->GetLocalTransform((MATRIX4 *)&frame);
            stretch->posGoal = frame.row[3].q;
            stretch->gTuning = D_002C0A3C;
            murray->apactCarryC[0] = NewIkAct(murray, stretch->precA, (void *)5, stretch);
            murray->apactCarryC[1] = NewIkAct(murray, stretch->precB, (void *)5, stretch);
            murray->apactCarryB[1] =
                NewMatrixAct(murray, murray->unknown15F4, (void *)5, (void *)5, (int)stretch->pxfGoal, &frame);
        }
        if ((SO *)((CID36 *)murray->ploCarried)->plo3) {
            SO *goal = (SO *)((CID36 *)murray->ploCarried)->plo3;
            PARTLINK *stretch = murray->ikChainB;
            stretch->pxfGoal = goal->GetLocalTransform((MATRIX4 *)&frame);
            stretch->posGoal = frame.row[3].q;
            stretch->gTuning = D_002C0A40;
            murray->apactCarryC[2] = NewIkAct(murray, stretch->precA, (void *)5, stretch);
            murray->apactCarryC[3] = NewIkAct(murray, stretch->precB, (void *)5, stretch);
            murray->apactCarryB[3] =
                NewMatrixAct(murray, murray->unknown15FC, (void *)5, (void *)5, (int)stretch->pxfGoal, &frame);
        }
        if (IsBasicDerivedFrom__FP5BASIC3CID(owner, 0x38))
            StunStepguardThrownOp1(owner);
        ClearXYZ(&murray->unknown190);
        break;
    }
    default: break;
    }
    do {
        if (murray->liftSubstate >= 2)
            break;
        if (murray->liftSubstate < 0)
            break;
        ((PROXYV *)murray->unknown1088)->active = 1;
        goto end;
    } while (0);
    ((PROXYV *)murray->unknown1088)->active = 0;
end:;
}
} // namespace M_SetMurrayLiftSubstate
#undef FIELD_OFFSET

// State 15: aim/throw mode controlled by the throw controller (JTTHROW).
extern "C" void EnterMurrayThrowAim(MURRAY *murray, int n, void *pv)
{
    SetMurrayState(murray, 0xF, pv);
}

namespace M_FCanUseGadget__6MURRAYi {
/* CanUseGadget (Claude agent, 2026-10-17): matched. The p/k s0/s1 swap came from declaring IsStepplayerInJtMode with (p, k): it takes only p (as in CanUseGadget), and the extra k use raised k's allocation priority. */
extern int g_grfjoyt __asm__("g_grfjoyt");
int IsStepplayerInJtMode(MURRAY *p) __asm__("IsStepplayerInJtMode");
int CanJtUseGadgetInAir(void *p, int k) __asm__("CanJtUseGadgetInAir");
int CanJtUseGadgetInGrab(void *p, int k) __asm__("CanJtUseGadgetInGrab");
int CanJtUseGadgetOnPipe(void *p, int k) __asm__("CanJtUseGadgetOnPipe");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
int IsCameraPolicyActive__FP5CPLCY(void *p) __asm__("IsCameraPolicyActive__FP5CPLCY");
// MURRAY slot 99: whether gadget/power k can be used in the current state (per-state allow lists, components).
// Also known as: FMurrayAbilityOk (upstream readable / Sly 1 name; FMurrayAbilityOk__FP6MURRAYi).
extern "C" int CanMurrayUseGadget__6MURRAYi(MURRAY *p, int k)
{
    if (IsStepplayerInJtMode(p)) return 0;
    switch (p->state) {
    case MURRAY_IDLE: case MURRAY_RUN: case MURRAY_STRAFE: case MURRAY_WALK:
        break;
    case MURRAY_JUMP:
        if (!CanJtUseGadgetInAir(&p->jtair, k)) return 0;
        break;
    case MURRAY_GRAB:
        if (!CanJtUseGadgetInGrab(&p->jtgrab, k)) return 0;
        break;
    case MURRAY_PIPE:
        if (!CanJtUseGadgetOnPipe(&p->jtpipe, k)) return 0;
        break;
    case MURRAY_CARRY:
        if (k == 3) return 1;
        if (k == 19) { if (IsBasicDerivedFrom__FP5BASIC3CID(((CID36 *)p->ploCarried)->unknown180, 0x38)) return 1; }
        return 0;
    default:
        return 0;
    }
    switch (k) {
    case 3:
    {
        int r = 0;
        if (p->unknown12F0 && (g_grfjoyt & 2)) r = !IsCameraPolicyActive__FP5CPLCY((*(char (*)[])&g_cm.part3a0));
        return r;
    }
    case 22:
    {
        int r = 0;
        if (p->jtair.jumpKind != 0 && p->jtair.jumpKind != 11) r = p->jtair.jumpKind != -1;
        return r;
    }
    case 1: case 2: case 15: case 16: case 20: case 21:
        break;
    default:
        return 0;
    }
    return 1;
}
} // namespace M_FCanUseGadget__6MURRAYi

namespace M_UseMurrayGadget {
void BroadcastGameEvent(void *p, int a, MURRAY *b) __asm__("BroadcastGameEvent");
void ChargeGadgetMp(MURRAY *p, int k) __asm__("ChargeGadgetMp");
void StartStepplayerGadget(MURRAY *p, int k) __asm__("StartStepplayerGadget");
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
void SetMurrayPowerSubstate(MURRAY *p, int n) __asm__("SetMurrayPowerSubstate");
void SetJtGrabState(JTGRAB *p, int a, LO *plo, int c) __asm__("SetJtGrabState");
// MURRAY slot 100: broadcasts game event 0x3C, then per power: 20 power move (state 12), 19 alternate toss (state 8),
// ...
// Also known as: StartMurrayAbility (upstream readable / Sly 1 name; StartMurrayAbility__FP5ACTORi).
extern "C" void UseMurrayGadget(MURRAY *murray, int k)
{
    g_game.idGadgetLast = k;
    BroadcastGameEvent(&g_game, 0x3C, murray);
    switch (k) {
    case 20:
        ChargeGadgetMp(murray, k);
        SetMurrayState(murray, MURRAY_POWER_MOVE);
        SetMurrayPowerSubstate(murray, 0);
        SetMurrayPowerSubstate(murray, 1);
        break;
    case 15:
        murray->jtgrab.gadgetId = k;
        murray->pasegGadget = murray->pasegGadgetUse;
        SetJtGrabState(&murray->jtgrab, 0xB, 0, 0);
        break;
    case 19:
        ChargeGadgetMp(murray, k);
        murray->fTossGadget = 1;
        SetMurrayState(murray, MURRAY_TOSS);
        break;
    case 22:
        ChargeGadgetMp(murray, k);
        murray->fFlopGadget = 1;
        SetMurrayState(murray, MURRAY_FLOP);
        break;
    case 21:
        murray->jtgrab.gadgetId = 0x15;
        SetJtGrabState(&murray->jtgrab, 0xB, murray->interactTarget, 0);
        break;
    case 16:
        SetMurrayState(murray, MURRAY_TURNBUCKLE);
        break;
    default:
        StartStepplayerGadget(murray, k);
        break;
    }
}
} // namespace M_UseMurrayGadget

// MURRAY slot 103: returns "murray", the save-variable namespace for his hp/mp/gadgets.
extern "C" char *GetMurraySaveVarRoot()
{
    return (char *)D_002A75E0;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const char r_D_002A75E0[] __asm__("D_002A75E0") = "murray"; }
namespace M_func_001B9B70 {
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void GetStepplayerStunHold(MURRAY *p, STUNHOLD *po) __asm__("GetStepplayerStunHold");
// MURRAY slot 96: STEPPLAYER base; during an attack the target point is the predicted attack end.
extern "C" void GetMurrayStunHold(MURRAY *murray, STUNHOLD *object)
{
    GetStepplayerStunHold(murray, object);
    switch (murray->state) {
    case MURRAY_ATTACK:
        object->fValid = 1;
        object->qwPos = murray->unknown1830;
        object->qwVel = D_002A2DD0;
        break;
    case MURRAY_IDLE:
    case MURRAY_RUN:
    case MURRAY_STRAFE:
    case MURRAY_WALK:
        object->fValid = 1;
        break;
    }
    object->gSide = 200.0f;
}
} // namespace M_func_001B9B70

namespace M_func_001B9C00 {
void SetMurrayState(MURRAY *p, int n) __asm__("SetMurrayState");
void SeekSma(void *p, int n) __asm__("SeekSma");
void SetSmaGoal(void *p, int n) __asm__("SetSmaGoal");
// Enters state 6 with a partner state machine and table index; optionally sets or skips both machines to a state.
// Also known as: StartMurrayHold (upstream readable / Sly 1 name; StartMurrayHold__FP6MURRAYP3SMAiii).
extern "C" void StartMurrayStruggle(MURRAY *murray, SMA *psma, int n, int m, int f)
{
    murray->psmaWork = psma;
    murray->workAction = m;
    SetMurrayState(murray, MURRAY_STRUGGLE);
    if (n == -1)
        return;
    if (murray->psmaWork) {
        if (f)
            SeekSma(murray->psmaWork, n);
        else
            SetSmaGoal(murray->psmaWork, n);
    }
    if (f)
        SeekSma(murray->activeStateMachine, n);
    else
        SetSmaGoal(murray->activeStateMachine, n);
}
} // namespace M_func_001B9C00

namespace M_func_001B9CA0 {
void ClearBhvstk__FP6BHVSTK(void *p) __asm__("ClearBhvstk__FP6BHVSTK");
BHV *PushStepBehavior(MURRAY *p, int oid) __asm__("PushStepBehavior");
// Splice-callable: clears Murray's behaviour stack; with the flag set also triggers child object 0xC6 (slot 8).
extern "C" void ClearMurrayBehaviors(MURRAY *murray, int f)
{
    ClearBhvstk__FP6BHVSTK(&murray->behaviors);
    if (!f)
        return;
    BHV *object = PushStepBehavior(murray, 0xC6);
    object->Start();
}
} // namespace M_func_001B9CA0

/* Splice-callable: clear the alternate flag and enter state 3. */
// Splice-callable: clears the alternate flag and enters state 3 (flop).
extern "C" void StartMurrayFlop(MURRAY *murray, int n, void *pv)
{
    murray->fFlopGadget = 0;
    SetMurrayState(murray, 3, pv);
}

namespace M_func_001B9D28 {
int IsPowerupAcquired(int n) __asm__("IsPowerupAcquired");
void ApplyAsegCore(ASEG *paseg, SO *pso, int a, int b, float g0, float g1) __asm__("ApplyAsegCore");
// If power-up 0x11 is owned: places the hit effect object at the point and plays its animation.
extern "C" void SpawnMurrayHitEffect(MURRAY *murray, qword q)
{
    if (!IsPowerupAcquired(0x11))
        return;
    SO *object = murray->psoHitEffect;
    if (!((((object->isInWorld) & 1) == 0)))
        return;
    object->TranslateToPos(q);
    ApplyAsegCore(murray->pasegHitEffect, murray->psoHitEffect, 0, 0, 0.0f, 1.0f);
}
} // namespace M_func_001B9D28

namespace M_func_001B9DB8 {
void ApplyAsegCore(ASEG *paseg, SO *pso, int c, int d, float f, float g) __asm__("ApplyAsegCore");
// Same as SpawnMurrayHitEffect without the power-up check (event 0x40000 from Murray).
extern "C" void SpawnMurrayHitEffectUnchecked(MURRAY *murray, qword pos)
{
    if (!((((int)murray->psoHitEffectB->isInWorld & 1) == 0)))
        return;
    murray->psoHitEffectB->TranslateToPos(pos);
    ApplyAsegCore(murray->pasegHitEffectB, murray->psoHitEffectB, 0, 0, 0.0f, 1.0f);
}
} // namespace M_func_001B9DB8

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
struct SNIP { int grfsnip; int oid; int ib; };   /* Sly 1's SnipLoObjects record */























   /* POLY */





LO *D_002F7900;   /* g_pmurray */

// ---- murray's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
SNIP D_002C0270[0x89] = {{0x300, 0x1A0, SLY2_OFFSETOF(MURRAY, gaitA.abl[0].animation)}, {0x300, 0x19E, SLY2_OFFSETOF(MURRAY, gaitA.abl[1].animation)}, {0x300, 0x1A2, SLY2_OFFSETOF(MURRAY, unknown1028)},
    {0x300, 0x24E, SLY2_OFFSETOF(MURRAY, jtair.apasegJump[0])}, {0x300, 0x1A1, SLY2_OFFSETOF(MURRAY, pasegFlop)}, {0x300, 0x1A8, SLY2_OFFSETOF(MURRAY, apasegAttack[0])}, {0x300, 0x1A9, SLY2_OFFSETOF(MURRAY, apasegAttack[1])},
    {0x300, 0x1B3, SLY2_OFFSETOF(MURRAY, apasegAttack[2])}, {0x300, 0x1B2, SLY2_OFFSETOF(MURRAY, apasegAttack[3])}, {0x300, 0xC4, SLY2_OFFSETOF(MURRAY, apasegAttack[4])}, {0x300, 0x1B8, SLY2_OFFSETOF(MURRAY, pasegPowerA)},
    {0x300, 0x1B9, SLY2_OFFSETOF(MURRAY, pasegPowerB)}, {0x300, 0x1BA, SLY2_OFFSETOF(MURRAY, arcLeap.paseg)}, {0x300, 0x1BB, SLY2_OFFSETOF(MURRAY, pasegPowerEnd)}, {0x300, 0x1AF, SLY2_OFFSETOF(MURRAY, gaitD.abl[0].animation)},
    {0x300, 0x1AE, SLY2_OFFSETOF(MURRAY, gaitD.abl[1].animation)}, {0x300, 0x206, SLY2_OFFSETOF(MURRAY, psmStrafeIdle)}, {0x300, 0x1C4, SLY2_OFFSETOF(MURRAY, apasegLift[0])}, {0x300, 0x1C5, SLY2_OFFSETOF(MURRAY, apasegLift[1])},
    {0x300, 0x1E2, SLY2_OFFSETOF(MURRAY, gaitB.abl[0].animation)}, {0x300, 0x1E3, SLY2_OFFSETOF(MURRAY, gaitB.abl[1].animation)}, {0x300, 0x1E4, SLY2_OFFSETOF(MURRAY, psmCarryStand)}, {0x300, 0x1E5, SLY2_OFFSETOF(MURRAY, unknown1514[0])},
    {0x300, 0x1E6, SLY2_OFFSETOF(MURRAY, unknown1514[1])}, {0x300, 0x1E7, SLY2_OFFSETOF(MURRAY, pasegCarryRun)}, {0x300, 0x1EA, SLY2_OFFSETOF(MURRAY, pasegThrow)}, {0x300, 0x1EC, SLY2_OFFSETOF(MURRAY, pasegToss)},
    {0x300, 0x1A6, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[0])}, {0x300, 0x1A7, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[8])}, {0x300, 0x1BC, SLY2_OFFSETOF(MURRAY, unknown16F0[0].animation)}, {0x300, 0x1BD, SLY2_OFFSETOF(MURRAY, unknown16F0[1].animation)},
    {0x300, 0x1BE, SLY2_OFFSETOF(MURRAY, unknown16F0[2].animation)}, {0x300, 0x1BF, SLY2_OFFSETOF(MURRAY, unknown16F0[3].animation)}, {0x300, 0x1C0, SLY2_OFFSETOF(MURRAY, unknown16F0[4].animation)}, {0x300, 0x1C1, SLY2_OFFSETOF(MURRAY, unknown16F0[5].animation)},
    {0x300, 0x1C2, SLY2_OFFSETOF(MURRAY, unknown16F0[6].animation)}, {0x300, 0x1C3, SLY2_OFFSETOF(MURRAY, unknown16F0[7].animation)}, {0x300, 0x1E9, SLY2_OFFSETOF(MURRAY, gaitC.abl[0].animation)}, {0x300, 0x1E8, SLY2_OFFSETOF(MURRAY, gaitC.abl[1].animation)},
    {0x300, 0x1C9, SLY2_OFFSETOF(MURRAY, psmStruggle)}, {0x300, 0x1D1, SLY2_OFFSETOF(MURRAY, jtthrowAim.abl[0].animation)}, {0x300, 0x1D2, SLY2_OFFSETOF(MURRAY, jtthrowAim.abl[1].animation)}, {0x300, 0x1D3, SLY2_OFFSETOF(MURRAY, jtthrowAim.pasegHoldTurnA)},
    {0x300, 0x1D4, SLY2_OFFSETOF(MURRAY, jtthrowAim.pasegHoldTurnB)}, {0x300, 0x1D5, SLY2_OFFSETOF(MURRAY, jtthrowAim.unknown64)}, {0x300, 0x1D6, SLY2_OFFSETOF(MURRAY, jtthrowAim.ablB[0].animation)}, {0x300, 0x1D7, SLY2_OFFSETOF(MURRAY, jtthrowAim.ablB[1].animation)},
    {0x300, 0x1D8, SLY2_OFFSETOF(MURRAY, jtthrowAim.ablB[2].animation)}, {0x300, 0x1D9, SLY2_OFFSETOF(MURRAY, jtthrowAim.ablB[3].animation)}, {0x300, 0x1DA, SLY2_OFFSETOF(MURRAY, jtthrowAim.pasegAimTurnA)}, {0x300, 0x1DB, SLY2_OFFSETOF(MURRAY, jtthrowAim.pasegAimTurnB)},
    {0x300, 0x1DC, SLY2_OFFSETOF(MURRAY, jtthrowAim.ablC[0].animation)}, {0x300, 0x1DD, SLY2_OFFSETOF(MURRAY, jtthrowAim.ablC[1].animation)}, {0x300, 0x1E1, SLY2_OFFSETOF(MURRAY, jtthrowAim.ploHoldParent)}, {0x300, 0x24A, SLY2_OFFSETOF(MURRAY, jtthrowAim.ploMarker)},
    {0x320, 0x25D, SLY2_OFFSETOF(MURRAY, jtair.precAir)}, {0x320, 0x25E, SLY2_OFFSETOF(MURRAY, jtair.precAirWallRun)}, {0x300, 0x24E, SLY2_OFFSETOF(MURRAY, jtair.apasegJump[0])}, {0x300, 0x24F, SLY2_OFFSETOF(MURRAY, jtair.apasegJump[3])},
    {0x300, 0x250, SLY2_OFFSETOF(MURRAY, jtair.apasegJump[4])}, {0x300, 0x251, SLY2_OFFSETOF(MURRAY, jtair.apasegJump[2])}, {0x300, 0x252, SLY2_OFFSETOF(MURRAY, jtair.apasegJump[1])}, {0x300, 0x253, SLY2_OFFSETOF(MURRAY, jtair.apasegDoubleJump[0])},
    {0x300, 0x254, SLY2_OFFSETOF(MURRAY, jtair.apasegDoubleJump[3])}, {0x300, 0x255, SLY2_OFFSETOF(MURRAY, jtair.apasegDoubleJump[4])}, {0x300, 0x256, SLY2_OFFSETOF(MURRAY, jtair.apasegDoubleJump[2])}, {0x300, 0x257, SLY2_OFFSETOF(MURRAY, jtair.apasegDoubleJump[1])},
    {0x300, 0x258, SLY2_OFFSETOF(MURRAY, jtair.pasegEdge)}, {0x320, 0x1F0, SLY2_OFFSETOF(MURRAY, aprecCarry[0])}, {0x320, 0x1F0, SLY2_OFFSETOF(MURRAY, aprecCarry[1])}, {0x320, 0x1F0, SLY2_OFFSETOF(MURRAY, aprecCarry[2])},
    {0x320, 0x1EF, SLY2_OFFSETOF(MURRAY, aprecCarry[3])}, {0x320, 0x1EE, SLY2_OFFSETOF(MURRAY, aprecState[7])}, {0x320, 0x1EE, SLY2_OFFSETOF(MURRAY, aprecState[8])}, {0x320, 0x1F1, SLY2_OFFSETOF(MURRAY, aprecState[3])},
    {0x300, 0x259, SLY2_OFFSETOF(MURRAY, jtair.pasegKindE)}, {0x300, 0x25A, SLY2_OFFSETOF(MURRAY, jtair.pasegKindEDouble)}, {0x320, 0x284, SLY2_OFFSETOF(MURRAY, jtgrab.unknown54)}, {0x320, 0x285, SLY2_OFFSETOF(MURRAY, jtgrab.unknown58)},
    {0x320, 0x286, SLY2_OFFSETOF(MURRAY, jtgrab.unknown5C)}, {0x320, 0x287, SLY2_OFFSETOF(MURRAY, jtgrab.unknown60)}, {0x300, 0x266, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[0].animation)}, {0x300, 0x262, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[1].animation)},
    {0x300, 0x268, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[2].animation)}, {0x300, 0x264, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[3].animation)}, {0x300, 0x269, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[4].animation)}, {0x300, 0x265, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[5].animation)},
    {0x300, 0x267, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[6].animation)}, {0x300, 0x263, SLY2_OFFSETOF(MURRAY, jtgrab.abldirA[7].animation)}, {0x300, 0x25F, SLY2_OFFSETOF(MURRAY, jtgrab.unknown174[0])}, {0x300, 0x260, SLY2_OFFSETOF(MURRAY, jtgrab.unknown174[1])},
    {0x300, 0x261, SLY2_OFFSETOF(MURRAY, jtgrab.unknown174[2])}, {0x300, 0x27D, SLY2_OFFSETOF(MURRAY, jtgrab.unknown510[0])}, {0x300, 0x25B, SLY2_OFFSETOF(MURRAY, jtgrab.unknown4F0[0])}, {0x300, 0x25C, SLY2_OFFSETOF(MURRAY, jtgrab.unknown4F0[1])},
    {0x300, 0x623, SLY2_OFFSETOF(MURRAY, jtgrab.pproxyMarker)}, {0x320, 0x297, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[0])}, {0x320, 0x298, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[1])}, {0x320, 0x299, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[2])},
    {0x320, 0x29A, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[3])}, {0x320, 0x29B, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[4])}, {0x320, 0x29C, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[5])}, {0x320, 0x29D, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[6])},
    {0x320, 0x29E, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[7])}, {0x320, 0x29F, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[8])}, {0x320, 0x2A0, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[9])}, {0x320, 0x2A1, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[10])},
    {0x320, 0x2A2, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[11])}, {0x320, 0x2A3, SLY2_OFFSETOF(MURRAY, jthurt.aprecHurt[12])}, {0x300, 0x289, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[0])}, {0x300, 0x28A, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[1])},
    {0x300, 0x28B, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[2])}, {0x300, 0x28C, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[3])}, {0x300, 0x28D, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[4])}, {0x300, 0x28E, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[5])},
    {0x300, 0x28F, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[6])}, {0x300, 0x290, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[7])}, {0x300, 0x291, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[8])}, {0x300, 0x292, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[9])},
    {0x300, 0x293, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[10])}, {0x300, 0x294, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[11])}, {0x300, 0x295, SLY2_OFFSETOF(MURRAY, jthurt.unknownA0[12])}, {0x300, 0xFC, SLY2_OFFSETOF(MURRAY, pexplDustLand)},
    {0x300, 0xFD, SLY2_OFFSETOF(MURRAY, pexplDustGrab)}, {0x100, 0x1FF, SLY2_OFFSETOF(MURRAY, psoCarryRoot)}, {0x300, 0x5E9, SLY2_OFFSETOF(MURRAY, jtmode.pasegStart)}, {0x300, 0x5EA, SLY2_OFFSETOF(MURRAY, jtmode.pasegEnd)},
    {0x300, 0x156, SLY2_OFFSETOF(MURRAY, pasegGadgetUse)}, {0x300, 0x1F6, SLY2_OFFSETOF(MURRAY, psoHitEffect)}, {0x300, 0x1F7, SLY2_OFFSETOF(MURRAY, pasegHitEffect)}, {0x300, 0x1F9, SLY2_OFFSETOF(MURRAY, psoHitEffectB)},
    {0x300, 0x1FA, SLY2_OFFSETOF(MURRAY, pasegHitEffectB)}, {0x300, 0x1F8, SLY2_OFFSETOF(MURRAY, pasegThrowPowered)}, {0x300, 0x1FB, SLY2_OFFSETOF(MURRAY, pasegTossGadget)}, {0x300, 0x1FC, SLY2_OFFSETOF(MURRAY, pasegFlopGadget)},
    {0x300, 0x1FD, SLY2_OFFSETOF(MURRAY, pasegTurnbuckle)}, {0x300, 0x1FE, SLY2_OFFSETOF(MURRAY, unknown1AF0)}};
float D_002C08DC = 0.15f;
float D_002C08E0 = 0.1f;
float D_002C08E4 = 5.0f;
VECTOR4 D_002C08F0 = {1.25f, 1.25f, 1.25f, 0.0f};
float D_002C0900 = 1.0f;
int D_002C0908[3][5] = {{463, -1, -1, -1, 464}, {461, -1, 463, -1, 464}, {460, 461, 462, 463, 464}};
int D_002C0948[3][5] = {{458, -1, -1, -1, 459}, {458, -1, 459, -1, 461}, {458, 459, 459, 460, 461}};
float D_002C0984 = 1.3f;
float D_002C0988 = 1.0f;
float D_002C0990[3] = {1.5f, 1.5f, 1.5f};
float D_002C099C = 4.0f;
float D_002C09A0 = 0.75f;
float D_002C09A4 = 2.5f;   /* gap object: no code reads it by this address */
float D_002C09A8 __attribute__((aligned(8))) = -500.0f;
float D_002C09AC = 300.0f;
LM D_002C09B0 = {100.0f, 500.0f};
float D_002C09B8 = 400.0f;
LM D_002C09C0 = {0.0f, 600.0f};
float D_002C09C8 = 0.15f;
VECTOR4 D_002C09D0 = {300.0f, 0.0f, -1000.0f, 0.0f};
VECTOR4 D_002C09E0 = {-400.0f, 0.0f, 600.0f, 0.0f};
float D_002C09F0 = 0.4f;
int D_002C09F8[5][2] = {{11, -1}, {17, -1}, {16, -1}, {21, -1}, {20, -1}};
float D_002C0A20[3] = {0.5f, 0.5f, 0.0f};
float D_002C0A30 __attribute__((aligned(16))) = 0.66f;
float D_002C0A34 = 0.15f;
float D_002C0A38 = -0.3f;
float D_002C0A3C = 4.75f;
float D_002C0A40 = 1.5f;
