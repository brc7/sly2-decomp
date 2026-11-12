// stepplayer.cpp -- the player base (STEPPLAYER : STEP, stepplayer.h): init and load, the per-frame update, hero
// camera overrides, damage and healing, gadgets and gadget slots, grab / hands / throw hand-offs, lean and idle,
// booty, mode queries, HP / MP. TU 0x13FF70..0x144638 (96 functions, address order).
// Entry points: InitStepplayer/PostLoadStepplayer, UpdateStepplayer (per frame), HandleStepplayerMessage,
// the gadget slots (UpdateStepplayerGadgets, SetStepplayerGadget), HP/MP, booty and the Splice-callable helpers.
// Slot overrides are named from their behaviour (GetStepplayerMovementNoise, AddStepplayerComponentXps...; "(inferred)" in their comments).
//
// Types: sly2/stepplayer.h's STEPPLAYER and its sub-records (jtgrab, jthurt, jtmode, jtthrow, agadgetslot...) for
// the m2c bodies, with real virtual calls (slots 87, 88, 99, 101); three unnamed words through macros (as
// stepbhv.cpp does). The slots GetHealthState, CanUseGadget and CanBeHealed are STEPPLAYER members here (symbols
// GetStepplayerHealthState__10STEPPLAYER etc.). The hand-written per-function files keep their views in namespaces named after each function (as
// jt.cpp does), their C++-named definitions as C-linkage definitions of their literal symbols there.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_stepplayer
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_stepplayer

#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/guard.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/cm.h"
#include "player_input.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/cref.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#include "sly2/save.h"
#include "sly2/classes.h"

// The embedded movement components of a player (JTAIR at +0x2C0, JTGRAB +0x4C0, ... JTTHROW +0x11B0) are taken from
// the real STEPPLAYER layout in sly2/stepplayer.h; the per-function views below only declare the words they read.
static inline STEPPLAYER *AsStepplayer(void *player) { return (STEPPLAYER *)player; }
// The real class behind the per-function object views below, for its virtual calls.
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }
#include "sly2/scrap.h"
#include "sly2/dli.h"
#include "sly2/joy.h"
#include "sly2/relay.h"

// 0x1444E0 StaticInit_stepplayer and GlobalConstructors_stepplayer: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* STEPPLAYER members some bodies reach through char * parameters: the jthurt record's animation, the lean lock and the
   booty object. */
#define PASEGAHURT(p) (((STEPPLAYER *)(p))->jthurt.unknown10C)
#define FLEAN1100(p) (((STEPPLAYER *)(p))->isLeanTargetLocked)
#define PBOOTY(p) (((STEPPLAYER *)(p))->pbooty)

extern "C" {
extern const float D_002A4D60;
int IsSwHandsOff(SW *psw);
STEPPLAYER *GetHeroPo(SW *psw);
void SetTnTnsOverride(TN *ptn, int f);
void AdjustStepXps(STEPPLAYER *pstepplayer);
void FreeJtWallRunXps(JTAIR *pjtair);
void BroadcastGameEvent(void *pgame, int nEvent, LO *plo);
int TakeJtDamage(JTHURT *pjthurt, ZPR *pzpr);
int GetJtHurtStatus(JTHURT *pjthurt);
void RemoveSoHierarchy(SO *pso);
void ReleaseAsega(ASEGA **ppasega);
void ChargeGadgetMp(STEPPLAYER *pstepplayer, int idGadget);
void SetJtGrabState(JTGRAB *pjtgrab, int nState, LO *plo, int n);
void SetJtAirJump(JTAIR *pjtair, int jtas, int n, LO *plo);
void AttachJtHandsObject(JTHANDS *pjthands, LO *plo);
void ReleaseJtHandsActs(JTHANDS *pjthands);
void SetJtHandsTargetAttached(JTHANDS *pjthands, LO *plo);
BHV *FindBhvInBhvstk(BHVSTK *pbhvstk, int n);
void UpdateStepplayerBootySave(STEPPLAYER *pstepplayer, int n0, int n1);
void SetBootyState(LO *pbooty, int nState);
void TranslateStepToPos(STEP *pstep, qword pos);
int CanPoBeHealed(PO *ppo);
void UnlockStepplayerLeanTarget(STEPPLAYER *pstepplayer);
int IsStepplayerInScriptState(STEPPLAYER *pstepplayer);
void SetStepplayerGrabTarget(STEPPLAYER *pstepplayer, LO *plo);
}
void ClearBhvstk(BHVSTK *pbhvstk);
/* g_agadgetdesc D_002BCD00 (defined in this file's data, below): one 0x20-byte record per power-up id (43): +0x0 the
   gadget slot state the gadget starts in, +0xC the MP cost the gadget slots check and ChargeGadgetMp charges. */
struct GADGETDESC { int x0; int x4; int x8; int mp; int x10; int ids0; int ids1; int x1C; };

namespace M_InitStepplayer {
void InitStep(STEPPLAYER *p) __asm__("InitStep");
void InitJtAir(JTAIR *pv, STEPPLAYER *p) __asm__("InitJtAir");
void InitJtGrab(JTGRAB *pv, STEPPLAYER *p) __asm__("InitJtGrab");
void InitJtPlaces(JTPLACES *pv, STEPPLAYER *p) __asm__("InitJtPlaces");
void InitJtHurt(JTHURT *pv, STEPPLAYER *p) __asm__("InitJtHurt");
void InitJtMode(JTMODE *pv, STEPPLAYER *p) __asm__("InitJtMode");
void InitThrow(JTTHROW *pv, STEPPLAYER *p) __asm__("InitThrow");
void InitJtPipe(JTPIPE *pv, STEPPLAYER *p) __asm__("InitJtPipe");
void InitJtgoal(JTGOAL *pv, STEPPLAYER *p) __asm__("InitJtgoal");
void SetSoZapResponse(STEPPLAYER *p, int mask, int n) __asm__("SetSoZapResponse");
// STEPPLAYER Init (vtable slot 7), shared by Sly, Bentley and Murray: STEP init, then the embedded components (air
// +0x2C0, grab +0x4C0, places +0x9E0, hurt +0xDF0, mode +0xF00, pipe +0xF50, throw ...) and the player defaults.
// Also known as: InitActorFields (upstream readable / Sly 1 name; InitActorFields__FP5ACTOR).
extern "C" void InitStepplayer(STEPPLAYER *player)
{
    InitStep(player);
    InitJtAir(&player->jtair, player);
    InitJtGrab(&player->jtgrab, player);
    InitJtPlaces(&player->jtplaces, player);
    InitJtHurt(&player->jthurt, player);
    InitJtMode(&player->jtmode, player);
    InitThrow(&player->jtthrow, player);
    InitJtPipe(&player->jtpipe, player);
    InitJtgoal(&player->jtgoal, player);


    player->grabRadius = 20.0f;
    player->unknown10F0 = 1;
    player->unknown1048 = 125.0f;
    player->renderAlpha = player->renderAlpha2 = 1.0f;
    player->agdtIdle[0] = 1.5f;
    player->agdtIdle[1] = 2.5f;
    player->agdtIdle[2] = 3.0f;
    player->agdtIdle[3] = 5.0f;
    player->unknown12F0 = player->unknown10F0;
    player->unknown1020 = 100.0f;
    player->maxGadgetPower = 100;
    player->lastGadgetUseTime = -60.0f;
    player->interactMode = -1;
    player->nUnk10e4 = -1;
    player->unknown1318 = -1;
    player->hideState = -1;

    SetSoZapResponse(player, 0x3FFFFFF, 0);
    SetSoZapResponse(player, 0x10FF00, 8);
}
} // namespace M_InitStepplayer

namespace M_PostLoadStepplayer {
struct K { int a; int b; };
extern const struct LONGU D_002A4D10 __asm__("D_002A4D10");
struct AREQ { int cb; int cbAlign; };
extern char D_002BC9F0[] __asm__("D_002BC9F0");
void PostStepLoad(STEPPLAYER *) __asm__("PostStepLoad");
void BindLoOidTable(STEPPLAYER *, int, void *) __asm__("BindLoOidTable");
void PostLoadJtAir(void *) __asm__("PostLoadJtAir");
void PostLoadJtGrab(void *) __asm__("PostLoadJtGrab");
void PostLoadJtPlaces(void *) __asm__("PostLoadJtPlaces");
void PostLoadJtHurt(void *) __asm__("PostLoadJtHurt");
void PostLoadJtMode(void *) __asm__("PostLoadJtMode");
void PostJtThrowLoad(void *) __asm__("PostJtThrowLoad");
void PostJtLoadPipe(void *) __asm__("PostJtLoadPipe");
int FindSwObjects(SW *, int, int, STEPPLAYER *, int, void **) __asm__("FindSwObjects");
void *AllocClearedFromWorldHeap(struct AREQ *) __asm__("AllocClearedFromWorldHeap");
void LoadStepplayerGadgets(STEPPLAYER *) __asm__("LoadStepplayerGadgets");
void BindStepplayerHpMp(STEPPLAYER *, int) __asm__("BindStepplayerHpMp");
void *EnsureTargetRecordById(STEPPLAYER *) __asm__("EnsureTargetRecordById");
void SetSoMass(STEPPLAYER *, float) __asm__("SetSoMass");
qword GetLoWorldPos(STEPPLAYER *) __asm__("GetLoWorldPos");
struct MATRIX3Q *GetLoWorldMatrix(STEPPLAYER *) __asm__("GetLoWorldMatrix");
void *func_002333B4(void *, int, int) __asm__("memset");
void PostSwCallback(SW *, STEPPLAYER *, long, int) __asm__("PostSwCallback");

// STEPPLAYER post-load (slot 15): STEP base, then each component's post-load, hp/mp binding and gadget slots.
// Also known as: PostLoadActorParts (upstream readable / Sly 1 name; PostLoadActorParts__FP5ACTOR).
extern "C" void PostLoadStepplayer(STEPPLAYER *player)
{
    void *apv[16];
    int c;
    int i;

    PostStepLoad(player);
    BindLoOidTable(player, 0x35, D_002BC9F0);
    PostLoadJtAir(&player->jtair);
    PostLoadJtGrab(&player->jtgrab);
    PostLoadJtPlaces(&player->jtplaces);
    PostLoadJtHurt(&player->jthurt);
    PostLoadJtMode(&player->jtmode);
    PostJtThrowLoad(&player->jtthrow);
    PostJtLoadPipe(&player->jtpipe);
    c = FindSwObjects(&g_sw, 0x401, 0x2AB, player, 0x10, apv);
    if (c) {
        struct AREQ req;
        struct AREQ *preq = &req;
        preq->cb = c * 8;
        player->clorefSplash = c;
        preq->cbAlign = 0x10;
        player->alorefSplash = (LOREF *)AllocClearedFromWorldHeap(preq);
        for (i = 0; i < c; i++) {
            LO *object;
            player->alorefSplash[i].plo = (LO *)apv[i];
            object = player->alorefSplash[i].plo;
            object->Remove();
        }
    }
    player->SetCtlState(0);
    player->SendPowerupEvent();
    LoadStepplayerGadgets(player);
    BindStepplayerHpMp(player, g_sw.transition.flags & 4);
    player->unknown1088 = EnsureTargetRecordById(player);
    SetSoMass(player, 0.25f);
    if (player->transforms->kind == 0) {
        qword q = GetLoWorldPos(player);
        struct MATRIX3Q m = *GetLoWorldMatrix(player);
        XF *so = player->transforms;
        func_002333B4(so, 0, 0x40);
        so->localMatrix.mat[0][0] = so->localMatrix.mat[1][1] = so->localMatrix.mat[2][2] = so->localMatrix.mat[3][3] = 1.0f;
        g_sw.transformStamp++;
        player->transforms->worldStamp = g_sw.transformStamp;
        player->UpdateXfWorldHierarchy();
        player->TranslateToPos(q);
        player->RotateToMat((MATRIX3 *)&m);
    }
    PostSwCallback(&g_sw, player, D_002A4D10.l, 0);
}
} // namespace M_PostLoadStepplayer

namespace M_func_00140338 {
int GetCurrentWorld(void) __asm__("GetCurrentWorld");
void RestartHudWidget1(void *p) __asm__("RestartHudWidget1");
int FindSwObjectsByClass(void *p, int a, int b, int c, int d, int e) __asm__("FindSwObjectsByClass");
void RestartHudWidget2(void *p) __asm__("RestartHudWidget2");
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
// On the hint button (mask 0x200) when allowed, dismisses/acknowledges the pending hint. (inferred)
// Also known as: ShowWorldStats (upstream readable / Sly 1 name; ShowWorldStats__Fv).
extern "C" void AcknowledgeStepplayerHint(void)
{
    if (!GetCurrentWorld())
        return;
    RestartHudWidget1(&g_sw);
    if (FindSwObjectsByClass(&g_sw, 0x605, 0x5F, 0, 0, 0)) RestartHudWidget2(&g_sw);
    SetSndSourceLo(CreateSfxSound(0x32), 0);
    g_sw.worldStatsShownTime = g_clock.time;
    g_cm.cplook.numActivations++;   /* the look policy's activation count (ActivateCplook/ActivateCpzoom also bump it) */
}
} // namespace M_func_00140338

namespace M_UpdateStepplayer {
extern char D_003DA0F0[] __asm__("D_003DA0F0");
void UpdateStep(STEPPLAYER *p) __asm__("UpdateStep");
void UpdateStepplayerGadgets(STEPPLAYER *p) __asm__("UpdateStepplayerGadgets");
void AcknowledgeStepplayerHint(STEPPLAYER *p) __asm__("AcknowledgeStepplayerHint");
void UpdateJtgoal(JTGOAL *pl) __asm__("UpdateJtgoal");
int IsJtgoalHolding(JTGOAL *pl) __asm__("IsJtgoalHolding");
void UpdateJsgJtControl(RELAYJSG *p, POCTL *q) __asm__("UpdateJsgJtControl");
void UpdateStepplayerInteractCandidates(STEPPLAYER *p) __asm__("UpdateStepplayerInteractCandidates");
void UpdateStepplayerWater(STEPPLAYER *p) __asm__("UpdateStepplayerWater");
void UpdateStepplayerHideBlend(STEPPLAYER *p) __asm__("UpdateStepplayerHideBlend");
void PickHeroInteractTarget(STEPPLAYER *p) __asm__("PickHeroInteractTarget");
int IsAsegaRetracted(ASEGA *x) __asm__("IsAsegaRetracted");
void DropAsegaRef(ASEGA **px) __asm__("DropAsegaRef");
int IsCameraPolicyActive__FP5CPLCY(void *p) __asm__("IsCameraPolicyActive__FP5CPLCY");
void SetJoyBtnHandled(JOY *pg, int n) __asm__("SetJoyBtnHandled");
void OpenGadgetMenuForHero(void *p) __asm__("OpenGadgetMenuForHero");
void UpdateBhvstk(BHVSTK *p) __asm__("UpdateBhvstk");
// STEPPLAYER Update (slot 16): STEP update, input and actions, the ride component, water, gadgets, booty, interact.
extern "C" void UpdateStepplayer(STEPPLAYER *player)
{
    JTGOAL *pl;
    UpdateStep(player);
    UpdateStepplayerGadgets(player);
    if (player->controller->ConsumeButtonsPressedWithin(BUTTON_HINT, 0.0f))
        AcknowledgeStepplayerHint(player);
    pl = &player->jtgoal;
    if (player->jthurt.state == -1) {
        UpdateJtgoal(pl);
        if (!IsJtgoalHolding(pl)) {
            if (player->behaviors.depth > 0) {
                ((BHV *)player->behaviors.behaviors[player->behaviors.depth - 1])->UpdateSteering((POCTL *)player->controller);
            } else if (player->pjsgScript) {
                UpdateJsgJtControl(player->pjsgScript, player->controller);
            }
        }
    }
    UpdateStepplayerInteractCandidates(player);
    UpdateStepplayerWater(player);
    UpdateStepplayerHideBlend(player);
    PickHeroInteractTarget(player);
    if (player->jthurt.unknown10C && IsAsegaRetracted(player->jthurt.unknown10C))
        DropAsegaRef(&player->jthurt.unknown10C);
    if (!IsCameraPolicyActive__FP5CPLCY((*(char (*)[])&g_cm.part3a0)) && player->controller->state == 1 && player->controller->GetButtonsPressedWithin(BUTTON_GADGET_MENU, 0.0f)) {

        JOY *pg = &g_joy;
        if ((pg->buttonsHeld & (BUTTON_GADGET_MENU | BUTTON_RUN)) == BUTTON_GADGET_MENU) {
            SetJoyBtnHandled(pg, BUTTON_GADGET_MENU);
            OpenGadgetMenuForHero(D_003DA0F0);
        }
    }
    if (!IsJtgoalHolding(pl))
        UpdateBhvstk(&player->behaviors);
}
} // namespace M_UpdateStepplayer

namespace M_UpdateStepplayerInteractCandidates {
extern float D_002A4D18 __asm__("D_002A4D18");
int GuardEvalInteractMode(STEPGUARD *pso, STEPPLAYER *p, float *pg) __asm__("GuardEvalInteractMode");
// Unless interaction is locked (+0x10E0), scans the guard list for interact candidates (pickpocket/takedown).
// Also known as: PickActorTargets (upstream readable / Sly 1 name; PickActorTargets__FP5ACTOR).
extern "C" void UpdateStepplayerInteractCandidates(STEPPLAYER *player)
{
    STEPGUARD *so, *pso0, *pso1;
    int n0, n1;
    float g0, g1;
    if (player->interactBlockFlags) return;
    g1 = D_002A4D18;
    pso1 = 0;
    pso0 = 0;
    n1 = -1;
    n0 = -1;
    g0 = g1;
    for (so = (STEPGUARD *)g_sw.dlStepguard.head; so; so = (STEPGUARD *)so->dleGuard.next) {
        float ag[2];
        int n = GuardEvalInteractMode(so, player, &ag[1]);
        float g = ag[1];
        if (g < g0) {
            g0 = g;
            n0 = n;
            pso0 = so;
        }
        if (n1 < n || (n == n1 && g < g1)) {
            g1 = g;
            n1 = n;
            pso1 = so;
        }
    }
    player->SetPlayerInteractMode(n0, pso0, n1, pso1);
}
} // namespace M_UpdateStepplayerInteractCandidates

namespace M_SetStepplayerPlayerInteractMode {
void ReleaseInteractTargetMark(STEPPLAYER *p, int a, LO *b, int c, LO *d) __asm__("ReleaseInteractTargetMark");
// Slot 95: stores the interact kind, target, mode and fight target.
// Also known as: SetActorTargets (upstream readable / Sly 1 name; SetActorTargets__FP5ACTORiiii).
extern "C" void SetStepplayerPlayerInteractMode(STEPPLAYER *player, int a, LO *b, int c, LO *d)
{
    if (b != player->interactTarget) ReleaseInteractTargetMark(player, a, b, c, d);
    player->interactTarget = b;
    player->nUnk10e4 = a;
    player->ploFightTarget = d;
    player->interactMode = c;
}
} // namespace M_SetStepplayerPlayerInteractMode

namespace M_SetStepplayerCtlState {
void SetPoCtlState(void *p, int n) __asm__("SetPoCtlState");
void SetSoTargetRecordParams(void *p, int a, int b) __asm__("SetSoTargetRecordParams");
void ClearBhvstk__FP6BHVSTK(void *p) __asm__("ClearBhvstk__FP6BHVSTK");
// Slot 79: PO control state, then the camera hookup; on becoming the current player refreshes the HUD/camera.
// Also known as: SetActorControl (upstream readable / Sly 1 name; SetActorControl__FP2LOi).
extern "C" void SetStepplayerCtlState(STEPPLAYER *player, int n)
{
    SetPoCtlState(player, n);
    SetSoTargetRecordParams(player, n == 1 ? 0 : -1, 0);
    if (n == 1) ClearBhvstk__FP6BHVSTK(&player->behaviors);
}
} // namespace M_SetStepplayerCtlState

namespace M_func_00140750 {
/* UpdateStepplayerHideBlend (Claude agent): matched. MinF/MaxF as `b < a ? b : a`, block-local dt per case, and the x10C8 state as `if (xDF4 != -1 || (g = x10CC) == 0) -1; else if (g == 1 || (x10C8 == 1 && 0 < g)) 1; else 0`. */
extern float D_002BCC6C __asm__("D_002BCC6C"), D_002BCC70 __asm__("D_002BCC70"), D_002BCC74 __asm__("D_002BCC74"), D_002BCC78 __asm__("D_002BCC78");
int TestStepplayerHidden(STEPPLAYER *p) __asm__("TestStepplayerHidden");
qword GetLoWorldPos(STEPPLAYER *p) __asm__("GetLoWorldPos");
void CreateNoise(void *a, qword pos, STEPPLAYER *p, int n) __asm__("CreateNoise");


// Larger of a and b.
extern "C" { static inline float MaxF(float a, float b) { return b > a ? b : a; } }

// If a hide noise source exists: smooths the hide blend toward 1 while hidden, 0 otherwise.
// Also known as: UpdateActorNoise (upstream readable / Sly 1 name; UpdateActorNoise__FP5ACTOR).
extern "C" void UpdateStepplayerHideBlend(STEPPLAYER *player)
{
    int k;
    float g;
    if (!player->hideNoise) return;
    switch (TestStepplayerHidden(player)) {
    case 1:
        { float dt = g_clock.deltaTime; player->hideBlend = StdMin(player->hideBlend + D_002BCC6C * dt, 1.0f); }
        break;
    case 0:
        { float dt = g_clock.deltaTime; player->hideBlend = MaxF(player->hideBlend + D_002BCC70 * dt, 0.0f); }
        break;
    case 2:
        player->hideBlend = 0.0f;
        break;
    }
    if (player->jthurt.state != -1 || (g = player->hideBlend) == 0.0f) player->hideState = -1;
    else if (g == 1.0f || (player->hideState == 1 && 0.0f < g)) player->hideState = 1;
    else player->hideState = 0;
    k = player->GetMovementNoise();
    if (player->hideState != 1 && k == 1) k = 2;
    switch (k) {
    case 0:
        player->noiseBuildup = 0.0f;
        break;
    case 1:
        { float dt = g_clock.deltaTime; player->noiseBuildup = Clamp(player->noiseBuildup + D_002BCC74 * dt, 0.0f, 1.0f); }
        break;
    case -1:
    case 2:
        { float dt = g_clock.deltaTime; player->noiseBuildup = Clamp(player->noiseBuildup + D_002BCC78 * dt, 0.0f, 1.0f); }
        break;
    }
    if (player->noiseBuildup == 1.0f) CreateNoise(player->hideNoise, GetLoWorldPos(player), player, 0);
}
} // namespace M_func_00140750

// Slot 98 base, movement-noise class (inferred): 0 (silent) while the world is hands-off, else -1 (no opinion: the subclass decides). UpdateStepplayerHideBlend resets its noise meter on 0 and fills it at one of two rates otherwise; a full meter makes noise.
extern "C" int GetStepplayerMovementNoise()
{
    return (IsSwHandsOff(&g_sw) != 0) ? 0 : -1;
}

namespace M_func_001409E8 {
struct X { char pad[0x104]; struct X *pxNext; };
int IsStepplayerInJtMode(STEPPLAYER *p) __asm__("IsStepplayerInJtMode");
qword GetLoWorldPos(STEPPLAYER *p) __asm__("GetLoWorldPos");
void *NewNoise(void *a, qword pos, STEPPLAYER *p) __asm__("NewNoise");
int StepguardHasSgf(STEPGUARD *pstepguard, int n) __asm__("StepguardHasSgf");
int CanListenerHearNoise(struct X *px, void *q) __asm__("CanListenerHearNoise");
void RemoveNoise(void *q) __asm__("RemoveNoise");
// 2 if in a look mode or no hide query is possible; else 1 if any guard can see the player, 0 otherwise. (inferred)
// Also known as: NActorPerceived (upstream readable / Sly 1 name; NActorPerceived__FP5ACTOR).
extern "C" int TestStepplayerHidden(STEPPLAYER *player)
{
    int unused[4];
    void *q;
    int r;
    STEPGUARD *so;
    struct X *px;
    if (IsStepplayerInJtMode(player)) return 2;
    q = 0;
    if (player->hideNoise) q = NewNoise(player->hideNoise, GetLoWorldPos(player), player);
    if (!q) return 2;
    r = 0;
    so = (STEPGUARD *)g_sw.dlStepguard.head;
    for (;;) {
        if (!so) break;
        if (StepguardHasSgf(so, 0x1D) || StepguardHasSgf(so, 0xB)) { r = 2; break; }
        if (so->CanSenseStimulus((CID147 *)q)) { r = 1; break; }
        so = (STEPGUARD *)so->dleGuard.next;
    }
    if (r == 0) {
        for (px = (*(struct X * *)&g_sw.dlCid47); px; px = px->pxNext)
            if (CanListenerHearNoise(px, q)) { r = 1; break; }
    }
    RemoveNoise(q);
    return r;
}
} // namespace M_func_001409E8

namespace M_FStepplayerNearInteractTarget {
extern float D_002BCC7C __asm__("D_002BCC7C"), D_002BCC80 __asm__("D_002BCC80"), D_002BCC84 __asm__("D_002BCC84"), D_002BCC88 __asm__("D_002BCC88"), D_002BCC8C __asm__("D_002BCC8C"), D_002BCC90 __asm__("D_002BCC90");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// True in interact mode 4 (kind != 3) when the target is within reach.
// Also known as: FActorTargetInReach (upstream readable / Sly 1 name; FActorTargetInReach__FP5ACTORi).
extern "C" int IsStepplayerNearInteractTarget(STEPPLAYER *player, int fX)
{
    float gDist, minValue, maxValue;
    qword a, b, t;
    QWROW d;
    float g;
    int f;
    if (player->interactMode != 4 || player->nUnk10e4 == 3) return 0;
    if (IsBasicDerivedFrom__FP5BASIC3CID(player->ploFightTarget, 0x3A)) gDist = D_002BCC7C;
    else gDist = D_002BCC80;
    minValue = D_002BCC84;
    maxValue = D_002BCC88;
    if (fX) {
        gDist += D_002BCC8C;
        minValue -= D_002BCC90;
        maxValue += D_002BCC90;
    }
    a = GetLoWorldPos(player->ploFightTarget);
    b = GetLoWorldPos(player);
    f = 0;
    {
        qword qwdqOut;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(qwdqOut) : "j"(a), "j"(b));
        d.q = qwdqOut;
    }
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(t) : "j"(d.q));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(t));
    if (g < gDist && minValue < d.f[2] && d.f[2] < maxValue) f = 1;
    return f;
}
} // namespace M_FStepplayerNearInteractTarget

namespace M_func_00140C30 {
void SetJtAirJump(JTAIR *p, int a, int b, int c) __asm__("SetJtAirJump");
void AddStepAxisXps(STEPPLAYER *p, void *a, LO *b, qword d) __asm__("AddStepAxisXps");
// Slot 82 (STEP constraint hook): if the other object left the world, resets the air component; then the base.
extern "C" void AddStepplayerAxisXps(STEPPLAYER *player, void *a, LO *b, qword v)
{
    qword d;
    if (((int)b->isInWorld & 1) == 0) {
        SetJtAirJump(&player->jtair, 0, 0, 0);
        return;
    }
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(v), "j"(player->unknown1010));
    AddStepAxisXps(player, a, b, d);
}
} // namespace M_func_00140C30

// If this is the current player, swaps the camera TN override (+0x1024).
// Also known as: SetActorZone (upstream readable / Sly 1 name; SetActorZone__FP5ACTORP2LO).
extern "C" void SetStepplayerTnOverride(STEPPLAYER *player, TN *tn)
{
    STEPPLAYER *pstepplayerHero = GetHeroPo(&g_sw);
    if (pstepplayerHero != player)
        return;
    if (pstepplayerHero->unknown1024 != 0) {
        SetTnTnsOverride(pstepplayerHero->unknown1024, 0);
    }
    pstepplayerHero->unknown1024 = tn;
    if (tn != 0) {
        SetTnTnsOverride(tn, 1);
    }
}

namespace M_func_00140D00 {
void IgnoreSoSlot59(void *p) __asm__("IgnoreSoSlot59");
void AddJtWallRunXps(void *p) __asm__("AddJtWallRunXps");
void UpdateJtInternalXpsHide(void *p) __asm__("UpdateJtInternalXpsHide");
void UpdateJtInternalXpsPipe(void *p) __asm__("UpdateJtInternalXpsPipe");
// Slot 59: SO base, then the active component's hook (air, grab, pipe). (inferred: adds component contacts)
// Also known as: RegisterActorContacts (upstream readable / Sly 1 name; RegisterActorContacts__FP3ALO).
extern "C" void AddStepplayerComponentXps(STEPPLAYER *player)
{
    IgnoreSoSlot59(player);
    if (player->jtair.state != -1) AddJtWallRunXps(&player->jtair);
    if (player->jtgrab.kind != -1) UpdateJtInternalXpsHide(&player->jtgrab);
    if (player->jtpipe.state != -1) UpdateJtInternalXpsPipe(&player->jtpipe);
}
} // namespace M_func_00140D00

// Slot 58: STEP base, then FreeJtWallRunXps when in the air. (inferred: frees component contacts)
// Also known as: FindActorFloor (upstream readable / Sly 1 name; FindActorFloor__FP3ALO).
void STEPPLAYER::AdjustXps()   /* key function: STEPPLAYER's vtable comes out here */
{
    STEPPLAYER *player = this;
    AdjustStepXps(player);
    if (player->jtair.state != -1) {
        FreeJtWallRunXps(&player->jtair);
    }
}

namespace M_func_00140DB0 {
/* HandleStepplayerContactImpulse (Claude agent for Lilly, T1644): saves s->v, scales it by 0.01 when the
   other entry's k (s->ae[1 - i].k) is one of three known values and p allows it
   (p->controller->state != 1 or IsStepplayerPushing(p)), calls CalculateSoContactResponse with the same arguments,
   then restores s->v.
   Matching notes: `save` is a plain qword that is only read by "j" asm operands, so it
   becomes a VU-class pseudo that is spilled across the call (the game's lq/sq v1 + lqc2
   vf3 from 0x10(sp)); the byte offset in its own variable makes the address
   (ae + off), not (off * 64 + ae). */
struct E { int a, b, k; char pad[0x34]; };
struct S { char pad[0x40]; qword v; struct E *ae; };
extern int D_002E1E40 __asm__("D_002E1E40");
extern int D_002DD5BC __asm__("D_002DD5BC");
extern int D_002F7900 __asm__("D_002F7900");
int IsStepplayerPushing(STEPPLAYER *p) __asm__("IsStepplayerPushing");
void CalculateSoContactResponse(STEPPLAYER *p, void *a, struct S *s, void *b, int i) __asm__("CalculateSoContactResponse");
// Slot 68 (contact impulse): special handling when the other body is one of the player characters.
// Also known as: FillActorDzResponse (upstream readable / Sly 1 name; FillActorDzResponse__FP3ALOP2DZP5DZENTPvi).
extern "C" void HandleStepplayerContactImpulse(STEPPLAYER *player, void *a, struct S *s, void *b, int i)
{
    QWROW c;
    qword save;
    int kk, off;
    save = s->v;
    off = (1 - i) * 64;
    kk = ((struct E *)((char *)s->ae + off))->k;
    if ((kk == D_002E1E40 || kk == D_002DD5BC || kk == D_002F7900) &&
        (player->controller->state != 1 || IsStepplayerPushing(player))) {
        qword vo;
        {
            qword qwcqOut;
            __asm__("qmtc2.ni %1, %0" : "=j"(qwcqOut) : "r"(0.01f));
            c.q = qwcqOut;
        }
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(vo) : "j"(save), "j"(c.q));
        s->v = vo;
    }
    CalculateSoContactResponse(player, a, s, b, i);
    s->v = save;
}
} // namespace M_func_00140DB0

// Slot 93: broadcasts game event 0x10 with the player.
extern "C" void OnStepplayerHeroDied(STEPPLAYER *player)
{
    BroadcastGameEvent(&g_game, 0x10, player);
}

namespace M_STEPPLAYER_FInvulnerable {
extern void *D_003D4A54 __asm__("D_003D4A54");
int IsPoCurrent(void *p) __asm__("IsPoCurrent");
int ShouldStepplayerIgnoreZap(void *p, int a) __asm__("ShouldStepplayerIgnoreZap");
// Slot 76 base: invulnerable while a look mode runs, in air state 0x15, or while the global invulnerability flag is set.
// Also known as: FActorImmune (upstream readable / Sly 1 name; FActorImmune__FP5ACTORi).
extern "C" int IsStepplayerInvulnerable(STEPPLAYER *player, int a)
{
    if (player->jtmode.kind == 1) return 1;
    if (player->jtair.state == 0x15) return 1;
    if (D_003D4A54 && IsPoCurrent(D_003D4A54)) return 1;
    return ShouldStepplayerIgnoreZap(&player->jthurt, a);
}
} // namespace M_STEPPLAYER_FInvulnerable

// Slot 75: forwards the damage to the hurt component (TakeJtDamage).
// Also known as: FActorAcceptContact (upstream readable / Sly 1 name; FActorAcceptContact__FP3ALO).
extern "C" void TakeStepplayerDamage(STEPPLAYER *player, ZPR *damage)
{
    TakeJtDamage(&player->jthurt, damage);
}

// Slot 77: the hurt component status (GetJtHurtStatus).
// Also known as: NActorDamageStatus (upstream readable / Sly 1 name; NActorDamageStatus__FP2LO).
int STEPPLAYER::GetHealthState()
{
    return GetJtHurtStatus(&jthurt);
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER {   /* a non-virtual pointer-to-member constant {delta 0, index -1, pfn} */
void p_func_00143BD8() __asm__("RestoreStepplayerBooty");
extern const unsigned int r_D_002A4D10[2] __asm__("D_002A4D10") = {0xFFFF0000, (unsigned int)p_func_00143BD8};
}
namespace RO_ORDER { extern const float r_D_002A4D18 __asm__("D_002A4D18") = 3.40282347e+38f; }
namespace M_HandleStepplayerMessage {
// HandleStepplayerMessage (0x140F90): message handler (p, msg, pv): msg 3 resets x1038 and stops the 0xDF0/0x11B0 actions, msg 9 (pso->p18 == p)
// switches on pmsg->k (0x15E: x190 = -v and virtual slot 35 called with the 0x1B0 rows times v; 0x161/0x162; 0x27B/0x27C; 0x1C7/0x1C8),
// msgs 4/5 call virtual slots 8/9 on the 0x1090 entries, msg 1 / msg 11 release 0x4D4 / 0x1310; then forwards to HandleBhvstkMessage/HandlePoMessage.
// MATCH (Claude agent for Lilly, T1682, 2026-10-25), no pins: from v6 (aligned 2), case 0x15E written with NO volatile asm and no vf6 pin.
// The -1.0f VU_FLOAT's home store (s.q = sv) goes last, so sv stays live in vf5 across the transform and the earlyclobber output lands
// in vf6 by itself; with the plain asms sched2 orders lh / lqc2 / lqc2 / addu / lw like the target.
/* g_pdliFirst: sly2/dli.h's declaration (one entity with the inline DLI helpers) */
#define F(T, p, off) (*(T *)((char *)(p) + (off)))
struct SO : BASIC { void *parent; void *root; void *p18; };
struct MSG { struct SO *pso; float g; int k; };
struct MSGB { void *p0; void *p4; };
struct ENT { void *po; int x4; };
extern JT *D_002E1E40 __asm__("D_002E1E40");   /* the hero JT */
extern float D_002BCC94 __asm__("D_002BCC94");
void RunJtHurtCallback(void *p) __asm__("RunJtHurtCallback");
void SetJtThrowState(void *p, int n) __asm__("SetJtThrowState");
qword GetStepAsegaTranslation(void *p, struct SO *pso, float g) __asm__("GetStepAsegaTranslation");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void ReleaseJtThrowObject(void *p) __asm__("ReleaseJtThrowObject");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
int IsTossableActive(void *p) __asm__("IsTossableActive");
void TriggerTossable(void *p) __asm__("TriggerTossable");
void ReleaseAsega(void *p) __asm__("ReleaseAsega");
void ChargeGadgetMp(void *p) __asm__("ChargeGadgetMp");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *pa, void *p, float g, int n, float h, void *pb) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void SetGuardOverride(void *p, int n) __asm__("SetGuardOverride");
void SetAllGuardsSpecialState(void *p, int n) __asm__("SetAllGuardsSpecialState");
void *AcquireProxyObject(void *p, int n) __asm__("AcquireProxyObject");
void SetLoParent(void *p, int a, int b) __asm__("SetLoParent");
void RemoveNoise(void *p) __asm__("RemoveNoise");
void StartJtHandsCarry(void *p, void *q) __asm__("StartJtHandsCarry");
void SetAsegaSpeed__FP5ASEGAf(void *p, float g) __asm__("SetAsegaSpeed__FP5ASEGAf");
void AttachJtHandsObject(void *p, void *q) __asm__("AttachJtHandsObject");
void UnsubscribeLoObject(void *p, void *q) __asm__("UnsubscribeLoObject");
void HandleBhvstkMessage(void *p, int msg, void *pmsg) __asm__("HandleBhvstkMessage");
void HandlePoMessage(void *p, int msg, void *pmsg) __asm__("HandlePoMessage");

// Return the current element of the iterator and advance it along the element's link field.
extern "C" { static inline void *NextDliItem(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
} }

// STEPPLAYER message handler (slot 5): message 3 (reset), animation events (message 9 kinds 0x15E/0x161 ...).
// Also known as: HandleActorMessage (upstream readable / Sly 1 name; HandleActorMessage__FP5ACTOR5MSGIDPv).
extern "C" void HandleStepplayerMessage(void *player, int msg, void *pv)
{
    switch (msg) {
    case 3:
        if (pv == F(void *, player, 0x1030) ||
            (F(void *, player, 0x1034) != 0 && pv == F(void *, F(void *, player, 0x1034), 0x30))) {
            F(int, player, 0x1038) = -1;
            int hurtState = AsStepplayer(player)->jthurt.state;
            if (hurtState >= 8 && hurtState <= 12) {   // hurt states with a pending callback (inferred)

                RunJtHurtCallback(&AsStepplayer(player)->jthurt);
            } else {
                switch (AsStepplayer(player)->jtthrow.state) {
                case 5:
                case 6: SetJtThrowState(&AsStepplayer(player)->jtthrow, 4); break;
                case 1:
                case 2: SetJtThrowState(&AsStepplayer(player)->jtthrow, 0); break;
                }
            }
        }
        break;
    case 9: {
        struct MSG *message = (struct MSG *)pv;
        if (message->pso->p18 != player)
            break;
        switch (message->k) {
        case 0x15E: {
            qword v = GetStepAsegaTranslation(player, message->pso, message->g);
            qword *pm = &F(qword, player, 0x1B0);
            union {
                qword q;
                float f[4];
            } s; /* home of the -1.0f VU_FLOAT: stored, never read */
            qword sv, r;
            VTE *vtable;
            void *pthis;
            void (*function)(void *, qword);
            __asm__("qmtc2.ni %1, %0" : "=j"(sv) : "r"(-1.0f));
            {
                qword qwFqwordp0x190Out;
                __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qwFqwordp0x190Out) : "j"(v), "j"(sv));
                F(qword, player, 0x190) = qwFqwordp0x190Out;
            }
            vtable = F(VTE *, player, 0xC);
            pthis = (char *)player + vtable[35].d;
            function = (void (*)(void *, qword))vtable[35].pfn;
            __asm__("vmulax.xyz ACC, %1, %2x\n\tvmadday.xyz ACC, %3, %2y\n\tvmaddz.xyz %0, %4, %2z"
                    : "=&j"(r)
                    : "j"(pm[0]), "j"(v), "j"(pm[1]), "j"(pm[2]));
            s.q = sv;
            function(pthis, r);
            break;
        }
        case 0x161: F(int, player, 0x1044) = 1; break;
        case 0x162: {
            qword v = F(qword, player, 0x1060);
            F(int, player, 0x1044) = 0;
            F(qword, player, 0x1070) = VuSub(v, GetLoWorldPos(player));
            F(int, player, 0x1078) = 0;
            break;
        }
        case 0x27B:
            if (AsStepplayer(player)->jtthrow.state != -1)
                ReleaseJtThrowObject(&AsStepplayer(player)->jtthrow);
            break;
        case 0x27C:
            if (AsStepplayer(player)->jtgrab.state == 0xB) {
                void *object;
                int n;
                if (AsStepplayer(player)->jtgrab.grabbedObject != 0 && IsBasicDerivedFrom__FP5BASIC3CID(AsStepplayer(player)->jtgrab.grabbedObject, 100)) {
                    object = AsStepplayer(player)->jtgrab.grabbedObject;
                    if (IsTossableActive(object))
                        TriggerTossable(object);
                }
                n = F(int, player, 0x9BC);
                if (F(void *, player, 0x130C) != 0) {
                    __asm__("" : : : "$3", "$4"); /* clobber of dead v1/a0 (Claude agent): target keeps n in a1 */
                    if (AsStepplayer(player)->idGadgetOverlay == n) {
                        ReleaseAsega(&AsStepplayer(player)->pasegaGadgetOverlay);
                        AsStepplayer(player)->idGadgetOverlay = -1;
                    } else {
                        ChargeGadgetMp(player);
                        ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(F(void *, player, 0x130C), player, 0.0f, 8, 1.0f,
                                                              &AsStepplayer(player)->pasegaGadgetOverlay);
                        AsStepplayer(player)->idGadgetOverlay = F(int, player, 0x9BC);
                    }
                } else if (n != -1) {
                    ChargeGadgetMp(player);
                    switch (F(int, player, 0x9BC)) {
                    case 0xE: SetGuardOverride(&g_sw, 1); break;
                    case 0x15: SetAllGuardsSpecialState(&g_sw, 0x15); break;
                    case 0x17: {
                        DLI dli;
                        void *po2;
                        void *pa = AcquireProxyObject(D_002E1E40->unknown2010, 0);
                        if (pa)
                            SetLoParent(pa, 0, 3);
                        InitDli(&dli, (DL *)&g_sw.dlStimulus);
                        while ((po2 = NextDliItem(&dli)) != 0) {
                            if (F(int, po2, 0x54) == 4 && F(void *, po2, 0x44) == player)
                                RemoveNoise(po2);
                        }
                        g_pdliFirst = dli.m_pdliNext;
                        g_sw.guardAlertResumeTime = g_clock.time + D_002BCC94;
                        break;
                    }
                    }
                }
                F(void *, player, 0x130C) = 0;
                F(int, player, 0x9BC) = -1;
            }
            break;
        case 0x1C8:
            if (AsStepplayer(player)->jtgrab.state == 0xC)
                StartJtHandsCarry(&AsStepplayer(player)->jtgrab.hands, AsStepplayer(player)->jtgrab.grabbedObject);
            break;
        case 0x1C7:
            if (AsStepplayer(player)->jtgrab.state == 0xC) {
                SetAsegaSpeed__FP5ASEGAf(F(void *, player, 0x1030), 1.0f);
                AttachJtHandsObject(&AsStepplayer(player)->jtgrab.hands, AsStepplayer(player)->jtgrab.grabbedObject);
            }
            break;
        }
        break;
    }
    case 4: {
        struct MSGB *pm = (struct MSGB *)pv;
        if (pm->p4 == player) {
            int i;
            F(void *, player, 0x108C) = pm->p0;
            for (i = 0; i < F(int, player, 0x1094); i++) {
                void *object = F(struct ENT *, player, 0x1090)[i].po;
                AsLo(object)->Add();
                F(struct ENT *, player, 0x1090)[i].x4 = 0;
            }
        }
        break;
    }
    case 5: {
        struct MSGB *pm = (struct MSGB *)pv;
        if (pm->p4 == player && pm->p0 == F(void *, player, 0x108C)) {
            int i;
            F(void *, player, 0x108C) = 0;
            for (i = 0; i < F(int, player, 0x1094); i++) {
                void *object = F(struct ENT *, player, 0x1090)[i].po;
                AsLo(object)->Remove();
            }
        }
        break;
    }
    case 1:
        if (AsStepplayer(player)->jtgrab.state == 0xB && AsStepplayer(player)->jtgrab.grabbedObject == pv) {
            UnsubscribeLoObject(pv, player);
            AsStepplayer(player)->jtgrab.grabbedObject = 0;
        }
        break;
    case 11:
        if (((struct MSGB *)pv)->p4 == player) {
            ReleaseAsega(&AsStepplayer(player)->pasegaGadgetOverlay);
            AsStepplayer(player)->idGadgetOverlay = -1;
        }
        break;
    }
    HandleBhvstkMessage(&AsStepplayer(player)->behaviors, msg, pv);
    HandlePoMessage(player, msg, pv);
}
} // namespace M_HandleStepplayerMessage
#undef F

namespace M_GetStepplayerCmFocusPos {
void GetJtPipePosition(JTPIPE *pjtpipe, VECTOR4 *ppos) __asm__("GetJtPipePosition");
qword GetLoWorldPos(STEPPLAYER *p) __asm__("GetLoWorldPos");
// Slot 97: the camera focus point (the pipe hand position on a pipe, else the world position raised in z).
// Also known as: PosActorFocus (upstream readable / Sly 1 name; PosActorFocus__FP5ACTORP9ACTORPOSE).
extern "C" void GetStepplayerCameraFocusPosition(STEPPLAYER *player, VECTOR4 *v)
{
    float z;
    if (player->jtpipe.state != -1) {
        GetJtPipePosition(&player->jtpipe, v);
        return;
    }
    *(qword *)v = GetLoWorldPos(player);
    if (player->floorContact) {
        z = player->vecWorldBasePos.z + player->unknown1048;
    } else if (player->jtair.state == 0x14 || g_cm.nCut != 0 || (void *)g_cm.focus != (void *)player) {
        v->z += player->vecPivotOffset.z + player->unknown1048;
        return;
    } else {
        z = player->unknown104C;
    }
    v->z = z;
}
} // namespace M_GetStepplayerCmFocusPos

namespace M_GetStepplayerCpdefi {
/* VU0 length via asm; qmfc2 into an int then explicit mtc1 (gives the original GPR/FPR allocation). */
struct XP { char pad[0x10]; QWROW pos; char pad2[0x34 - 0x20]; float g34; };
struct JT {
    char pad[0xC]; void *vptr; char pad1[0x2C0 - 0x10]; char ax2C0[0x2DC - 0x2C0]; int x2DC;
    char pad2[0x434 - 0x2E0]; int x434; char pad3[0x4C0 - 0x438]; char ax4C0[0xC]; int x4CC;
    char pad4[0xDF0 - 0x4D0]; char axDF0[4]; int xDF4; char pad5[0x1044 - 0xDF8]; int x1044; int pad6;
    float x104C; float x1050; char pad7[0x1060 - 0x1054]; QWROW x1060; qword x1070;
};
extern float D_002BCC98 __asm__("D_002BCC98"), D_002BCC9C __asm__("D_002BCC9C"), D_002BCCA0 __asm__("D_002BCCA0"), D_002BCCA4 __asm__("D_002BCCA4"), D_002BCCA8 __asm__("D_002BCCA8"), D_002BCCAC __asm__("D_002BCCAC"), D_002BCCB0 __asm__("D_002BCCB0"), D_002BCCB4 __asm__("D_002BCCB4");
extern char D_002BCCB8[] __asm__("D_002BCCB8"), D_002BCCC8[] __asm__("D_002BCCC8"), D_002BCCD8[] __asm__("D_002BCCD8");
extern struct S_D_002BCCF0 { float a, b, c; } D_002BCCF0 __asm__("D_002BCCF0");
extern struct S_lmZeroOne { float lo, hi; } g_lmZeroOne __asm__("g_lmZeroOne");
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void GetSoCameraDefaults(struct JT *, struct XP *) __asm__("GetSoCameraDefaults");
qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
float SmoothValue__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *n) __asm__("SmoothValue__FfffP3SMPPf");
qword PosSmooth(qword a, qword b, float dt, void *pv, int n) __asm__("PosSmooth");
void AddJtAirCpdefiFlags(void *, struct XP *) __asm__("AddJtAirCpdefiFlags");
void SmoothJtCameraCurveTarget(void *, struct XP *) __asm__("SmoothJtCameraCurveTarget");
void AddJtHurtCpdefiFlags(void *, struct XP *) __asm__("AddJtHurtCpdefiFlags");

// Evaluate the quadratic a + b*x + c*x^2.
extern "C" { static __inline__ float EvaluatePoly(float x, float a, float b, float c) { return a + x * (b + x * c); } }

// STEPPLAYER camera definition (slot 50): SO base, then the camera focus height and flags.
// Also known as: GetActorCmTarget (upstream readable / Sly 1 name; GetActorCmTarget__FP5ACTORP9ACTORPOSE).
extern "C" void GetStepplayerCpdefi(struct JT *jt, struct XP *xp)
{
    GetSoCameraDefaults(jt, xp);
    if (jt->x1044) {
        xp->pos.q = jt->x1060.q;
        jt->x104C = jt->x1060.f[2];
    } else {
        QWROW v;
        float z, lo, hi, g;
        { qword q = GetLoWorldPos(jt); __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); }
        AsStepplayer(jt)->GetCameraFocusPos((VECTOR4 *)&v);
        if (jt->x434) {
            lo = D_002BCCA0;
            hi = D_002BCCA4;
        } else if (jt->x2DC == 0x16) {
            lo = D_002BCCB0;
            hi = D_002BCCB4;
        } else {
            lo = D_002BCCA8;
            hi = D_002BCCAC;
        }
        g = Clamp(v.f[2], z + lo, z + hi);
        jt->x104C = g;
        if (g_cm.nCut != 0 || (void *)g_cm.focus != (void *)jt) {
            xp->pos.f[2] = g;
        } else {
            void *pv = jt->x434 ? D_002BCCC8 : D_002BCCB8;
            xp->pos.f[2] = SmoothValue__FfffP3SMPPf(jt->x1050, jt->x104C, g_clock.deltaTime, pv, 0);
            xp->pos.f[2] = Clamp(xp->pos.f[2], z + D_002BCC98, z + D_002BCC9C);
        }
        xp->pos.f[0] = v.f[0];
        xp->pos.f[1] = v.f[1];
    }
    {
        qword vel = PosSmooth(jt->x1070, D_002A2DD0, g_clock.deltaTime, D_002BCCD8, 0);
        qword r;
        jt->x1070 = vel;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(xp->pos.q), "j"(vel));
        xp->pos.q = r;
        jt->x1060.q = r;
    }
    jt->x1050 = xp->pos.f[2];
    if (jt->x2DC != -1)
        AddJtAirCpdefiFlags(jt->ax2C0, xp);
    if (jt->x4CC != -1)
        SmoothJtCameraCurveTarget(jt->ax4C0, xp);
    if (jt->xDF4 != -1)
        AddJtHurtCpdefiFlags(jt->axDF0, xp);
    {
        register qword w __asm__("vf3") = AsStepplayer(jt)->GetAngularVelocity();
        register qword l1 __asm__("vf1");
        register qword r2 __asm__("vf2");
        float s, k;
        __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(r2), "=&j"(l1) : "j"(w));
        __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l1) : "j"(l1));
        { int t; __asm__("qmfc2.ni %0, %1" : "=r"(t) : "j"(l1)); __asm__("mtc1 %1, %0" : "=f"(s) : "r"(t)); }
        k = EvaluatePoly(s, D_002BCCF0.a, D_002BCCF0.b, D_002BCCF0.c);
        xp->g34 *= Clamp(k, g_lmZeroOne.lo, g_lmZeroOne.hi);
    }
}
} // namespace M_GetStepplayerCpdefi

/* STEPPLAYER RemoveHierarchy. */
// Slot 12: base, then stops two actions and clears the behaviour stack.
extern "C" void RemoveStepplayerHierarchy(STEPPLAYER *player)
{
    RemoveSoHierarchy(player);
    ReleaseAsega(&PASEGAHURT(player));
    ReleaseAsega(&player->jtmode.animationPlayback);
    ClearBhvstk(&player->behaviors);
}

namespace M_func_00141968 {
qword GetLoWorldPos(STEPPLAYER *p) __asm__("GetLoWorldPos");
qword CalculateBallisticLaunchVelocity(qword a, qword b, float g, qword c) __asm__("CalculateBallisticLaunchVelocity");
void SetJtAirJump(JTAIR *p, int a, int b, int c) __asm__("SetJtAirJump");
// Splice: computes the ballistic velocity from the player's position toward a target and applies it.
// Also known as: LaunchActorTo (upstream readable / Sly 1 name; LaunchActorTo__FP5ACTORG9VU_VECTORff).
extern "C" void LaunchStepplayer(STEPPLAYER *player, qword v, float g, float h)
{
    union { qword q; float f[4]; } pos;
    union { qword q; float f[4]; } r;
    pos.q = GetLoWorldPos(player);
    pos.f[2] += player->vecPivotOffset.z;
    r.q = CalculateBallisticLaunchVelocity(pos.q, v, g, *(qword *)&player->physics->gravity);
    player->SetVelocity(r.q);
    if (player->jthurt.state != -1)
        return;
    SetJtAirJump(&player->jtair, 8, 0, 0);
    player->jtair.unknown40 = 3;
    player->jtair.unknownA0 = r.q;
    player->jtair.vecUnknownA0.z = 0.0f;
    player->jtair.unknown34 = h;
    if (h == player->jtair.vecUnknownA0.z) {
        player->jtair.canDoubleJump = 0;
        player->jtair.unknown2C = 0;
    }
}
} // namespace M_func_00141968

namespace M_UpdateStepplayerGadgets {
extern int D_002C3258[] __asm__("D_002C3258");
extern GADGETDESC D_002BCD00[] __asm__("D_002BCD00");
int ConsumeButtonsPressedWithin__5POCTLUif(POCTL *p, int mask, float g) __asm__("ConsumeButtonsPressedWithin__5POCTLUif");
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
int GetStepplayerMp(STEPPLAYER *p) __asm__("GetStepplayerMp");
void SetGadgetSlotState(STEPPLAYER *p, GADGETSLOT *pe, int n) __asm__("SetGadgetSlotState");
// Per frame for the three gadget slots (buttons 4/1/2): starts, updates and stops held gadgets.
extern "C" void UpdateStepplayerGadgets(STEPPLAYER *player)
{
    int i;
    int *pmask;
    if (!player->controller) return;
    i = 0;
    pmask = D_002C3258;
    do {
        int mask = *pmask;
        GADGETSLOT *pe = &player->gadgetSlots[i];
        int id = pe->idGadget;
        switch (pe->state) {
        case 0:
            if (ConsumeButtonsPressedWithin__5POCTLUif(player->controller, mask, 0.15f)) {
                if (!player->CanUseGadget(id)) {
                    SetSndSourceLo(CreateSfxSound(0x2E), 0);
                } else {
                    int n = GetStepplayerMp(player);
                    if (n < D_002BCD00[id].mp) {
                        SetSndSourceLo(CreateSfxSound(0x2E), 0);
                        player->lastGadgetUseTime = g_clock.time;
                    } else {
                        SetGadgetSlotState(player, pe, D_002BCD00[id].x0);
                    }
                }
            }
            break;
        case 1:
            if (!(player->controller->buttonsHeld & mask) || !player->CanUseGadget(id))
                SetGadgetSlotState(player, pe, 0);
            else
                player->UpdateGadget(pe);
            break;
        }
        pmask++;
    } while (++i < 3);
}
} // namespace M_UpdateStepplayerGadgets

// Slot 99 default: no gadget is usable (returns 0).
int STEPPLAYER::CanUseGadget(int gadgetId)
{
    return 0;
}

// Changes a gadget slot's state: leaving 1 stops the gadget (slot 102); stamps the time.
// Also known as: SetAbilityState (upstream readable / Sly 1 name; SetAbilityState__FP5ACTORP6ABSLOTi).
extern "C" void SetGadgetSlotState(STEPPLAYER *player, GADGETSLOT *pslot, int state)
{
    if (pslot->state == state)
        return;
    if (pslot->state == 1) {
        player->StopGadget(pslot->idGadget);
    }
    pslot->state = state;
    pslot->stateStartTime = g_clock.time;
    switch (state) {
    case 2:
        player->StartGadget(pslot->idGadget);  /* slot 99 */
        pslot->state = 0;
        return;
    case 1:
        player->StartGadget(pslot->idGadget);
        break;
    }
}

namespace M_ChargeGadgetMp {
extern GADGETDESC D_002BCD00[] __asm__("D_002BCD00");
int GetStepplayerMp(STEPPLAYER *p) __asm__("GetStepplayerMp");
void SetStepplayerMp(STEPPLAYER *p, int a) __asm__("SetStepplayerMp");
// Subtracts the gadget's MP cost from the player's MP and stamps the use time.
extern "C" void ChargeGadgetMp(STEPPLAYER *p, int i)
{
    if (i != -1) SetStepplayerMp(p, GetStepplayerMp(p) - D_002BCD00[i].mp);
    p->lastGadgetUseTime = g_clock.time;
}
} // namespace M_ChargeGadgetMp

// Slot 100 base: gadgets 14/21/23 go through the grab component (state 0xB); others ...
extern "C" void StartStepplayerGadget(STEPPLAYER *player, int gadgetId)
{
    switch (gadgetId) {
    case 14:
    case 21:
    case 23:
        player->jtgrab.gadgetId = gadgetId;
        SetJtGrabState(&player->jtgrab, 0xB, 0, 0);
        return;
    case 12:
        ChargeGadgetMp(player, 0xC);
        SetJtAirJump(&player->jtair, 0x11, 0, 0);
        return;
    }
}

namespace M_UpdateStepplayerGadget {
extern float D_002BD260 __asm__("D_002BD260");
extern float D_002BD264 __asm__("D_002BD264");
void DrainStepplayerMp(void *p, float f) __asm__("DrainStepplayerMp");
// Slot 101: while a gadget is held, drains MP at its per-second rate.
extern "C" void UpdateStepplayerGadget(void *player, int *pm)
{
    switch (*pm) {
    case 0x1D:
        DrainStepplayerMp(player, D_002BD260);
        break;
    case 0x2A:
        DrainStepplayerMp(player, D_002BD264);
        break;
    }
}
} // namespace M_UpdateStepplayerGadget

// Slot 102 base: empty (called when a held gadget slot leaves state 1).
extern "C" void StopStepplayerGadget()
{
}

namespace M_func_00141E30 {
// Slot 104: sends Splice event 0x1A to itself (after a power-up purchase, see RefreshPlayersAfterPowerupChange).
extern "C" void SendStepplayerPowerupEvent(STEPPLAYER *player)
{
    player->HandleSpliceEvent(0x1A, 0, 0, 0, 0);
}
} // namespace M_func_00141E30

// Slot 105 base: empty; called after the gadget slot assignment changes or loads.
extern "C" void OnStepplayerGadgetsChanged()
{
}

namespace M_IGadgetSlotFromId {
// Index (0..2) of the gadget slot holding the id, else -1.
extern "C" int GetGadgetSlotFromId(STEPPLAYER *p, int id)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (id == p->gadgetSlots[i].idGadget) return i;
    }
    return -1;
}
} // namespace M_IGadgetSlotFromId

namespace M_GrfbtnGadget {
extern int D_002C3258[] __asm__("D_002C3258");
// Button mask of every slot holding the gadget id.
extern "C" int GetGadgetButtonMask(STEPPLAYER *p, int id)
{
    int i;
    int m = 0;
    for (i = 0; i < 3; i++) {
        if (id == p->gadgetSlots[i].idGadget) m |= D_002C3258[i];
    }
    return m;
}
} // namespace M_GrfbtnGadget

namespace M_LoadStepplayerGadgets {
extern char D_002A4D50[] __asm__("D_002A4D50");
extern char D_002A4D58[] __asm__("D_002A4D58");
int sprintf(char *, const char *, ...) __asm__("sprintf");
void GetSaveVarByName(CRefData *r, void *a, void *b, char *c, int d) __asm__("GetSaveVarByName");
void Decref__4CRef(CRefData *r) __asm__("Decref__4CRef");
// 0x141EF0: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("addiu $sp, $sp, 0x20"); }

// Reads the three gadget slots from the save variables (root.apukCur[i]), then slot 105.
extern "C" void LoadStepplayerGadgets(STEPPLAYER *player)
{
    int i;
    for (i = 0; i < 3; i++) {
        char ach[16];
        CRefData r;
        sprintf(ach, D_002A4D50, i);
        GetSaveVarByName(&r, player->GetSaveVarRoot(), D_002A4D58, ach, 0);
        player->gadgetSlots[i].idGadget = r.u.n;
        Decref__4CRef(&r);
    }
    player->OnGadgetsChanged();
}
} // namespace M_LoadStepplayerGadgets

namespace M_SetStepplayerGadget {
extern void Decref__4CRef(CRef *) __asm__("Decref__4CRef");


extern char D_002A4D50[] __asm__("D_002A4D50");
extern char D_002A4D58[] __asm__("D_002A4D58");
int sprintf(char *pch, const char *fmt, ...) __asm__("sprintf");

CRef SetSaveVarByName(const CRef &ref, void *o, const char *fmt, ...) __asm__("SetSaveVarByName");
// Assigns a gadget id to slot i: stops the old one, clears duplicates in other slots, writes the save variable.
// Also known as: SetActorAbilitySlot (upstream readable / Sly 1 name; SetActorAbilitySlot__FP5ACTORii).
extern "C" void SetStepplayerGadget(STEPPLAYER *player, unsigned int i, int id)
{
    if (!(i < 3))
        return;
    GADGETSLOT *pe = &player->gadgetSlots[i];
    SetGadgetSlotState(player, pe, -1);
    pe->idGadget = id;
    pe->state = 0;
    pe->stateStartTime = g_clock.time;
    for (int j = 0; j < 3; j++) {
        char ach[16];
        GADGETSLOT *pe2 = &player->gadgetSlots[j];
        if (j != i && pe2->idGadget == id) {
            SetGadgetSlotState(player, pe2, -1);
            pe2->idGadget = -1;
        }
        sprintf(ach, D_002A4D50, j);
        SetSaveVarByName(CRef(player->gadgetSlots[j].idGadget, 0x15), player->GetSaveVarRoot(), D_002A4D58, ach, 0);
    }
    player->OnGadgetsChanged();
}
} // namespace M_SetStepplayerGadget

// Splice: with an object enters grab state 5 on it; without, leaves state 5.
// Also known as: SetActorInteractObject (upstream readable / Sly 1 name; SetActorInteractObject__FP5ACTORi).
extern "C" void SetStepplayerGrabTarget(STEPPLAYER *player, LO *lo)
{
    if (lo != 0) {
        SetJtGrabState(&player->jtgrab, 5, lo, 0);
    } else if (player->jtgrab.state == 5) {
        player->SetPlayerState(0);
    }
}

// Splice: clears the grab, then sets/clears the hands' attached object (+0x900).
extern "C" void SetStepplayerGrabTargetEx(STEPPLAYER *player, LO *lo)
{
    SetStepplayerGrabTarget(player, 0);
    if (lo != 0) {
        AttachJtHandsObject(&player->jtgrab.hands, lo);
        return;
    }
    ReleaseJtHandsActs(&player->jtgrab.hands);
}

// Splice: grab state 5 with the object and the hands' attached target; else leaves state 5.
extern "C" void SetStepplayerGrabTargetAlt(STEPPLAYER *player, LO *lo)
{
    if (lo != 0) {
        SetJtGrabState(&player->jtgrab, 5, lo, 0);
        SetJtHandsTargetAttached(&player->jtgrab.hands, lo);
    } else if (player->jtgrab.state == 5) {
        player->SetPlayerState(0);
    }
}

namespace M_func_00142278 {
void FadeSoIn(void *p, float f) __asm__("FadeSoIn");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
void UpdateJtThrowArc(JTTHROW *p, int n) __asm__("UpdateJtThrowArc");
void SetJtThrowState(JTTHROW *p, int n) __asm__("SetJtThrowState");
// Splice: makes object a the carried object (+0x11C0) aimed at object b's position.
extern "C" void StartStepplayerThrowAt(STEPPLAYER *player, LO *a, void *b)
{
    FadeSoIn(a, 0.25f);
    player->jtthrow.ploCarried = a;
    player->jtthrow.holdPosition = GetLoWorldPos(b);
    UpdateJtThrowArc(&player->jtthrow, 0);
    player->SetPlayerState(7);
    SetJtThrowState(&player->jtthrow, 7);
}
} // namespace M_func_00142278

namespace M_func_00142310 {
// Splice: stores the throw parameters (four floats, a range and two vectors) in the throw controller.
extern "C" void SetStepplayerThrowParams(STEPPLAYER *player, LM *ps, qword q1, qword q2, float a, float b, float c, float d)
{
    player->jtthrow.holdTurnAngle = a;
    player->jtthrow.aimTurnAngle = b;
    player->jtthrow.lmAim = *ps;
    player->jtthrow.tPredict = c;
    player->jtthrow.aimSpeed = d;
    player->jtthrow.qwPosLaunchLocal = q1;
    player->jtthrow.qwWLaunchLocal = q2;
}
} // namespace M_func_00142310

// Splice: grab state 9 with the object, or leaves state 9.
extern "C" void SetStepplayerGrabState9(STEPPLAYER *player, LO *lo)
{
    if (lo != 0) {
        SetJtGrabState(&player->jtgrab, 9, lo, 0);
    } else if (player->jtgrab.state == 9) {
        player->SetPlayerState(0);
    }
}

namespace M_func_001423A0 {
struct BUF { char pad[0x30]; qword pos; };
/* The hand position actuator (JTHANDS::pactPos; act.h ACT) as read here: +0x1C its end time. */
struct X994 { char pad[0x1C]; float t; };
void LoadIdentityMatrix4(XF *a, int b, MATRIX4Q *pm, int c, float g) __asm__("LoadIdentityMatrix4");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
// Transform point v by the 4x4 matrix pm.
extern "C" { static __inline__ qword Mul(MATRIX4Q *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(o) : "j"(pm->r[0].q), "j"(pm->r[1].q), "j"(pm->r[2].q), "j"(pm->r[3].q), "j"(v));
    return o;
} }
// Distance between a and b as a float.
extern "C" { static __inline__ float DistanceBetween(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
} }
// Splice predicate: true unless in grab state 5 with a target farther than 5 units from its goal.
// Also known as: FActorPoseDone (upstream readable / Sly 1 name; FActorPoseDone__FP5ACTOR).
extern "C" int IsStepplayerGrabReady(STEPPLAYER *player)
{
    struct BUF buf;
    MATRIX4Q m;
    qword v;
    if (player->jtgrab.state != 5 || !player->jtgrab.grabbedObject) return 1;
    LoadIdentityMatrix4(((SO *)player->jtgrab.hands.ploHand)->GetLocalTransform((MATRIX4 *)&buf), 0, &m, 0, 0.0f);
    v = Mul(&m, buf.pos);
    if (5.0f < DistanceBetween(GetLoWorldPos(player->jtgrab.hands.ploHand), v)) return 0;
    if (player->jtgrab.hands.mode != 1) return 1;
    if (g_clock.time < ((X994 *)player->jtgrab.hands.pactPos)->t) return 0;
    return 1;
}
} // namespace M_func_001423A0

// Splice predicate: the throw controller is idle (state -1).
extern "C" int IsStepplayerNotThrowing(STEPPLAYER *player)
{
    return ~player->jtthrow.state == 0;
}

// Splice predicate: grab state != 9.
extern "C" int IsStepplayerNotInGrabState9(STEPPLAYER *player)
{
    return player->jtgrab.state != 9;
}

// Enters grab state 0xC with the given (or default) parameter.
// Also known as: StartPrizeInteract (upstream readable / Sly 1 name; StartPrizeInteract__FP5ACTORP4ASEGP2LOi).
extern "C" void StartStepplayerGrabStateC(STEPPLAYER *player, ASEG *aseg, LO *lo, int n)
{
    if (aseg == 0) {
        aseg = (ASEG *)player->jtgrab.unknown508;
    }
    *(ASEG **)&player->jtgrab.pasegPhase0xC = aseg;
    SetJtGrabState(&player->jtgrab, 0xC, lo, 0);
    player->unknown1318 = n;
}

// Splice predicate: grab state != 0xC.
extern "C" int IsStepplayerNotInGrabStateC(STEPPLAYER *player)
{
    return player->jtgrab.state != 0xC;
}

namespace M_StartStepplayerFollow {
BHV *PushStepBehavior(STEPPLAYER *p, int n) __asm__("PushStepBehavior");
// Splice op 446: clears the behaviour stack and, with a leader, starts following it.
// Runtime: the task t1_recon_brawl uses it to make Murray follow Sly.
// Also known as: SetActorFollow (upstream readable / Sly 1 name; SetActorFollow__FP5ACTORiG9VU_VECTOR).
extern "C" void StartStepplayerFollow(STEPPLAYER *player, PO *leader, qword q)
{
    BHVFOLLOW *o;
    ClearBhvstk(&player->behaviors);
    if (!leader)
        return;
    o = (BHVFOLLOW *)PushStepBehavior(player, 0xC2);
    o->leader = leader;
    o->followOffset = q;
    o->Start();
}
} // namespace M_StartStepplayerFollow

namespace M_RenderStepplayerSelf {
extern JT *D_002E1E40 __asm__("D_002E1E40");
extern float D_002BD268 __asm__("D_002BD268"), D_002BD26C __asm__("D_002BD26C");
extern char D_002BD270[] __asm__("D_002BD270");
void RefreshSoBoundsRecord(STEPPLAYER *p) __asm__("RefreshSoBoundsRecord");
float SmoothValue__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *n) __asm__("SmoothValue__FfffP3SMPPf");
void RenderSoGlobset(GLOBSET *p, int n, struct SOBOUNDS *m, float a, float b) __asm__("RenderSoGlobset");
// Slot 19: renders when visible to the current camera; smooths the transparency.
// Also known as: DrawActor (upstream readable / Sly 1 name; DrawActor__FP5ACTOR).
extern "C" void RenderStepplayerSelf(STEPPLAYER *player)
{
    float g0, g1;
    if (!(player->visibilityMask & (*(int *)&g_cm.grfvis))) return;
    RefreshSoBoundsRecord(player);
    if (player->jtmode.state == 1 && player->jtmode.kind != 2) return;
    if (player->jtgrab.state == 2) {
        g0 = D_002BD268;
        g1 = D_002BD26C;
    } else {
        g1 = 1.0f;
        g0 = g1;
    }
    player->renderAlpha = SmoothValue__FfffP3SMPPf(player->renderAlpha, g0, g_clock.deltaTime, D_002BD270, 0);
    player->renderAlpha2 = SmoothValue__FfffP3SMPPf(player->renderAlpha2, g1, g_clock.deltaTime, D_002BD270, 0);
    if (player == D_002E1E40) {
        ((JT *)player)->unknown19A0->opacity = player->renderAlpha;
        ((JT *)player)->unknown19A4->opacity = player->renderAlpha;
    }
    RenderSoGlobset(&player->model, 0, player->psobounds, player->opacity * player->renderAlpha, player->opacity * player->renderAlpha2);
}
} // namespace M_RenderStepplayerSelf

namespace M_func_001426E8 {
XF *FindSoXf(STEPPLAYER *p, int x, int n) __asm__("FindSoXf");
void InvertMatrix3CofactorP(MATRIX3Q *pmOut, void *pm) __asm__("InvertMatrix3Cofactor");
qword DecomposeRotationMatrixToEuler(qword *aqwRows) __asm__("DecomposeRotationMatrixToEuler");
ACT *NewPositionAct(STEPPLAYER *p, XF *pxf, int a, int b, int c, qword q) __asm__("NewPositionAct");
void DetachAct(ACT *pact) __asm__("DetachAct");
// Rotate v by the 3x3 matrix pm.
extern "C" { static __inline__ qword MulV(MATRIX3Q *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(o) : "j"(pm->r[0].q), "j"(pm->r[1].q), "j"(pm->r[2].q), "j"(v));
    return o;
} }
// Return the 3x3 matrix product *pa * *pb.
extern "C" { static __inline__ MATRIX3Q MulM3(MATRIX3Q *pa, MATRIX3Q *pb)
{
    MATRIX3Q m;
    m.r[0].q = MulV(pa, pb->r[0].q);
    m.r[1].q = MulV(pa, pb->r[1].q);
    m.r[2].q = MulV(pa, pb->r[2].q);
    return m;
} }
// Finds the start-marker child and places the player relative to it.
// Also known as: AttachActorLook (upstream readable / Sly 1 name; AttachActorLook__FP5ACTORi).
extern "C" void AttachStepplayerToStartMarker(STEPPLAYER *player, int x)
{
    XF *so = FindSoXf(player, x, 6);
    MATRIX3Q m, a;
    if (!so)
        return;
    InvertMatrix3CofactorP(&a, &player->rootTransform->worldMatrix);
    {
        MATRIX3Q t;
        MATRIX3Q *pb = (MATRIX3Q *)&so->worldMatrix;
        t.r[0].q = m.r[0].q = MulV(&a, pb->r[0].q);
        t.r[1].q = m.r[1].q = MulV(&a, pb->r[1].q);
        m.r[2].q = t.r[2].q = MulV(&a, pb->r[2].q);
        *(MATRIX3Q *)player->aqwStartMarker = t;
    }
    player->pactStartMarker = NewPositionAct(player, so, 3, 7, 2, DecomposeRotationMatrixToEuler(player->aqwStartMarker));
    DetachAct(player->pactStartMarker);
}
} // namespace M_func_001426E8

// Sets the lean target object (+0x10F8) and lean kind (+0x10FC); restarts the idle timer on a new target.
// Also known as: SetActorLookTarget (upstream readable / Sly 1 name; SetActorLookTarget__FP5ACTORiP2LO).
extern "C" void SetStepplayerLeanTarget(STEPPLAYER *player, int nLeanKind, LO *ploLean)
{
    if (!(ploLean != player->ploLean || nLeanKind != player->leanKind))
        return;
    player->leanKind = nLeanKind;
    player->ploLean = ploLean;
    if (ploLean == 0)
        return;
    if (player->tNextIdle != D_002A4D60) {
        player->idleKind = 1;
        UnlockStepplayerLeanTarget(player);
    }
}

namespace M_ForceStepplayerLeanTarget {
void SetStepplayerLeanTarget(void *p, int a, int b) __asm__("SetStepplayerLeanTarget");
// Splice: SetStepplayerLeanTarget, then locks it (+0x1100 = 1).
extern "C" void ForceStepplayerLeanTarget(char *player, int a, int b)
{
    SetStepplayerLeanTarget(player, a, b);
    FLEAN1100(player) = 1;
}
} // namespace M_ForceStepplayerLeanTarget

// Unlocks the lean target (+0x1100 = 0).
extern "C" void UnlockStepplayerLeanTarget(STEPPLAYER *player)
{
    FLEAN1100(player) = 0;
}

namespace RO_ORDER { extern const char r_D_002A4D50[] __asm__("D_002A4D50") = "%d"; }
namespace RO_ORDER { extern const char r_D_002A4D58[] __asm__("D_002A4D58") = "apukCur"; }
namespace RO_ORDER { extern const float r_D_002A4D60 __asm__("D_002A4D60") = 3.40282347e+38f; }
namespace M_func_00142868 {
// Sets an idle timer's kind and makes it never fire (FLT_MAX).
// Also known as: ActorSetLookFlag (upstream readable / Sly 1 name; ActorSetLookFlag__FP5ACTORi).
extern "C" void InitIdleTimer(STEPPLAYER *p, int a)
{
    p->idleKind = a;
    p->tNextIdle = 3.40282346638528859812e+38f;
}
} // namespace M_func_00142868

namespace M_func_00142880 {
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
// Next idle event = now + a random time in the chosen range.
extern "C" void ScheduleIdleTimer(STEPPLAYER *p)
{
    float t;
    if (p->idleKind) {
        t = g_clock.time;
        t += RandomFloatInRange__Fff(p->agdtIdle[0], p->agdtIdle[1]);
        p->tNextIdle = t;
    } else {
        t = g_clock.time;
        t += RandomFloatInRange__Fff(p->agdtIdle[2], p->agdtIdle[3]);
        p->tNextIdle = t;
    }
}
} // namespace M_func_00142880

namespace M_HeroInteractDistance {
// GetHeroInteractDistance: ported from Lilly's personal Sly 2 decomp (src/P2/stepplayer_raw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BD280 (D_002BD280), D_002BD290 (D_002BD290), D_002BD2A0 (D_002BD2A0), D_002BD2B0 (D_002BD2B0), g_lmZeroOne
// 0x001428D8 GetHeroInteractDistance  (T1414, our name)
// How far an object is from the hero as the interact pickers rank it: *psDist = the xy distance |hero - plo| plus a bias, *pdz = the height difference (hero - plo)
// plus a bias (both optional). Callers: GuardEvalInteractMode 0x208E70 (plo = a guard, *pploCur = &hero->interactTarget, *pdz is tested against the guard's height window;
// HeroPickInteractGuard minimises *psDist) and the interact-target pick 0x142AF8 (three calls, *pploCur = &hero->ploLean).
// The biases are CLQs (c0 + c1 d + c2 d^2) from 0x2BD280: (1) for the current target: xy D_002BD280(d) = -200 - 0.2 d, height D_002BD290(dz) = -0.2 dz (hysteresis: the
// target already chosen looks nearer). (2) while the hero is steered (pctl) and d > 0.0001: rAway = (stick . (hero - plo)) / d with the stick vector = SetVectorCylind
// (leftStickAngle, leftStickDeflection, 0) (|stick| = leftStickDeflection); xy += clamp01(D_002BD2A0(rAway) = 0.5 - 0.5 rAway) * D_002BD2B0(d) (= -500 - 0.5 d): pushing the stick toward the object (rAway
// = -leftStickDeflection) takes up to 500 + d/2 off its distance, away from it nothing, no stick half.
// NOTE as compiled the hysteresis test compares the HERO (a0) with the current target (`lw v0,0(s1); bnel s2,v0`, s2 = a0), not the object (a1): both callers pass a
// field of the hero (+0x10DC / +0x10F8) that never holds the hero himself, so bias (1) is never added by the game (it looks like a typo for plo in the original source).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
struct VU_VECTOR4
{
    qword qw;
};
qword PosLoWorld(LO *plo) __asm__("GetLoWorldPos");
// Evaluate the quadratic g0 + g1*x + g2*x^2.
static inline float EvaluateClq(CLQ *clq, float x)
{
    return clq->g0 + x * (clq->g1 + x * clq->g2);
}
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
extern CLQ D_002BD280 __asm__("D_002BD280");
extern CLQ D_002BD290 __asm__("D_002BD290");
extern CLQ D_002BD2A0 __asm__("D_002BD2A0");
extern CLQ D_002BD2B0 __asm__("D_002BD2B0");
extern LM g_lmZeroOne __asm__("g_lmZeroOne");
// Return a - b.
static inline VU_VECTOR4 SubPos(qword a, qword b)
{
    VU_VECTOR4 r;
    r.qw = VuSub(a, b);
    return r;
}
// XY length of v, in the x lane.
static inline qword LengthXYInX2(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %2\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v), "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
// x lane of qw as a float (keeps it in a VF register).
static inline float LaneXKeepVf(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw), "j"(qw));
    return x;
}
// Evaluate the quadratic and clamp it to the range.
static inline float EvaluateClqClamped(CLQ *clq, LM *range, float x)
{
    float g = clq->g0 + x * (clq->g1 + x * clq->g2);
    float minValue = range->minValue;
    float maxValue = range->maxValue;
    if (g < minValue)
        return minValue;
    if (maxValue < g)
        return maxValue;
    return g;
}
extern qword SetVectorCylind(float rad, float sXY, float sZ) __asm__("SetVectorCylind");
extern "C" void GetHeroInteractDistance(STEPPLAYER *pstepplayer, LO *plo, LO **pploCur, float *psDist, float *pdz) __asm__("GetHeroInteractDistance");
// Distance score of an object for interact picking: xy distance plus biases (hysteresis for the current target).
// Also known as: LookScore (upstream readable / Sly 1 name; LookScore__FP5ACTORP2LOPP2LOPfT3).
extern "C" void GetHeroInteractDistance(STEPPLAYER *player, LO *lo, LO **pploCur, float *psDist, float *pdz)
{
    float sBias = 0.0f;
    float zBias = 0.0f;
    qword posHero = PosLoWorld(player);
    VU_VECTOR4 vecS = SubPos(posHero, PosLoWorld(lo));
    qword vec = vecS.qw;
    float dz = QwordZ(vec);
    float sXY = VuToFloat(LengthXYInX2(vec));
    if ((LO *)player == *pploCur)
    {
        sBias = EvaluateClq(&D_002BD280, sXY);
        zBias = EvaluateClq(&D_002BD290, dz);
    }
    POCTL *pctl = player->controller;
    if (pctl != 0 && sXY > 0.0001f)
    {
        qword qwStick = SetVectorCylind(pctl->leftStickAngle, pctl->leftStickDeflection, 0.0f);
        float rAway = LaneXKeepVf(VuDot(qwStick, vec)) / sXY;
        sBias += EvaluateClqClamped(&D_002BD2A0, &g_lmZeroOne, rAway) * EvaluateClq(&D_002BD2B0, sXY);
    }
    sXY += sBias;
    dz += zBias;
    if (psDist != 0)
    {
        *psDist = sXY;
    }
    if (pdz != 0)
    {
        *pdz = dz;
    }
}
} // namespace M_HeroInteractDistance

namespace M_func_00142AF8 {
// PickHeroInteractTarget (Claude agent, laneC38 2026-10-24): MATCH from fresh5 (aligned 18). Every matrix product is written with Lilly's
// MATRIX3 operator* (the NRV helper from src/game/engine/anim/tail/UpdateChainLink.cpp / GetSoAngularVelocity.cpp: rows read into qword locals, each
// result row stored through a VU_VECTOR4 copy), and the first ConvertXfMat call is a plain assignment (no dead-copy wrapper).
// The one remaining trick is fresh3's commented clobber of the dead a0 in the c4678 block.
struct R { char pad[0x40]; MATRIX3Q m40; char pad1[0xA0 - 0x70]; void *pA0; };
struct Q { char pad[8]; R *p8; };
struct S {
    char pad[0xE0]; S *pNext; int pad1; float gE8; char rng[8]; int nF4;
};
struct P : LO {
    char pad[0x60 - 0x50]; void *p60; char pad1[0xF04 - 0x64]; int jtModeState; char pad2[0x10DC - 0xF08];
    S *ps10DC; int pad3; int interactKind; char pad4[0x10F4 - 0x10E8]; Q *pq; void *leanTarget; int pad5;
    int n1100; int n1104; int pad6[2]; float g1110; float g1114; float g1118; int pad7;
    LM lmPan; LM lmTilt; float g1130; float g1134; char pad8[0x1148 - 0x1138]; int n1148; float g114C;
    MATRIX3Q m1150;
};

extern int D_002DD640 __asm__("D_002DD640");
extern float D_002A4D68 __asm__("D_002A4D68");
extern qword D_002A2DA0 __asm__("D_002A2DA0");
void ScheduleIdleTimer(P *p) __asm__("ScheduleIdleTimer");
void SetStepplayerLeanTarget(P *p, int n, S *ps) __asm__("SetStepplayerLeanTarget");
void GetHeroInteractDistance(P *p, S *ps, void **ppv, float *pg, float *pg2) __asm__("GetHeroInteractDistance");
int IsWithinLimits__FP2LMf(void *p, float g) __asm__("IsWithinLimits__FP2LMf");
void AttachAct(Q *pq) __asm__("AttachAct");
void DetachAct(Q *pq) __asm__("DetachAct");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword ConvertPosBetweenLos(int n, P *p, qword q) __asm__("ConvertPosBetweenLos");
void CalculateVectorPanTilt(qword v, float *pa, float *pb) __asm__("CalculateVectorPanTilt");
float ClampAngleToRange(LM *plm, float g) __asm__("ClampAngleToRange");
MATRIX3Q LoadRotateMatrixPanTilt(float a, float b) __asm__("LoadRotateMatrixPanTilt");
MATRIX3Q BuildAxisAngleRotationMatrix(qword v, float g) __asm__("BuildAxisAngleRotationMatrix");
MATRIX3Q ConvertXfMat(void *pvFrom, void *pvTo, MATRIX3Q *pm) __asm__("ConvertXfMat");
MATRIX3Q InvertMatrix3Cofactor(const MATRIX3Q *pm) __asm__("InvertMatrix3Cofactor");
qword DecomposeRotationMatrixToEuler(MATRIX3Q *pm) __asm__("DecomposeRotationMatrixToEuler");
void SetActPositionGoal(Q *pq, qword q) __asm__("SetActPositionGoal");
void PickHeroInteractTarget(P *p) __asm__("PickHeroInteractTarget");

struct VU_VECTOR4 { qword qw; };
// 3x3 matrix product matL * matR.
inline MATRIX3Q operator*(const MATRIX3Q &matL, const MATRIX3Q &matR) return mat
{
    qword qwL0 = matL.r[0].q;
    qword qwL1 = matL.r[1].q;
    qword qwL2 = matL.r[2].q;
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)&matR.r[0].q)->qw));
    *(VU_VECTOR4 *)&mat.r[0].q = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)&matR.r[1].q)->qw));
    *(VU_VECTOR4 *)&mat.r[1].q = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)&matR.r[2].q)->qw));
    *(VU_VECTOR4 *)&mat.r[2].q = v2;
}
// Fraction of g between a and b; 0 when a == b.
static inline float InverseLerp(float g, float a, float b)
{
    if (b == a)
        return 0.5f;
    return (g - a) / (b - a);
}
// Float i of the matrix's first row.
static inline float X(const MATRIX3Q &m, int i) { return ((float *)&m.r[0].q)[i]; }

// Per-frame interact/lean target pick: ranks candidates with GetHeroInteractDistance.
// Also known as: UpdateActorLook (upstream readable / Sly 1 name; UpdateActorLook__FP5ACTOR).
void PickHeroInteractTarget(P *p)
{
    int f;
    float pan, tilt;
    float g;
    if (p->pq == 0)
        return;
    f = 0;
    if (p->lmPan.minValue < p->lmPan.maxValue || p->lmTilt.minValue < p->lmTilt.maxValue)
        f = 1;
    if (p->jtModeState != -1)
        f = 0;
    if (p->g114C < g_clock.time && (p->n1148 != 0 || f)) {
        p->n1148 = !p->n1148;
        ScheduleIdleTimer(p);
    }
    if (p->n1100 == 0) {
        if (D_002DD640 != 0) {
            SetStepplayerLeanTarget(p, -1, 0);
        } else {
            S *psBest = p->ps10DC;
            int nBest = p->interactKind;
            float bestScore, g1, g2;
            S *ps;
            if (psBest)
                GetHeroInteractDistance(p, psBest, &p->leanTarget, &bestScore, 0);
            else
                bestScore = D_002A4D68;
            for (ps = (*(S * *)&g_sw.dlCid21); ps; ps = ps->pNext) {
                if (ps->nF4 < nBest)
                    continue;
                GetHeroInteractDistance(p, ps, &p->leanTarget, &g1, &g2);
                if (g1 > ps->gE8)
                    continue;
                if (!IsWithinLimits__FP2LMf(ps->rng, g2))
                    continue;
                if (nBest < ps->nF4 || (ps->nF4 == nBest && g1 < bestScore)) {
                    psBest = ps;
                    bestScore = g1;
                    nBest = psBest->nF4;
                }
            }
            if (g_sw.cjtgrabcand > 0 && nBest < 3) {
                __asm__("" : : : "$4"); /* clobber of dead a0 (Claude agent): target keeps &g_sw in a1 */
                ps = (S *)g_sw.ajtgrabcand[0].unknown10;
                if (ps) {
                    float g3, g4;
                    GetHeroInteractDistance(p, ps, &p->leanTarget, &g3, &g4);
                    if (nBest < 2 || (nBest == 2 && g3 < bestScore)) {
                        psBest = ps;
                        nBest = 2;
                        bestScore = g3;
                    }
                }
            }
            SetStepplayerLeanTarget(p, nBest, psBest);
        }
    }
    if (p->n1104 != 0) {
        AttachAct(p->pq);
        pan = p->g1110;
        tilt = p->g1114;
        g = p->g1118;
    } else {
        if (p->leanTarget == 0 || p->n1148 == 0 || !f) {
            DetachAct(p->pq);
            return;
        }
        AttachAct(p->pq);
        CalculateVectorPanTilt(ConvertPosBetweenLos(0, p, GetLoWorldPos(p->leanTarget)), &pan, &tilt);
        pan = ClampAngleToRange(&p->lmPan, pan);
        tilt = ClampAngleToRange(&p->lmTilt, tilt);
        {
            float t;
            if (p->lmPan.maxValue == p->lmPan.minValue)
                t = 0.5f;
            else
                t = (pan - p->lmPan.minValue) / (p->lmPan.maxValue - p->lmPan.minValue);
            float u = t * __builtin_fabsf(t);
            g = (1.0f - u) * p->g1130 + u * p->g1134;
        }
    }
    MATRIX3Q m0, m1;
    m0 = LoadRotateMatrixPanTilt(pan, tilt) * BuildAxisAngleRotationMatrix(D_002A2DA0, g);
    m1 = ConvertXfMat(0, p->p60, &p->pq->p8->m40);
    m1 = m1 * InvertMatrix3Cofactor(&p->m1150);
    MATRIX3Q m2;
    if (X(m0, 0) + X(m1, 0) < 0.0f && X(m0, 1) * X(m1, 1) < 0.0f)
        m2 = p->m1150;
    else
        m2 = m0 * p->m1150;
    m2 = ConvertXfMat(p->p60, p->pq->p8->pA0, &m2);
    SetActPositionGoal(p->pq, DecomposeRotationMatrixToEuler(&m2));
}
} // namespace M_func_00142AF8

namespace M_func_00143090 {
// Copies three pairs of idle-timer ranges into the record.
extern "C" void SetStepplayerIdleParams(STEPPLAYER *player, LM *a, LM *b, LM *c)
{
    player->almIdle[0] = *a;
    player->almIdle[1] = *b;
    player->almIdle[2] = *c;
}
} // namespace M_func_00143090

namespace RO_ORDER { extern const float r_D_002A4D68 __asm__("D_002A4D68") = 3.40282347e+38f; }
namespace M_func_001430C8 {
extern float D_002BD2C0 __asm__("D_002BD2C0");
extern float D_002BD2C4 __asm__("D_002BD2C4");
qword GetLoWorldPos(void *p) __asm__("GetLoWorldPos");
qword ConvertPosBetweenLos(void *pm, STEPPLAYER *p, qword v) __asm__("ConvertPosBetweenLos");
float atan2f(float y, float x) __asm__("atan2f");
// Lean flags from the lean target's direction (quadrant in local space) and the lean kind.
// Also known as: GrfActorLookMask (upstream readable / Sly 1 name; GrfActorLookMask__FP5ACTOR).
extern "C" int GetStepplayerLeanFlags(STEPPLAYER *player)
{
    int n;
    int r;
    if (player->ploLean) {
        qword v = ConvertPosBetweenLos(0, player, GetLoWorldPos(player->ploLean));
        float g = atan2f(QwordY(v), QwordX(v));
        float a = __builtin_fabsf(g);
        if (a < D_002BD2C0) n = 0;
        else if (D_002BD2C4 < a) n = 3;
        else if (0.0f < g) n = 1;
        else n = 2;
    }
    r = 8;
    switch (player->leanKind) {
    case -1: case 0: break;
    case 1: r = 0x10 << n; break;
    case 2: r = 0x100 << n; break;
    case 3: r = 0x1000; break;
    case 4: r = 0x100; break;
    }
    return r;
}
} // namespace M_func_001430C8

/* Clears the interaction target's behaviour 0xB1 hold on this player. */
// If the interact target's takedown behaviour (0xB1) points at the player, clears its mark.
// Also known as: StopNearestLaunchTracking (upstream readable / Sly 1 name; StopNearestLaunchTracking__FP5ACTOR).
extern "C" void ReleaseInteractTargetMark(STEPPLAYER *player)
{
    BHV *pbhv;
    if (player->interactTarget == 0)
        return;
    pbhv = FindBhvInBhvstk((BHVSTK *)((char *)player->interactTarget + 0x26C), 0xB1);
    if (pbhv != 0 && *(STEPPLAYER **)((char *)pbhv + 0x5C) == player) {
        *(int *)((char *)pbhv + 0x60) = 0;
    }
}

namespace M_GetStepplayerStunHold {
extern qword g_normalZ __asm__("g_normalZ");
qword GetLoWorldPos(LO *p) __asm__("GetLoWorldPos");
// Slot 96: velocity without its vertical part gives the direction; target = position + direction * dt; radii 150/75.
// Also known as: GetSlyLaunchTarget (upstream readable / Sly 1 name; GetSlyLaunchTarget__FP3SLYPvf).
extern "C" void GetStepplayerStunHold(STEPPLAYER *player, STUNHOLD *ps, float g)
{
    qword v, h, pos, s, r;
    QWROW u;
    v = player->GetVelocity();
    {
        qword d, one, up = g_normalZ;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(v), "j"(up));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(d) : "j"(up), "j"(d));
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(h) : "j"(v), "j"(d));
    }
    pos = GetLoWorldPos(player);
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
    __asm__("vadda.xyzw ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos), "j"(h), "j"(s));
    ps->qwVel = h;
    ps->gSide = 150.0f;
    ps->gUp = 75.0f;
    ps->qwPos = r;
    u.q = s;
    ps->fValid = 0;
}
} // namespace M_GetStepplayerStunHold

namespace M_func_001432E0 {
int GetCurrentBootyEntryId(void *p) __asm__("GetCurrentBootyEntryId");
void DropStepplayerBooty(void *p) __asm__("DropStepplayerBooty");
void *AcquireProxyObject(void *p, int n) __asm__("AcquireProxyObject");
void SelectBootyEntry(void *p, int n) __asm__("SelectBootyEntry");
void SetBootyState(void *p, int n) __asm__("SetBootyState");
// If carrying booty not shown yet: releases the old marker and clones the template.
// Also known as: GiveActorBooty (upstream readable / Sly 1 name; GiveActorBooty__FP5ACTORi).
extern "C" void SpawnStepplayerBootyMarker(char *player, int n)
{
    void *q = PBOOTY(player);
    char *o;
    if (q && GetCurrentBootyEntryId(q) == n) return;
    DropStepplayerBooty(player);
    if (n == -1) return;
    if (!(*(void * *)&g_sw.bootyProxySource)) return;
    o = (__typeof__(o))AcquireProxyObject((*(void * *)&g_sw.bootyProxySource), 1);
    SelectBootyEntry(o, n);
    *(void **)(o + 0x160) = player;
    SetBootyState(o, 6);
    SetBootyState(o, 4);
}
} // namespace M_func_001432E0

// If carrying booty: booty save case 2 (dropped), its state 5.
// Also known as: DropActorBooty (upstream readable / Sly 1 name; DropActorBooty__FP5ACTORii).
extern "C" void DropStepplayerBooty(STEPPLAYER *player)
{
    if (PBOOTY(player) != 0) {
        UpdateStepplayerBootySave(player, 2, 0);
        SetBootyState(PBOOTY(player), 5);
    }
}

// If carrying booty: booty save case 1 (clears the carried id, increments the loot count of its kind).
// Also known as: ReturnActorBooty (upstream readable / Sly 1 name; ReturnActorBooty__FP5ACTORii).
extern "C" void DeliverStepplayerBooty(STEPPLAYER *player)
{
    if (PBOOTY(player) != 0) {
        UpdateStepplayerBootySave(player, 1, 0);
        SetBootyState(PBOOTY(player), 0);
    }
}

namespace M_PkeyFindBootySaveVar {
struct KEY { int c; unsigned short *a; };
struct OUT { SVENT *pe; int n; };
// Allocation request: byte count and alignment.
struct AREQ { int cb; int cbAlign; AREQ(int c, int a) { cb = c; cbAlign = a; } };

extern char D_002A4D88[] __asm__("D_002A4D88");
extern char D_002A4D90[] __asm__("D_002A4D90");
extern char D_002A4D98[] __asm__("D_002A4D98");
extern char D_002A4DA0[] __asm__("D_002A4DA0");
short FindSaveSymbol(SVNHASH *, char *) __asm__("FindSaveSymbol");
int FindSaveVar(SAVEVARS *, KEY *, OUT *) __asm__("FindSaveVar");
CRef GetSaveVar(SAVEVARS *p, KEY *key, SAVEDATA *pb) __asm__("GetSaveVar");
void *AllocFromStack(const AREQ &) __asm__("AllocFromStack");
void CopyAb__FPvT0Ui(void *, void *, int) __asm__("CopyAb__FPvT0Ui");

// Look up a save variable by key.
static inline CRef Lookup(KEY &k) { return GetSaveVar(&g_sw.savevars, &k, &g_gs); }
// Allocate cb bytes, 16-byte aligned.
static inline void *Alloc(int cb)
{
    AREQ req(cb, 0x10);
    return AllocFromStack(req);
}
// Searches the save trie episode.level.booty.aoidAssembly[0..14] for the given booty id.
// Also known as: AkeyStandHeldBy (upstream readable / Sly 1 name; AkeyStandHeldBy__Fi).
extern "C" KEY *FindBootySaveVar(int id)
{
    unsigned short as[8];
    KEY key;
    OUT out;
    OUT out2;
    SAVEVARS *pt = &g_sw.savevars;
    key.c = 0;
    key.a = as;
    as[0] = FindSaveSymbol(&pt->symtab, D_002A4D88);
    as[2] = FindSaveSymbol(&pt->symtab, D_002A4D90);
    as[4] = FindSaveSymbol(&pt->symtab, D_002A4D98);
    as[5] = FindSaveSymbol(&pt->symtab, D_002A4DA0);
    key.c = 1;
    FindSaveVar(pt, &key, &out);
    SVENT *pe1;
    for (int n = out.pe->ientChild; n != 0; n = pe1->ientSibling) {
        pe1 = &g_sw.savevars.aent[n];
        key.c = 3;
        as[1] = pe1->symid;
        if (FindSaveVar(&g_sw.savevars, &key, &out2)) {
            SVENT *pe2;
            for (int n2 = out2.pe->ientChild; n2 != 0; n2 = pe2->ientSibling) {
                pe2 = &g_sw.savevars.aent[n2];
                as[3] = pe2->symid;
                CRef ref;
                for (int i = 0; i < 15; i++) {
                    as[6] = i;
                    key.c = 7;
                    ref = Lookup(key);
                    if (ref.u.n == id) {
                        KEY *pk = (KEY *)Alloc(8);
                        CopyAb__FPvT0Ui(pk, &key, 8);
                        pk->a = (unsigned short *)Alloc(key.c * 2);
                        CopyAb__FPvT0Ui(pk->a, key.a, key.c * 2);
                        return pk;
                    }
                }
            }
        }
    }
    return 0;
}
} // namespace M_PkeyFindBootySaveVar

namespace M_UpdateStepplayerBootySave {
// MATCH (Claude agent, laneC20 2026-10-23): v4 + __4CRefRC4CRef (CRef copy ctor) declared returning void * instead of void.
struct CRefIsland;
extern void Decref__4CRef(CRefIsland *) __asm__("Decref__4CRef");
extern void *__4CRefRC4CRef(CRefIsland *, const CRefIsland *) __asm__("__4CRefRC4CRef"); // CRef copy ctor returns this (void *); declaring it void scheduled the pv copy early
extern CRefIsland *func_00129658(CRefIsland *, const CRefIsland *) __asm__("__as__4CRefRC4CRef");
/* CRefIsland: CRef with its copy constructor inline over the void*-returning copy function (cref.h's out-of-line
   copy constructor moves the stack slots: 37/286 off; matching-only form). */
struct CRefIsland {
    int tagk; int tag;
    // Empty (untagged) script reference.
    CRefIsland() { tagk = -1; }
    // Script reference with the given tag and value.
    CRefIsland(int k, int n) { tagk = k; tag = n; }
    // Copy a script reference (adds a reference).
    CRefIsland(const CRefIsland &r) { __4CRefRC4CRef(this, &r); }
    // Release the reference.
    ~CRefIsland() { Decref__4CRef(this); }
    // Assign a script reference.
    CRefIsland &operator=(const CRefIsland &r) { func_00129658(this, &r); return *this; }
};
// Script reference padded to 16 bytes.
struct CRef16 : CRefIsland { int pad[2]; CRef16(int k, int n) : CRefIsland(k, n) {} };
struct Y { int x0; int x4; };
struct X { char pad[0x158]; Y *p158; void *p15C; };
struct LO { char pad[0xC]; void *vptr; char pad2[0x18 - 0x10]; void *p18; char pad3[0x11A8 - 0x1C]; X *booty; };
struct Q { char pad[0x194]; void *p194; };
struct SO { char pad[0x180]; int x180; };
struct K { char pad[0xC]; unsigned short xC; };
struct S { int c; K *pk; };
struct BL { int c; void *pv; };

extern char D_002A4DB0[] __asm__("D_002A4DB0"), D_002A4D50[] __asm__("D_002A4D50"), D_002A4DC0[] __asm__("D_002A4DC0"), D_002A4DC8[] __asm__("D_002A4DC8");
void *FindBootySaveVar(void *p) __asm__("FindBootySaveVar");
int GetCurrentBootyEntryId(void *p) __asm__("GetCurrentBootyEntryId");
void *GetBootyContainerSaveKey(Q *p) __asm__("GetBootyContainerSaveKey");
S *ClearStepplayerBootySave(LO *p, int n) __asm__("ClearStepplayerBootySave");
int GetCurrentLevelIndex__Fv(void) __asm__("GetCurrentLevelIndex__Fv");
BL *GetLevelListEntry(int) __asm__("GetLevelListEntry");
int func_00233178(const void *a, const void *b, int n) __asm__("memcmp");
int FindSwObjectsByClass(void *p, int a, int b, int c, int d, SO **apso) __asm__("FindSwObjectsByClass");
int sprintf(char *, const char *, ...) __asm__("sprintf");
CRefIsland SetGlobalSaveVar(void *p, const CRefIsland &ref, SAVEDATA *pv) __asm__("SetGlobalSaveVar");

CRefIsland SetSaveVarByName(const CRefIsland &ref, const void *o, ...) __asm__("SetSaveVarByName");
CRefIsland GetSaveVarByName(const char *, ...) __asm__("GetSaveVarByName");
// Store ref into the global save variable pv.
static inline void Send(void *pv, CRefIsland ref) { SetGlobalSaveVar(pv, ref, &g_gs); }
// Store the reference r into the save variable pv.
static inline void SetPv(void *pv, const CRefIsland &r, void *)
{
    CRefData arg;
    __4CRefRC4CRef((CRefIsland *)&arg, &r);
    SetGlobalSaveVar(pv, *(CRefIsland *)&arg, &g_gs);
    Decref__4CRef((CRefIsland *)&arg);
}

// Booty save-state transitions: 0 picked up, 1 delivered, 2 dropped ...
// Also known as: SaveBootyMove (upstream readable / Sly 1 name; SaveBootyMove__FP5ACTORiP2LO).
void UpdateStepplayerBootySave(LO *lo, int n, Q *q) __asm__("UpdateStepplayerBootySave"); void UpdateStepplayerBootySave(LO *lo, int n, Q *q)
{
    STACKSCOPE m;
    switch (n) {
    case 0: {
        CRefIsland ret = SetSaveVarByName(CRefIsland(0x15, q->p194 ? GetCurrentBootyEntryId(q->p194) : -1), AsStepplayer(lo)->GetSaveVarRoot(), D_002A4DB0, 0);
        if (lo->booty) {
            if (!q->p194) {
                int dummy[2];
                void *pv0 = FindBootySaveVar(lo->p18);
                CRef16 a0(0x15, -1);
                SetPv(pv0, a0, &g_gs);
            }
            void *pv = GetBootyContainerSaveKey(q);
            CRefIsland a(0x15, GetCurrentBootyEntryId(lo->booty));
            SetPv(pv, a, &g_gs);
        } else {
            void *pv = GetBootyContainerSaveKey(q);
            CRefIsland a(0x15, (int)lo->p18);
            SetPv(pv, a, &g_gs);
        }
        break;
    }
    case 1: {
        void *pv = FindBootySaveVar(lo->p18);
        if (pv) {
            CRefIsland a(0x15, -1);
            Send(pv, a);
        }
        CRefIsland ret = SetSaveVarByName(CRefIsland(0x15, -1), AsStepplayer(lo)->GetSaveVarRoot(), D_002A4DB0, 0);
        Y *py = lo->booty->p158;
        if (py->x4 != -1) {
            char ach[32];
            sprintf(ach, D_002A4D50, py->x4);
            ret = GetSaveVarByName(D_002A4DC0, D_002A4DC8, ach, 0);
            ret = SetSaveVarByName(CRefIsland(0x15, ret.tag + 1), D_002A4DC0, D_002A4DC8, ach, 0);
        }
        break;
    }
    case 2: {
        S *ps = ClearStepplayerBootySave(lo, GetCurrentBootyEntryId(lo->booty));
        int i = GetCurrentLevelIndex__Fv();
        if (ps && i != -1) {
            BL *pb = GetLevelListEntry(i);
            int f = 0;
            if (pb->c >= 4 && func_00233178(ps->pk, pb->pv, 8) == 0) f = 1;
            if (f) {
                SO *apso[15];
                int c = FindSwObjectsByClass(&g_sw, 5, 0x25, 0, 0xF, apso);
                for (int j = 0; j < c; j++) {
                    if (apso[j]->x180 == ps->pk->xC) {
                        lo->booty->p15C = apso[j];
                        break;
                    }
                }
            }
        }
        break;
    }
    }
}
} // namespace M_UpdateStepplayerBootySave

namespace M_func_00143AF0 {
extern void Decref__4CRef(CRef *) __asm__("Decref__4CRef");
extern void __4CRefRC4CRef(CRef *, const CRef *) __asm__("__4CRefRC4CRef");
struct LO { char pad[0xC]; void *vptr; char pad2[0x18 - 0x10]; void *p18; };
extern char D_002A4DB0[] __asm__("D_002A4DB0");

void *FindBootySaveVar(void *p) __asm__("FindBootySaveVar");
CRef SetGlobalSaveVar(void *p, const CRef &ref, SAVEDATA *pv) __asm__("SetGlobalSaveVar");
CRef SetSaveVarByName(const CRef &ref, void *o, void *pv, int n) __asm__("SetSaveVarByName");

/* Same SetPv form as SetBootyContainerKind: the unused dummy param makes gcc set up %hi(D_003D4A60)
   before the copy (lui in the copy-ctor jal slot). The copy is made in a raw struct with explicit
   copy/dtor calls: an inlined ~CRef would put &arg in a pseudo that gcse PRE then shares with
   the second CRef temp at the same stack slot. */
static inline void SetPv(void *pv, const CRef &r, void *)
{
    CRefData arg;
    __4CRefRC4CRef((CRef *)&arg, &r);
    {
        const CRef &ret = SetGlobalSaveVar(pv, *(CRef *)&arg, &g_gs);
    }
    Decref__4CRef((CRef *)&arg);
}
// Marks the assembly save slot for the booty and clears the carried id.
// Also known as: AkeyReturnBooty (upstream readable / Sly 1 name; AkeyReturnBooty__FP5ACTORi).
extern "C" void *ClearStepplayerBootySave(LO *lo, int x)
{
    void *pv = FindBootySaveVar(lo->p18);
    if (pv) {
        CRef a(x, 0x15);
        SetPv(pv, a, &g_gs);
    }
    CRef ret = SetSaveVarByName(CRef(-1, 0x15), AsStepplayer(lo)->GetSaveVarRoot(), D_002A4DB0, 0);
    return pv;
}
} // namespace M_func_00143AF0

namespace M_func_00143BD8 {
extern char D_002A4DB0[] __asm__("D_002A4DB0");
void GetSaveVarByName(CRefData *r, void *a, void *b, int c) __asm__("GetSaveVarByName");
void Decref__4CRef(CRefData *r) __asm__("Decref__4CRef");
void ClearStepplayerBootySave(STEPPLAYER *p, int n) __asm__("ClearStepplayerBootySave");
void SpawnStepplayerBootyMarker(STEPPLAYER *p, int n) __asm__("SpawnStepplayerBootyMarker");
// On load: restores or clears the carried booty depending on the transition flags.
// Also known as: PostLoadActorBooty (upstream readable / Sly 1 name; PostLoadActorBooty__FP5ACTOR).
extern "C" void RestoreStepplayerBooty(STEPPLAYER *player)
{
    CRefData r;
    int n;
    GetSaveVarByName(&r, AsStepplayer(player)->GetSaveVarRoot(), D_002A4DB0, 0);
    n = r.u.n;
    Decref__4CRef(&r);
    if ((unsigned int)(n + 1) < 2) return;
    if (g_sw.transition.flags & 0x40) ClearStepplayerBootySave(player, n);
    else SpawnStepplayerBootyMarker(player, n);
}
} // namespace M_func_00143BD8

namespace RO_ORDER { extern const char r_D_002A4D88[] __asm__("D_002A4D88") = "episode"; }
namespace RO_ORDER { extern const char r_D_002A4D90[] __asm__("D_002A4D90") = "level"; }
namespace RO_ORDER { extern const char r_D_002A4D98[] __asm__("D_002A4D98") = "booty"; }
namespace RO_ORDER { extern const char r_D_002A4DA0[] __asm__("D_002A4DA0") = "aoidAssembly"; }
namespace RO_ORDER { extern const char r_D_002A4DB0[] __asm__("D_002A4DB0") = "oidBooty"; }
namespace RO_ORDER { extern const char r_D_002A4DC0[] __asm__("D_002A4DC0") = "loot"; }
namespace RO_ORDER { extern const char r_D_002A4DC8[] __asm__("D_002A4DC8") = "mplootkc"; }
namespace M_func_00143C78 {
// Slot 85 base: the ground-probe flags (step.cpp PROBE_*) from the component states (0 if +0x204, 4 in grab 3/4, 1 in the air, 7 = default ...).
// Also known as: GrfActorStance (upstream readable / Sly 1 name; GrfActorStance__FP5ACTOR).
extern "C" int GetStepplayerGroundProbeFlags(STEPPLAYER *player)
{
    if (player->hasNoBase) return 0;
    switch (player->jtgrab.state) {
    case 3:
    case 4:
        return 4;
    }
    if (player->jtair.state != -1) return 1;
    switch (player->jthurt.state) {
    case -1:
    case 0:
    case 6:
        return 7;
    case 4:
    case 7:
    case 8:
    case 12:
        return 1;
    }
    return 0;
}
} // namespace M_func_00143C78

// Slot 29: base translate, then forgets the recorded safe places.
extern "C" void TranslateStepplayerToPos(STEPPLAYER *player, qword pos)
{
    TranslateStepToPos(player, pos);
    player->jtplaces.cplace = 0;
}

namespace M_func_00143D20 {
qword GetLoWorldPos(STEPPLAYER *p) __asm__("GetLoWorldPos");
int SnapToGround(qword *ppos, int a, int b, float f, float g) __asm__("SnapToGround");
// Predicts the height after dt under gravity; if at or below the ground, tests for ground at that point.
// Also known as: FActorBelowFloorSoon (upstream readable / Sly 1 name; FActorBelowFloorSoon__FP5ACTORf).
extern "C" int WillStepplayerLand(STEPPLAYER *so, float dt)
{
    union { qword q; float f[4]; } pos;
    float h = so->vecPivotOffset.z;
    h += dt * QwordZ(so->GetVelocity());
    h += 0.5f * dt * dt * so->physics->gravity.z;
    if (h > 0.0f)
        return 0;
    pos.q = GetLoWorldPos(so);
    return SnapToGround(&pos.q, 0, 0, 0.0f, h);
}
} // namespace M_func_00143D20

namespace M_func_00143DE8 {
void ClearBhvstk__FP6BHVSTK(void *p) __asm__("ClearBhvstk__FP6BHVSTK");
int IsStepplayerInJtMode(STEPPLAYER *p) __asm__("IsStepplayerInJtMode");
void SetJtModeState__FP6JTMODEi(void *p, int n) __asm__("SetJtModeState__FP6JTMODEi");
// Sets the job sequence scripting the hero (+0x1084): the old one is released (leaving grab states 5/6), the new one
// referenced; the behaviour stack is cleared and a recon mode left.
extern "C" void SetStepplayerRide(STEPPLAYER *player, RELAYJSG *lo)
{
    RELAYJSG *old = player->pjsgScript;
    if (lo == old) return;
    if (old) {
        old->Release();
        switch (player->jtgrab.state) {
        case 5: case 6:
            player->SetPlayerState(0);
            break;
        }
    }
    player->pjsgScript = lo;
    if (!lo)
        return;
    lo->AddRef();
    ClearBhvstk__FP6BHVSTK(&player->behaviors);
    if (IsStepplayerInJtMode(player)) SetJtModeState__FP6JTMODEi(&player->jtmode, -1);
    if (player->jtthrow.state != -1 && player->pjsgScript->prelaycmdJt->relayck != RELAYCK_StepCmd1E)
        player->SetPlayerState(0);
}
} // namespace M_func_00143DE8

namespace M_func_00143EE0 {
void StopStepplayerScriptAnim(STEPPLAYER *p) __asm__("StopStepplayerScriptAnim");
void *FindSwObject__FP2SWi3OIDP2LO(void *g, int k, int oid, STEPPLAYER *p) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int IsBasicDerivedFrom__FP5BASIC3CID(void *o, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void ReleaseSma(void *p) __asm__("ReleaseSma");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *o, STEPPLAYER *p, int a, void *b, float f, float g) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void ReleaseAsega(void *p) __asm__("ReleaseAsega");
void ReplaceSma(void *o, STEPPLAYER *p, int n, int a, void *b) __asm__("ReplaceSma");
// oid -1 stops; otherwise finds the object and plays the scripted animation on the player.
// Also known as: PlayActorScriptAnim (upstream readable / Sly 1 name; PlayActorScriptAnim__FP2LOiii).
extern "C" void PlayStepplayerScriptAnim(STEPPLAYER *player, int oid, int a, int b)
{
    void *o;
    if (oid == -1) {
        StopStepplayerScriptAnim(player);
        return;
    }
    o = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 1, oid, player);
    if (!o) return;
    if (IsBasicDerivedFrom__FP5BASIC3CID(o, 0x6D)) {
        player->SetPlayerState(b);
        ReleaseSma(&player->activeStateMachine);
        ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(o, player, a, &player->activeAnimation, 0.0f, 1.0f);
    } else if (IsBasicDerivedFrom__FP5BASIC3CID(o, 0x6F)) {
        player->SetPlayerState(b);
        ReleaseAsega(&player->activeAnimation);
        ReplaceSma(o, player, -1, a, &player->activeStateMachine);
    }
}
} // namespace M_func_00143EE0

// In the scripted generic states 5/6 returns to state 0.
// Also known as: EndActorScriptAnim (upstream readable / Sly 1 name; EndActorScriptAnim__FP5ACTOR).
extern "C" void StopStepplayerScriptAnim(STEPPLAYER *player)
{
    if (IsStepplayerInScriptState(player) != 0) {
        player->SetPlayerState(0);
    }
}

// Whether the generic player state (slot 89) is 5 or 6.
// Also known as: FActorScriptAnim (upstream readable / Sly 1 name; FActorScriptAnim__FP2LO).
extern "C" int IsStepplayerInScriptState(STEPPLAYER *player)
{
    return (unsigned int)(player->GetPlayerState() - 5) < 2;
}

// True if the STEP behaviour stack is non-empty or the player rides an object.
extern "C" int IsStepplayerPushing(STEPPLAYER *player)
{
    int f = 0;
    if (player->behaviors.depth > 0 || player->pjsgScript != 0) {
        f = 1;
    }
    return f;
}

// Slot 80: PO heal check, and not dead (hurt status != 2).
// Also known as: FActorTakesCharm (upstream readable / Sly 1 name; FActorTakesCharm__FP5ACTOR).
int STEPPLAYER::CanBeHealed()
{
    if (CanPoBeHealed(this) == 0) {
        return 0;
    }
    return GetJtHurtStatus(&jthurt) != 2;
}

namespace M_BindStepplayerHpMp {
extern char D_002A4E18[] __asm__("D_002A4E18");
extern char D_002A4E20[] __asm__("D_002A4E20");
int *GetSaveVarAddressV(void *p, char *name, int n) __asm__("GetSaveVarAddressV");
void SetStepplayerHp(STEPPLAYER *p, int n) __asm__("SetStepplayerHp");
void SetStepplayerMp(STEPPLAYER *p, int n) __asm__("SetStepplayerMp");
// Binds hp/mp to the save variables hpCur/mpCur under the character's root, with defaults when unset.
// Also known as: StartPlayerCounters (upstream readable / Sly 1 name; StartPlayerCounters__FP5ACTORi).
extern "C" void BindStepplayerHpMp(STEPPLAYER *player, int f)
{
    player->jthurt.hitPoints = GetSaveVarAddressV(player->GetSaveVarRoot(), D_002A4E18, 0);
    player->gadgetPower = GetSaveVarAddressV(player->GetSaveVarRoot(), D_002A4E20, 0);
    if (!(*player->jthurt.hitPoints == 0 || f))
        return;
    SetStepplayerHp(player, player->jthurt.maxHitPoints);
    SetStepplayerMp(player, player->maxGadgetPower);
    player->gadgetPowerDrainRemainder = 0;
}
} // namespace M_BindStepplayerHpMp

// Sets hp, clamped to the maximum.
extern "C" void SetStepplayerHp(STEPPLAYER *player, int hp)
{
    int hpMax = player->jthurt.maxHitPoints;
    *player->jthurt.hitPoints = (hp >= hpMax) ? hpMax : hp;
}

namespace M_MpStepplayerCur {
// Current MP (the bound value, or the maximum when unbound).
extern "C" int GetStepplayerMp(STEPPLAYER *player)
{
    int *q = player->gadgetPower;
    if (!q) return player->maxGadgetPower;
    return *q;
}
} // namespace M_MpStepplayerCur

// Sets MP, clamped to the maximum.
extern "C" void SetStepplayerMp(STEPPLAYER *player, int mp)
{
    int *pmp = player->gadgetPower;
    if (pmp != 0) {
        int mpMax = player->maxGadgetPower;
        *pmp = (mp >= mpMax) ? mpMax : mp;
    }
}

namespace M_DrainStepplayerMp {
/* only diff: hazard nop placement (target: no nop after mfc1 before beqz, nop after mtc1); assembler-level */
int GetStepplayerMp(void *p) __asm__("GetStepplayerMp");
void SetStepplayerMp(void *p, int n) __asm__("SetStepplayerMp");
// Accumulates rate * dt and subtracts whole MP units (floored at 0).
extern "C" void DrainStepplayerMp(STEPPLAYER *player, float f)
{
    int n;
    player->gadgetPowerDrainRemainder += f * g_clock.deltaTime;
    n = (int)player->gadgetPowerDrainRemainder;
    if (!n)
        return;
    int x;
    player->gadgetPowerDrainRemainder -= (float)n;
    x = GetStepplayerMp(player) - n;
    SetStepplayerMp(player, x > -1 ? x : 0);
}
} // namespace M_DrainStepplayerMp

namespace M_AddStepplayerHpMp {
void AddPoHpMp(void *p, int a, int b) __asm__("AddPoHpMp");
void SetStepplayerHp(void *p, int a) __asm__("SetStepplayerHp");
int GetStepplayerMp(void *p) __asm__("GetStepplayerMp");
void SetStepplayerMp(void *p, int a) __asm__("SetStepplayerMp");
// Slot 81: PO base, then adds hp and mp (clamped).
// Also known as: HealActor (upstream readable / Sly 1 name; HealActor__FP5ACTORii).
extern "C" void AddStepplayerHpMp(STEPPLAYER *player, int a, int b)
{
    AddPoHpMp(player, a, b);
    SetStepplayerHp(player, *player->jthurt.hitPoints + a);
    SetStepplayerMp(player, GetStepplayerMp(player) + b);
}
} // namespace M_AddStepplayerHpMp

namespace M_func_001442D0 {
void AddStepDialogSmRefs(void *p, int a, int b) __asm__("AddStepDialogSmRefs");
void PlayStepplayerScriptAnim(void *p, int a, int b, int c) __asm__("PlayStepplayerScriptAnim");
void SetJtModeState__FP6JTMODEi(void *p, int a) __asm__("SetJtModeState__FP6JTMODEi");
// Slot 83 (STEP behaviour push hook): base; when the count goes 0 -> 1 plays the scripted animation. (inferred)
extern "C" void AddStepplayerDialogSmRefs(STEPPLAYER *player, int a, int b)
{
    AddStepDialogSmRefs(player, a, b);
    if (!b)
        return;
    if (++player->unknown12F4 == 1) {
        PlayStepplayerScriptAnim(player, 0x60B, 9, 5);
        SetJtModeState__FP6JTMODEi(&player->jtmode, -1);
    }
}
} // namespace M_func_001442D0

namespace M_func_00144338 {
void RemoveStepDialogSmRefs(void *p, int a, int b) __asm__("RemoveStepDialogSmRefs");
void StopStepplayerScriptAnim(void *p) __asm__("StopStepplayerScriptAnim");
// Slot 84 (STEP behaviour pop hook): base; when the count drops to 0 stops the scripted animation. (inferred)
extern "C" void RemoveStepplayerDialogSmRefs(STEPPLAYER *player, int a, int b)
{
    RemoveStepDialogSmRefs(player, a, b);
    if (b) {
        if (--player->unknown12F4 == 0) StopStepplayerScriptAnim(player);
    }
}
} // namespace M_func_00144338

namespace M_FIgnoreIntersection__10STEPPLAYERP2SO {
int ShouldSoIgnoreIntersection(SO *p, SO *other) __asm__("ShouldSoIgnoreIntersection");
int AdjustJtThrowFindFlags(JTTHROW *p, SO *other, int grf) __asm__("AdjustJtThrowFindFlags");
int GetJtHandsIgnoreFlags(JTHANDS *p, SO *other, int grf) __asm__("GetJtHandsIgnoreFlags");
// Slot 61: SO base flags, then the throw controller (AdjustJtThrowFindFlags) and grab component add theirs.
// Also known as: OverlapKindActor (upstream readable / Sly 1 name; OverlapKindActor__FP3ALOi).
extern "C" int ShouldStepplayerIgnoreIntersection__10STEPPLAYERP2SO(STEPPLAYER *player, SO *other)
{
    int grf = ShouldSoIgnoreIntersection(player, other);
    grf = AdjustJtThrowFindFlags(&player->jtthrow, other, grf);
    return GetJtHandsIgnoreFlags(&player->jtgrab.hands, other, grf);
}
} // namespace M_FIgnoreIntersection__10STEPPLAYERP2SO

// Whether a look mode is active (state 0..2).
extern "C" int IsStepplayerInJtMode(STEPPLAYER *player)
{
    return (unsigned int)player->jtmode.state < 3;
}

// Whether the requested look mode is kind 0 (binocucom).
extern "C" int IsStepplayerJtModeBinocucom(STEPPLAYER *player)
{
    return player->jtmode.kind == 0;
}

// Whether the requested look mode is kind 1 (recon).
extern "C" int IsStepplayerJtModeRecon(STEPPLAYER *player)
{
    return player->jtmode.kind == 1;
}

// Whether the requested look mode is kind 2.
extern "C" int IsStepplayerJtModeKind2(STEPPLAYER *player)
{
    return player->jtmode.kind == 2;
}

namespace M_func_00144418 {
/* only diff: original assembler padded a backward branch (to a jr) with a nop at 0x14443C; asfix.py does not pad when a jump is in between. Code otherwise identical. */
struct Q { char pad[0xC]; int c; int n; };
// Slot 86 (IsZapLethal): 1 when the zap's damage (-n) reaches JTHURT::maxHitPoints and its kind is not 0, 1, 0x80000 or 0x100000; else 0. (inferred)
// Also known as: FDamageFatal (upstream readable / Sly 1 name; FDamageFatal__FP5ACTORP5CTMSG).
extern "C" int IsStepplayerZapLethal(STEPPLAYER *player, struct Q *q)
{
    if (-q->n < player->jthurt.maxHitPoints) return 0;
    switch (q->c) {
    case 0:
    case 1:
    case 0x80000:
    case 0x100000:
        return 0;
    }
    return 1;
}
} // namespace M_func_00144418

namespace M_OnStepplayerActive {
void OnPoActive(void *p, int a) __asm__("OnPoActive");
void ReleaseAsega(void *p) __asm__("ReleaseAsega");
// Slot 78: PO base, then slot 105; when deactivated stops the current action.
extern "C" void OnStepplayerActive(STEPPLAYER *player, int a)
{
    OnPoActive(player, a);
    player->OnGadgetsChanged();
    if (!a) {
        ReleaseAsega(&player->pasegaGadgetOverlay);
        player->idGadgetOverlay = -1;
    }
}
} // namespace M_OnStepplayerActive

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const char D_002A4E18[] = "hpCur";
extern const char D_002A4E20[] = "mpCur";
struct SNIP { int grfsnip; int oid; int ib; };   /* Sly 1's SnipLoObjects record */
SNIP D_002BC9F0[0x35] = {{0x110, 0x26E, SLY2_OFFSETOF(STEPPLAYER, unknown10BC[0])}, {0x300, 0x26C, SLY2_OFFSETOF(STEPPLAYER, jtgrab.gaitA.abl[0].animation)}, {0x300, 0x26D, SLY2_OFFSETOF(STEPPLAYER, jtgrab.gaitA.abl[1].animation)}, {0x300, 0x26B, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown230[0])},
    {0x110, 0x5DC, SLY2_OFFSETOF(STEPPLAYER, unknown10BC[1])}, {0x300, 0x5DE, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown320[1])}, {0x300, 0x5DF, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown320[2])}, {0x300, 0x5E0, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown320[3])},
    {0x300, 0x5E1, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[0].animation)}, {0x300, 0x5E2, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[1].animation)}, {0x300, 0x5E3, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[2].animation)}, {0x300, 0x5E4, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[3].animation)},
    {0x300, 0x5E5, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[4].animation)}, {0x300, 0x5E6, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[5].animation)}, {0x300, 0x5E7, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[6].animation)}, {0x300, 0x5E8, SLY2_OFFSETOF(STEPPLAYER, jtgrab.abldirB[7].animation)},
    {0x320, 0x5DD, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown64)}, {0x300, 0x27A, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown4F0[2])}, {0x300, 0x26A, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown510[1])}, {0x300, 0x2AC, SLY2_OFFSETOF(STEPPLAYER, hideNoise)},
    {0x100, 0x5EC, SLY2_OFFSETOF(STEPPLAYER, jtair.ptnLaunch)}, {0x100, 0x5ED, SLY2_OFFSETOF(STEPPLAYER, jtair.unknown170)}, {0x300, 0x296, SLY2_OFFSETOF(STEPPLAYER, jthurt.aprecHurt[13])}, {0x300, 0x5EE, SLY2_OFFSETOF(STEPPLAYER, jtair.unknown178)},
    {0x300, 0x5EF, SLY2_OFFSETOF(STEPPLAYER, jtair.abl[0].animation)}, {0x300, 0x5F0, SLY2_OFFSETOF(STEPPLAYER, jtair.abl[1].animation)}, {0x300, 0x5F1, SLY2_OFFSETOF(STEPPLAYER, jtair.abl[2].animation)}, {0x300, 0x5F3, SLY2_OFFSETOF(STEPPLAYER, jtair.abl[3].animation)},
    {0x300, 0x5F2, SLY2_OFFSETOF(STEPPLAYER, jtair.abl[4].animation)}, {0x300, 0x5F4, SLY2_OFFSETOF(STEPPLAYER, jtair.unknown1BC)}, {0x300, 0x24B, SLY2_OFFSETOF(STEPPLAYER, jtair.unknown1C8)}, {0x300, 0x283, SLY2_OFFSETOF(STEPPLAYER, jtgrab.hands.ploHand)},
    {0x300, 0x26F, SLY2_OFFSETOF(STEPPLAYER, jtgrab.hands.abl[0].animation)}, {0x300, 0x270, SLY2_OFFSETOF(STEPPLAYER, jtgrab.hands.abl[1].animation)}, {0x300, 0x281, SLY2_OFFSETOF(STEPPLAYER, jtgrab.unknown508)}, {0x300, 0x271, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ablC[0].animation)},
    {0x300, 0x272, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ablC[1].animation)}, {0x300, 0x273, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ablB[0].animation)}, {0x300, 0x274, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ablB[1].animation)}, {0x300, 0x275, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ablB[2].animation)},
    {0x300, 0x276, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ablB[3].animation)}, {0x300, 0x277, SLY2_OFFSETOF(STEPPLAYER, jtthrow.pasegAimTurnA)}, {0x300, 0x278, SLY2_OFFSETOF(STEPPLAYER, jtthrow.pasegAimTurnB)}, {0x300, 0x283, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ploHoldParent)},
    {0x300, 0x24A, SLY2_OFFSETOF(STEPPLAYER, jtthrow.ploMarker)}, {0x300, 0x1A3, SLY2_OFFSETOF(STEPPLAYER, unknown102C)}, {0x300, 0x2AD, SLY2_OFFSETOF(STEPPLAYER, unknown11A4)}, {0x300, 0x2A6, SLY2_OFFSETOF(STEPPLAYER, jtpipe.pasegMount)},
    {0x300, 0x2A7, SLY2_OFFSETOF(STEPPLAYER, jtpipe.pasegHold)}, {0x300, 0x2A8, SLY2_OFFSETOF(STEPPLAYER, jtpipe.pasegTurn)}, {0x300, 0x2A9, SLY2_OFFSETOF(STEPPLAYER, jtpipe.abl[0].animation)}, {0x300, 0x2AA, SLY2_OFFSETOF(STEPPLAYER, jtpipe.abl[1].animation)},
    {0x300, 0x181, SLY2_OFFSETOF(STEPPLAYER, pemitterGrab)}};
float D_002BCC6C = 2.0f;
float D_002BCC70 = -4.0f;
float D_002BCC74 = 4.0f;
float D_002BCC78 = -4.0f;
float D_002BCC7C = 1700.0f;
float D_002BCC80 = 1000.0f;
float D_002BCC84 = -750.0f;
float D_002BCC88 = 750.0f;
float D_002BCC8C = 500.0f;
float D_002BCC90 = 500.0f;
float D_002BCC94 = 2.0f;
float D_002BCC98 = -300.0f;
float D_002BCC9C = 200.0f;
float D_002BCCA0 = -125.0f;
float D_002BCCA4 = 200.0f;
float D_002BCCA8 = -275.0f;
float D_002BCCAC = 150.0f;
float D_002BCCB0 = -50.0f;
float D_002BCCB4 = 85.0f;
SMPA D_002BCCB8 = {4000.0f, 0.0f, 4.0f, 0.0f};
SMPA D_002BCCC8 = {4000.0f, 0.0f, 0.3f, 0.0f};
SMPA D_002BCCD8 = {1500.0f, 0.0f, 0.5f, 0.0f};
VECTOR4 D_002BCCF0 = {1.5f, -0.5f, 0.0f, 0.0f};
/* g_agadgetdesc (GADGETDESC, defined at the top of the file). */
GADGETDESC D_002BCD00[43] = {{-1, 0, 0, 0, 0, 0, 0, 0}, {-1, 0, 0, 0, 0, 0, 0, 0}, {-1, 0, 0, 0, 0, 0, 0, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0}, {2, 0, 0, 0, 0, 0, 0, 0}, {2, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0},
    {2, 8, 200, 0, 1, 129, 130, 209}, {2, 8, 500, 5, 2, 131, 132, 205}, {2, 8, 550, 5, 3, 133, 134, 204},
    {2, 8, 1000, 15, 4, 135, 136, 205}, {2, 8, 1050, 20, 5, 137, 138, 209}, {2, 8, 1200, 1, 6, 139, 140, 208},
    {2, 8, 1700, 20, 7, 141, 142, 204}, {2, 8, 2000, 25, 8, 143, 144, 205}, {2, 9, 400, 10, 1, 147, 148, 205},
    {2, 9, 400, 0, 2, 149, 150, 212}, {-1, 9, 750, 0, 3, 151, 152, 206}, {-1, 9, 900, 0, 4, 153, 154, 206},
    {2, 9, 2100, 20, 8, 155, 156, 214}, {2, 9, 1400, 10, 6, 157, 158, 205}, {2, 9, 1500, 15, 7, 159, 160, 205},
    {2, 9, 1150, 25, 5, 161, 162, 205}, {2, 7, 300, 30, 1, 163, 164, 205}, {2, 7, 600, 0, 2, 165, 166, 215},
    {2, 7, 650, 1, 3, 167, 168, 208}, {2, 7, 600, 5, 4, 169, 170, 216}, {-1, 7, 600, 0, 5, 171, 172, 211},
    {-1, 7, 1300, 0, 6, 173, 174, 210}, {1, 7, 1600, 2, 7, 175, 176, 208}, {2, 7, 1900, 0, 8, 177, 178, 205},
    {2, 7, 0, 0, -1, 179, 180, 205}, {-1, 7, 0, 0, -1, 181, 182, 205}, {2, 7, 0, 10, -1, 183, 184, 205},
    {2, 7, 0, 10, -1, 185, 186, 205}, {2, 7, 0, 10, -1, 187, 188, 205}, {-1, 7, 0, 0, -1, 189, 190, 206},
    {2, 7, 0, 25, -1, 191, 192, 216}, {2, 7, 0, 15, -1, 193, 194, 216}, {2, 7, 0, 20, -1, 195, 196, 205},
    {2, 7, 0, 3, -1, 197, 198, 208}, {2, 7, 0, 5, -1, 199, 200, 209}, {1, 7, 0, 1, -1, 201, 202, 208}};
float D_002BD260 = 2.0f;
float D_002BD264 = 1.0f;
float D_002BD268 = 0.15f;
float D_002BD26C = 0.0f;
VECTOR4 D_002BD270 = {12.0f, 2.0f, 0.1f, 0.0f};
VECTOR4 D_002BD280 = {-200.0f, -0.2f, 0.0f, 0.0f};
VECTOR4 D_002BD290 = {0.0f, -0.2f, 0.0f, 0.0f};
VECTOR4 D_002BD2A0 = {0.5f, -0.5f, 0.0f, 0.0f};
VECTOR4 D_002BD2B0 = {-500.0f, -0.5f, 0.0f, 0.0f};
float D_002BD2C0 = 1.0f;
float D_002BD2C4 = 2.5f;
int D_002DD640;
