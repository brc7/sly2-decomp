// hack.cpp -- the hacking minigame objects (Bentley's hack: HACK : SO and HACKSHIP, HACKENEMY, HACKLOCK,
// HACKWALL...): health and states, entry tables bound by oid, the ship singleton, scales and ranges, wall segments.
// Entry points: the HACK/HACKSHIP/HACKBULLET/HACKENEMY/HACKLOCK/HACKWALL vtables, their Splice setters
// (SetHack*Entry, AddHackwallSegment, AddHacklockRsmg...) and the world callback StartHackDying schedules.
// TU 0x17F090..0x182C70 (76 functions, address order).
//
// Types: the classes and their records from sly2/hack.h (HACK, HACKSHIP, HACKBULLET, HACKENEMY, HACKLOCK, HACKWALL;
// HACKE, HACKSHIPPART, HACKRULE, HACKWALLSEG, HACKWALLPIECE). The per-function namespaces (as jt.cpp has them) keep
// only the helpers that are part of a function's tuning and small views of records of other families (XP contact
// sides, the contact request, the LO flag word read as one 64-bit value).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_hack
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_hack

#include "sly2/port.h"   /* uptr */
#include "sly2/basic.h"
#include "sly2/so.h"
#include "sly2/slotheap.h"
#include "sly2/scrap.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#define SLY2_KEY_TU_HACKSHIP   /* this TU is the class's key TU: see sly2/classes.h */
#include "sly2/classes.h"
#include "sly2/hack.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/dli.h"
#include "sly2/sw.h"
#include "sly2/lsg.h"   /* LSG: the line clip's span records */

// 0x182B18 StaticInit_hack and GlobalConstructors_hack: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

extern "C" {
extern HACKSHIP *D_002DFB80;    /* the ship singleton */
extern float D_002BE870[3], D_002BE87C, D_002BE884;
void RemoveSoHierarchy(SO *pso);
void SO_ResetForProxy(SO *pso, int n);
void AdjustSoNewXp(SO *pso, XP *pxp);
void InitHack(HACK *hack);
void InitHackAnimEntries(HACK *hack, int c, HACKE *ahacke);
void RemoveHackHierarchy(HACK *hack);
int ResetHackHealth(HACK *hack, int n);
void PostHackLoad(HACK *hack);
void ResetHack(HACK *hack);
void SetHackshipState(HACKSHIP *ship, int nState);
void SetHackenemyState(HACKENEMY *enemy, int nState);
void UpdateHackshipHealthBar(HACKSHIP *ship);
int IsPoActive(SO *pso);
void BindHackAnimEntries(HACK *hack, int c, HACKE *ahacke);
void ActivateHackshipFromHacklock(HACKLOCK *lock);
void StartHackAnim(HACK *hack, HACKE *phacke);
void BroadcastGameEvent(void *pgame, int nEvent, LO *plo);
void SetHackwallSegmentState(HACKWALL *wall, HACKWALLPIECE *ppiece, int n);
}
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
LO *FindSwObject(SW *psw, int grffso, OID oid, LO *plo);

namespace M_func_0017F090 {
extern qword D_002BE7D0 __asm__("D_002BE7D0");
extern LM D_002BE7E0 __asm__("D_002BE7E0");
void InitSo(SO *pso) __asm__("InitSo");
// HACK (cid 0x47) vtable slot 7: InitSo, max health 10, the default knockback gains and range. (inferred)
// Also known as: InitHackSo (upstream readable / Sly 1 name; InitHackSo__FP2LO).
extern "C" void InitHack(HACK *hack) {
    uptr v;
    InitSo(hack);
    hack->cHealthMax = 10;
    hack->qwKnockback = D_002BE7D0;
    hack->lmKnockback = D_002BE7E0;
    v = (uptr)hack->ploupd;   /* LO's tagged word (lo.h) */
    if ((v ^ 1) & 1) ((LOUPD *)v)->grf |= 2; else hack->inlineFlags = v | 2;
}
} // namespace M_func_0017F090

// vtable slot 12: RemoveSoHierarchy, then clears the dying flag. (inferred)
/* HACK slot 12. */
// Also known as: OnRemoveHackSo (upstream readable / Sly 1 name; OnRemoveHackSo__FP2LO).
void HACK::RemoveHierarchy()
{
    RemoveSoHierarchy(this);
    fDying = 0;
}

namespace M_func_0017F138 {
void PostSoLoad(SO *p) __asm__("PostSoLoad");
// vtable slot 15: PostSoLoad then own vtable slot 83. (inferred)
// Also known as: PostLoadHackSoBase (upstream readable / Sly 1 name; PostLoadHackSoBase__FP2LO).
extern "C" void PostHackLoad(HACK *p) { PostSoLoad(p); p->Reset(); }
} // namespace M_func_0017F138

// vtable slot 26: SO base, then health = max health (reset). (inferred)
/* HACK slot 26: reset the health. */
extern "C" int ResetHackHealth(HACK *hack, int n)
{
    int c;
    SO_ResetForProxy(hack, n);
    c = hack->cHealthMax;
    hack->cHealth = c;
    return c;
}

// vtable slot 55: SO base, then clears the contact adjustment words +0x94/+0x98 (same as pipe twin ResetJtPipeAdjust). (inferred)
/* HACK slot 55: clear the contact's adjustment (as pipe.cpp's ResetJtPipeAdjust). */
extern "C" void AdjustHackNewXp(HACK *hack, XP *pxp)
{
    AdjustSoNewXp(hack, pxp);
    *(int *)((char *)pxp + 0x94) = 0;
    *(int *)((char *)pxp + 0x98) = 0;
}

namespace M_func_0017F1D0 {
/* Apply damage and a projected knockback impulse from the message. */
extern qword g_normalZ __asm__("g_normalZ"),D_002A2DA0 __asm__("D_002A2DA0");
qword GetLoWorldPos(HACK*) __asm__("GetLoWorldPos");
qword LimitVectorLength(qword,float) __asm__("LimitVectorLength");
qword ApplyConstr(CONSTR*,XF*,qword) __asm__("ApplyConstr");
// Helpers Sub, Reject, Normal, AddScaled, Clamp (small asm wrappers/accessors shaped for the match).
extern "C" { static inline qword Reject(qword a,qword n){qword d,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a),"j"(n));__asm__("vmulx.xyzw %0,%1,%2x":"=j"(d):"j"(n),"j"(d));return VuSub(a,d);} }
// Unit vector along a, or fallback when its length is too small.
extern "C" { static inline qword Normal(qword a,qword fallback){register qword d __asm__("vf1");qword one;float len;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));__asm__("qmfc2.ni %0,%1":"=r"(len):"j"(d));if(len<0.0001f){d=fallback;}else{__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(d):"j"(d),"j"(a));}return d;} }
// Return a + b * g.
extern "C" { static inline qword AddScaled(qword a,qword b,float g){qword k;QWROW u;__asm__("qmtc2.ni %1,%0":"=j"(k):"r"(g));u.q=k;__asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"=j"(a):"j"(b),"j"(k),"0"(a));return a;} }
// vt75 (msg with flag 8): health += delta; <=0 -> vt82 Die; else knockback away from hit pos (quadratic gain,
// limited) through constraint and SetVelocity. (inferred)
// Also known as: FHackSoHit (upstream readable / Sly 1 name; FHackSoHit__FP2LOP5CTMSG).
extern "C" int TakeHackDamage(HACK*hack,ZPR*pzpr){
 qword dir,v;float g,t;
 if(!(pzpr->grfzpr&8))return 0;
 hack->cHealth+=pzpr->dhp;
 if(hack->cHealth<=0){hack->Kill(0);return 1;}
 if(!hack->physics || hack->physics->constrForce.kind==3)return 1;
 dir=Normal(Reject(VuSub(GetLoWorldPos(hack),pzpr->position),g_normalZ),D_002A2DA0);
 {float *a=&hack->clqKnockback.g0;t=-pzpr->dhp;g=Clamp(hack->clqKnockback.g0+t*(a[1]+t*a[2]),hack->lmKnockback.minValue,hack->lmKnockback.maxValue);}
 v=hack->GetVelocity();
 v=LimitVectorLength(AddScaled(v,dir,g),hack->lmKnockback.maxValue);
 v=ApplyConstr(&hack->physics->constrForce,hack->positionXf,v);
 hack->SetVelocity(v);
 return 1;
}
} // namespace M_func_0017F1D0

namespace M_func_0017F3E0 {
// Set oid -1 on c entries. (inferred)
extern "C" void InitHackAnimEntries(HACK *hack, int c, HACKE *phacke) { int i; for (i = 0; i < c; i++) { phacke->oid = OID_Nil; phacke++; } }
} // namespace M_func_0017F3E0

// Resolves each entry's oid to its animation (an ASEG) or state machine (an SM).
/* Resolves the entries' oids. */
// Also known as: BindHackSegs (upstream readable / Sly 1 name; BindHackSegs__FP2LOP6HACKSOPv).
extern "C" void BindHackAnimEntries(HACK *hack, int c, HACKE *ahacke)
{
    HACKE *phacke;
    int i;
    LO *plo;
    if (c > 0) {
        phacke = ahacke;
        i = c;
        do {
            if (phacke->oid != -1) {
                plo = FindSwObject(&g_sw, 4, phacke->oid, hack);
                if (plo != 0) {
                    if (IsBasicDerivedFrom(plo, (CID)0x6D) != 0) {
                        phacke->paseg = (ASEG *)plo;
                    } else if (IsBasicDerivedFrom(plo, (CID)0x6F) != 0) {
                        phacke->psm = (SM *)plo;
                    }
                }
            }
            i--;
            phacke++;
        } while (i != 0);
    }
}

namespace M_func_0017F4C0 {
void ReleaseSma(SMA **ppsma) __asm__("ReleaseSma");
void ReleaseAsega(ASEGA **ppasega) __asm__("ReleaseAsega");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *paseg, LO *plo, float t, float svt, int grf, ASEGA **ppasega) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
void ReplaceSma(SM *psm, LO *plo, int n, int grf, SMA **ppsma) __asm__("ReplaceSma");
// Retract the current ASEGA / SMA and apply the entry's clip and SM state. (inferred)
// Also known as: PlayHackSeg (upstream readable / Sly 1 name; PlayHackSeg__FP2LOP6HACKSO).
extern "C" void StartHackAnim(HACK *hack, HACKE *phacke) {
    ReleaseSma(&hack->psma);
    ReleaseAsega(&hack->pasega);
    if (phacke) {
        ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(phacke->paseg, hack, 0.0f, 1.0f, phacke->grf, &hack->pasega);
        ReplaceSma(phacke->psm, hack, -1, phacke->grf, &hack->psma);
    }
}
} // namespace M_func_0017F4C0

namespace M_func_0017F550 {
int IsAsegaRetracted(ASEGA *pasega) __asm__("IsAsegaRetracted");
int GetSmaCurrentState(SMA *psma) __asm__("GetSmaCurrentState");
int GetSmaGoal(SMA *psma) __asm__("GetSmaGoal");
// True when the current ASEGA retracted or the SM reached its end state. (inferred)
// Also known as: FShipSegmentDone (upstream readable / Sly 1 name; FShipSegmentDone__FP2LO).
extern "C" int IsHackAnimDone(HACK *hack) { if (hack->pasega) return IsAsegaRetracted(hack->pasega); { int f = 1; if (hack->psma) f = GetSmaCurrentState(hack->psma) == GetSmaGoal(hack->psma); return f; } }
} // namespace M_func_0017F550

namespace M_func_0017F5B0 {
extern const struct LONGU D_002A5630 __asm__("D_002A5630");
void PostSwCallback(SW *g, void *p, long k, int n) __asm__("PostSwCallback");
// Once: set the dying flag and post the Kill callback (D_002A5630: a pointer to HACK's slot 82) with n. (inferred)
// Also known as: PostKillHackSo (upstream readable / Sly 1 name; PostKillHackSo__FP2LOi).
extern "C" void StartHackDying(HACK *hack, int n) { if (hack->fDying) return; hack->fDying = 1; PostSwCallback(&g_sw, hack, D_002A5630.l, n); }
} // namespace M_func_0017F5B0

namespace M_func_0017F5F8 {
void *func_002333B4(void *p, int c, int n) __asm__("memset");
qword GetLoWorldPos(LO *plo) __asm__("GetLoWorldPos");
void DeliverZapToSo(ZPR *pzpr) __asm__("DeliverZapToSo");
void ExplodeWithDefaults(EXPL *pexpl) __asm__("ExplodeWithDefaults");
// vtable slot 82: if it carries a damage, area damage at its position (credited to the killer); play its explosion,
// then vtable slot 9 Remove. (inferred)
// Also known as: KillHackSo (upstream readable / Sly 1 name; KillHackSo__FP2LOi).
extern "C" void KillHack(HACK *hack, LO *ploKiller) {
    if (ploKiller && hack->cDamage) {
        ZPR zpr;
        func_002333B4(&zpr, 0, sizeof(zpr));
        zpr.unknown08 = ploKiller;
        zpr.ploSource = hack;
        zpr.position = GetLoWorldPos(hack);
        zpr.velocity = hack->GetVelocity();
        zpr.zpk = 0x100;
        zpr.dhp = -hack->cDamage;
        DeliverZapToSo(&zpr);
    }
    if (hack->pexpl) ExplodeWithDefaults(hack->pexpl);
    hack->Remove();
}
} // namespace M_func_0017F5F8

namespace M_func_0017F6B0 {
extern const struct LONGU D_002A5638 __asm__("D_002A5638");
void ClearSwCallbacks(SW *g, int a, void *p, long k, int n) __asm__("ClearSwCallbacks");
// Health = max; cancel a pending dying callback (ClearSwCallbacks D_002A5638). (inferred)
// Also known as: ResetHackSoBase (upstream readable / Sly 1 name; ResetHackSoBase__FP2LO).
extern "C" void ResetHack(HACK *hack) { hack->cHealth = hack->cHealthMax; if (hack->fDying) ClearSwCallbacks(&g_sw, 1, hack, D_002A5638.l, 0); }
} // namespace M_func_0017F6B0

// CID 0x48 vtable slot 7: InitCid47, 5 entries, state -1, register singleton D_002DFB80 = this, sound -1. (inferred)
extern "C" void InitHackship(HACKSHIP *ship)
{
    InitHack(ship);
    InitHackAnimEntries(ship, 5, ship->ahacke);
    ship->state = -1;
    D_002DFB80 = ship;
    ship->sfxidEngine = -1;
}

// vtable slot 12: base then state change via SetCid48State. (inferred)
/* HACKSHIP slot 12. */
void HACKSHIP::RemoveHierarchy()
{
    RemoveHackHierarchy(this);
    SetHackshipState(this, -1);
}

namespace M_func_0017F770 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
XF *FindSoXf(SO *pso, OID oid, int n) __asm__("FindSoXf");
ACT *NewPositionAct(SO *pso, XF *pxf, int a, int b, int d, qword q) __asm__("NewPositionAct");
SMA *ApplySmToObject(LO *plo, SO *pso, int a, int b) __asm__("ApplySmToObject");
// Look up the parts' transforms and objects; a position actuator on each transform, the state machine on each object. (inferred)
// Also known as: BindShipGuns (upstream readable / Sly 1 name; BindShipGuns__FP2LO).
extern "C" void BindHackshipParts(HACKSHIP *ship) {
    int i;
    for (i = 0; i < ship->cpart; i++) {
        HACKSHIPPART *ppart = &ship->apart[i];
        ppart->pxf = FindSoXf(ship, ppart->oidXf, 6);
        ppart->plo = FindSwObject(&g_sw, 4, ppart->oidObject, ship);
        if (ppart->pxf) ppart->pact = NewPositionAct(ship, ppart->pxf, 0, 7, 2, D_002A2DD0);
        if (ppart->plo) ppart->psma = ApplySmToObject(ppart->plo, ship, -1, 0);
    }
}
} // namespace M_func_0017F770

namespace M_func_0017F850 {
void SetXfRole(SO *p, XF *pxf, int role) __asm__("SetXfRole");
void BindHackshipParts(HACKSHIP *p) __asm__("BindHackshipParts");
// vtable slot 15: PostCid47Load, bind entries and parts. (inferred)
// Also known as: PostLoadHackShip (upstream readable / Sly 1 name; PostLoadHackShip__FP2LO).
extern "C" void PostHackshipLoad(HACKSHIP *p) { PostHackLoad(p); p->SetCtlState(0); SetXfRole(p, p->positionXf, 2); SetXfRole(p, p->rotationXf, 2); BindHackshipParts(p); BindHackAnimEntries(p, 5, p->ahacke); }
} // namespace M_func_0017F850

namespace M_func_0017F8C8 {
void OnPoActive(PO *ppo, int f) __asm__("OnPoActive");
// vtable slot 78: base then SetHackshipState. (inferred)
// Also known as: SetHackShipFocus (upstream readable / Sly 1 name; SetHackShipFocus__FP2LOi).
extern "C" void OnHackshipActive(HACKSHIP *ship, int f) { OnPoActive(ship, f); if (f) SetHackshipState(ship, 1); else if ((int)ship->isInWorld & 1) SetHackshipState(ship, 0); }
} // namespace M_func_0017F8C8

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const unsigned int r_D_002A5630[2] __asm__("D_002A5630") = {0x530000, 0xC}; }
namespace RO_ORDER { extern const unsigned int r_D_002A5638[2] __asm__("D_002A5638") = {0, 0}; }
namespace M_func_0017F930 {
extern float D_002BE7E8 __asm__("D_002BE7E8");
extern char D_002BE7F0[] __asm__("D_002BE7F0");
void UpdateSo(SO *pso) __asm__("UpdateSo");
int IsHackAnimDone(HACK *hack) __asm__("IsHackAnimDone");
qword SetVectorCylind(float a, float b, float c) __asm__("SetVectorCylind");
void SetActPositionGoal(ACT *pact, qword q) __asm__("SetActPositionGoal");
void SetSmaGoal(SMA *psma, int n) __asm__("SetSmaGoal");
float SmoothValue__FfffP3SMPPf(void *, int, float, float, float) __asm__("SmoothValue__FfffP3SMPPf");
void SetSndVolume(CID151 *psnd, float g) __asm__("SetSndVolume");
void ResumeSnd(CID151 *psnd) __asm__("ResumeSnd");
void PauseSnd(CID151 *psnd) __asm__("PauseSnd");
struct V3 { float x, y, z; };
// Zeroes the x, y, z of a vector (w untouched).
extern "C" { static __inline__ void ZeroVector(qword *pq) { struct V3 *pv = (struct V3 *)pq; pv->z = 0; pv->y = 0; pv->x = 0; } }
// vtable slot 16: UpdateSo; state 2 -> 1 when anim done; per-state movement/actuator goals from the controller's
// sticks (POCTL's names: left stick = heading and speed, right stick = height and the parts' goal). (inferred)
// Also known as: UpdateShip (upstream readable / Sly 1 name; UpdateShip__FP2LO).
extern "C" void UpdateHackship(HACKSHIP *ship) {
    int s;
    UpdateSo(ship);
    do {
        s = ship->state;
        if (s == 2 && IsHackAnimDone(ship)) SetHackshipState(ship, 1);
    } while (ship->state != s);
    switch (ship->state) {
    /* one copy per case: the original's jump table has four separate (identical) targets, so merging the labels
       changes the code */
    case 0: ZeroVector(&ship->desiredVelocity); break;
    case 2: ZeroVector(&ship->desiredVelocity); break;
    case 3: ZeroVector(&ship->desiredVelocity); break;
    case 4: ZeroVector(&ship->desiredVelocity); break;
    case 1: {
        POCTL *pctl = ship->controller;
        union { qword q; } u; qword v;
        int i;
        ship->heading = pctl->leftStickAngle;
        ship->desiredVelocity = SetVectorCylind(pctl->leftStickAngle, pctl->leftStickDeflection * D_002BE7E8, 0.0f);
        v = QwordFromFloats<qword>(0.0f, 0.0f, ship->controller->rightStickAngle); u.q = v;
        for (i = 0; i < ship->cpart; i++) {
            HACKSHIPPART *ppart = &ship->apart[i];
            if (ppart->pact) SetActPositionGoal(ppart->pact, v);
            if (ppart->psma) {
                if (0.0f < ship->controller->rightStickDeflection) SetSmaGoal(ppart->psma, 0x68);
                else SetSmaGoal(ppart->psma, 0x66);
            }
        }
        break;
    }
    }
    if (ship->psndEngine) {
        float g = (ship->state == 1) ? ship->controller->leftStickDeflection : 0.0f;
        ship->uVolEngine = SmoothValue__FfffP3SMPPf(D_002BE7F0, 0, ship->uVolEngine, g, g_clock.deltaTime);
        SetSndVolume(ship->psndEngine, ship->uVolEngine);
        if (0.0f < ship->uVolEngine) {
            if (ship->psndEngine->pauseCount > 0) ResumeSnd(ship->psndEngine);
        } else {
            if (ship->psndEngine->pauseCount <= 0) PauseSnd(ship->psndEngine);
        }
    }
}
} // namespace M_func_0017F930

namespace M_func_0017FB50 {
#include "mat.h"

extern qword g_normalZ __asm__("g_normalZ"), D_002A2DD0 __asm__("D_002A2DD0");
extern char D_002BE800[] __asm__("D_002BE800"), D_002BE810[] __asm__("D_002BE810");
MATRIX3 BuildAxisAngleRotationMatrix(qword q, float g) __asm__("BuildAxisAngleRotationMatrix");
void AccelSoTowardMatBounded(SO *pso, MATRIX3 *pm, void *pv) __asm__("AccelSoTowardMatBounded");
void AccelSoTowardPosSpring(SO *pso, qword a, int b, qword c, void *pv) __asm__("AccelSoTowardPosSpring");
void PresetHackshipAccel(HACKSHIP *ship) __asm__("PresetHackshipAccel");

// vtable slot 70 override. (inferred)
// Also known as: MoveShip (upstream readable / Sly 1 name; MoveShip__FP3ALO).
void PresetHackshipAccel(HACKSHIP *ship) {
    if (ship->state == 3) return;
    MATRIX3 m; m = BuildAxisAngleRotationMatrix(g_normalZ, ship->heading);
    AccelSoTowardMatBounded(ship, &m, D_002BE800);
    AccelSoTowardPosSpring(ship, D_002A2DD0, 0, ship->desiredVelocity, D_002BE810);
}
} // namespace M_func_0017FB50

namespace M_HACKSHIP_TakeDamage {
extern int D_002E1E00 __asm__("D_002E1E00");
int IsSwHandsOff(SW *g) __asm__("IsSwHandsOff");
int TakeHackDamage(HACK *hack, ZPR *pzpr) __asm__("TakeHackDamage");
// vtable slot 75: ignored in states 2-4 / hands-off / global flag; HACK's TakeDamage, update the health bar, game event
// 0xF and state 2 (hit) or 4 (defeated). (inferred)
// Also known as: FAcceptShipContact (upstream readable / Sly 1 name; FAcceptShipContact__FP3ALOP5CTMSG).
extern "C" int TakeHackshipDamage(HACKSHIP *ship, ZPR *pzpr) {
    if (ship->state < 5) { if (ship->state >= 2) return 0; }
    if (IsSwHandsOff(&g_sw)) return 0;
    if (D_002E1E00 & 1) return 0;
    if (!TakeHackDamage(ship, pzpr)) return 0;
    UpdateHackshipHealthBar(ship);
    if (ship->cHealth > 0) { BroadcastGameEvent(&g_game, 0xF, pzpr->ploSource); SetHackshipState(ship, 2); }
    else SetHackshipState(ship, 4);
    return 1;
}
} // namespace M_HACKSHIP_TakeDamage

// Reset health and state (ResetCid47 + SetCid48State + health bar). (inferred)
/* HACKSHIP slot 83: reset. */
// Also known as: ResetShip (upstream readable / Sly 1 name; ResetShip__FP2LO).
extern "C" void ResetHackship(HACKSHIP *ship)
{
    ResetHack(ship);
    if (IsPoActive(ship) != 0) {
        SetHackshipState(ship, 1);
    } else {
        SetHackshipState(ship, 0);
    }
    UpdateHackshipHealthBar(ship);
}

namespace M_func_0017FD08 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");

void PlayWidgetAnim(WIDGET *pwidget, int n) __asm__("PlayWidgetAnim");
CID151 *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(CID151 *psnd, LO *plo) __asm__("SetSndSourceLo");
void RcPtr_Assign(CID151 **pp, CID151 *psnd) __asm__("RcPtr_Assign");
void PauseSnd(CID151 *psnd) __asm__("PauseSnd");
void ReleaseAct(ACT **ppact) __asm__("ReleaseAct");
void SetXfRole(SO *pso, XF *pxf, int k) __asm__("SetXfRole");
void SetLoParent(SO *pso, int a, int b) __asm__("SetLoParent");
void SetSoCnstrForce(SO *pso, int n) __asm__("SetSoCnstrForce");
void SetSoCnstrTorque(SO *pso, int n) __asm__("SetSoCnstrTorque");
void SetSoPartsBit48(SO *pso, int n) __asm__("SetSoPartsBit48");
void SetSmaGoal(SMA *psma, int n) __asm__("SetSmaGoal");
void StopSnd(CID151 *psnd, float f) __asm__("StopSnd");
struct MATRIX4Q GetRelativeXfMatrix(XF *pxfA, XF *pxfB, struct MATRIX4Q *pm) __asm__("GetRelativeXfMatrix");
ACT *NewPositionAct(SO *pso, XF *pxf, int a, int b, int c, qword v) __asm__("NewPositionAct");
qword DecomposeRotationMatrixToEuler(struct MATRIX4Q *pm) __asm__("DecomposeRotationMatrixToEuler");

// State switch (leave/enter): health widget, engine sound, docking actuators, states 0..5. (inferred)
// Also known as: SetShipState (upstream readable / Sly 1 name; SetShipState__FP2LOi).
extern "C" void SetHackshipState(HACKSHIP *ship, int n) {
    int old = ship->state;
    if (n == old) return;
    switch (old) {
    case -1:
        {
            WIDGET *pwidget = ship->pwidgetHealth;
            if (pwidget) {
                pwidget->Show(-1);
                PlayWidgetAnim(ship->pwidgetHealth, 0x34F);
                UpdateHackshipHealthBar(ship);
                if (ship->sfxidEngine != old) {
                    CID151 *psnd = CreateSfxSound(ship->sfxidEngine);
                    SetSndSourceLo(psnd, 0);
                    RcPtr_Assign(&ship->psndEngine, psnd);
                    PauseSnd(ship->psndEngine);
                }
            }
        }
        break;
    case 3:
        ReleaseAct(&ship->pactPos);
        ReleaseAct(&ship->pactRot);
        SetXfRole(ship, ship->positionXf, 2);
        SetXfRole(ship, ship->rotationXf, 2);
        SetLoParent(ship, 0, 0xF);
        SetSoCnstrForce(ship, 5);
        SetSoCnstrTorque(ship, 4);
        SetSoPartsBit48(ship, 0);
        ship->ploDock = 0;
        break;
    case 1:
        {
            int i;
            for (i = 0; i < ship->cpart; i++) {
                if (ship->apart[i].psma) SetSmaGoal(ship->apart[i].psma, 0x66);
            }
        }
        break;
    }
    ship->state = n;
    ship->stateTime = g_clock.time;
    switch (n) {
    case -1:
        {
            WIDGET *pwidget = ship->pwidgetHealth;
            if (pwidget) pwidget->Hide();
            CID151 **ppsnd = &ship->psndEngine;
            if (*ppsnd) {
                StopSnd(*ppsnd, 0.0f);
                RcPtr_Assign(ppsnd, 0);
            }
        }
        break;
    case 3:
        {
            struct MATRIX4Q m;
            XF *pxf;

            pxf = ship->ploDock->GetLocalTransform((MATRIX4 *)&m);
            m = GetRelativeXfMatrix(ship->transforms->parent, ship->positionXf->parent, &m);
            ship->SetParent(ship->ploDock->parent, pxf, 3);
            ship->SetVelocity(D_002A2DD0);
            ship->SetAngularVelocity(D_002A2DD0);
            ship->pactPos = NewPositionAct(ship, ship->positionXf, 1, 7, 2, m.r[3].q);
            ship->pactRot = NewPositionAct(ship, ship->rotationXf, 1, 7, 2, DecomposeRotationMatrixToEuler(&m));
            SetSoCnstrForce(ship, 1);
            SetSoCnstrTorque(ship, 1);
            SetSoPartsBit48(ship, 1);
        }
        break;
    case 4:
        BroadcastGameEvent(&g_game, 0x10, ship);
        break;
    }
    StartHackAnim(ship, n == -1 ? 0 : &ship->ahacke[n]);
}
} // namespace M_func_0017FD08

// Append (int, OID) to a 4-entry table at +0x234 (stride 0x18, count +0x230).
// Also known as: UseStepguardDeathAnimation (upstream readable / Sly 1 name;
//   UseStepguardDeathAnimation__FP9STEPGUARDi3OID).
extern "C" void AddHackshipOidPair(HACKSHIP *ship, int n, OID oid)
{
    unsigned int c = ship->cpart;
    if (c < 4) {
        int *ppair;
        ship->cpart = c + 1;
        ppair = (int *)((char *)ship + (c * 0x18 + 0x234));   /* &apair[c], spelled as the original computes it */
        ppair[0] = n;
        ppair[1] = oid;
    }
}

namespace M_func_001800D8 {
// Set entry i (<5): oid and flags. (inferred)
extern "C" void SetHackshipEntry(HACKSHIP *ship, unsigned int i, OID oid, int grf)
{
    if (i < 5) {
        ship->ahacke[i].oid = oid;
        ship->ahacke[i].grf = grf;
    }
}
} // namespace M_func_001800D8

namespace M_UpdateHackshipHealthBar {
void SetWidgetAnimPhase(WIDGET *pwidget, int anim, float phase) __asm__("SetWidgetAnimPhase");
// Set widget anim 0x34F phase = 1 - health/max (clamped). (inferred)
// Also known as: UpdateHackHud (upstream readable / Sly 1 name; UpdateHackHud__FP2LO).
extern "C" void UpdateHackshipHealthBar(HACKSHIP *ship)
{
    if (ship->pwidgetHealth) {
        int health = ship->cHealth;
        float g;
        if (health <= 0) health = 0;
        g = 1.0f - (float)health / (float)ship->cHealthMax;
        if (g < 0.0f) g = 0.0f;
        else if (g > 1.0f) g = 1.0f;
        SetWidgetAnimPhase(ship->pwidgetHealth, 0x34F, g);
    }
}
} // namespace M_UpdateHackshipHealthBar

// HACKBULLET (cid 0x4A) vtable slot 7: hack-object init, then its damage = 10. (inferred)
void HACKBULLET::Init()
{
    InitHack(this);
    cDamage = 10;
}

namespace M_func_001801B8 {
struct M3 {
 QWROW r[3];
 // Uninitialised 3x3 matrix.
 M3(){}
 // Copy the three rows.
 M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];}
 // Assign the three rows.
 M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;}
};
extern qword g_normalZ __asm__("g_normalZ");
extern void UpdateSo(SO*) __asm__("UpdateSo");
extern qword GetLoWorldPos(LO*) __asm__("GetLoWorldPos");
extern float atan2f(float,float) __asm__("atan2f");
extern float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");
extern float ClampAbs__Fff(float,float) __asm__("ClampAbs__Fff");
extern M3 BuildAxisAngleRotationMatrix(qword,float) __asm__("BuildAxisAngleRotationMatrix");
// Helpers Sub, X, Y, Transform (small asm wrappers/accessors shaped for the match).
static inline qword Transform(const M3&m,qword v){qword r;__asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"j"(v));return r;}
// 3x3 matrix product.
static inline M3 Mul(const M3&a,const M3&b) return r
{
 qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
// vt16: UpdateSo; self-destruct (Kill) when its timer expires; turn toward the target with a limited turn rate. (inferred)
// Also known as: UpdatePiece (upstream readable / Sly 1 name; UpdatePiece__FP2LO).
extern "C" void UpdateHackbullet(HACKBULLET*bullet){
 UpdateSo(bullet);
 if(!bullet->fDying&&bullet->tExpire<=g_clock.time)bullet->Kill(0);
 if(!bullet->ploTarget||bullet->radTurnRate==0.0f)return;
 M3 rotation,matrix;
 qword position;
 position=GetLoWorldPos(bullet->ploTarget);
 qword displacement=VuSub(position,GetLoWorldPos(bullet));
 float desired=atan2f(QwordY(displacement),QwordX(displacement));
 qword direction=bullet->GetVelocity();
 float current=atan2f(QwordY(direction),QwordX(direction));
 float angle=ClampAbs__Fff(NormalizeAngle__Ff(desired-current),bullet->radTurnRate*g_clock.deltaTime);
 rotation=BuildAxisAngleRotationMatrix(g_normalZ,angle);
 bullet->SetVelocity(Transform(rotation,direction));
 // The original reads this local matrix before initialization.
 matrix=Mul(rotation,matrix);
 bullet->RotateToMat((MATRIX3*)&matrix);
}
} // namespace M_func_001801B8

namespace M_func_001803A8 {
int ShouldSoIgnoreIntersection(SO *pso, SO *psoOther) __asm__("ShouldSoIgnoreIntersection");
// vtable slot 61 override: ignore contacts with the tree of the object that fired it. (inferred)
// Also known as: OverlapKindPiece (upstream readable / Sly 1 name; OverlapKindPiece__FP3ALOT0).
extern "C" int ShouldHackbulletIgnoreIntersection(HACKBULLET *bullet, SO *psoOther)
{
    if (bullet->psoOwner && psoOther->physicsTree == bullet->psoOwner->physicsTree)
        return 3;
    return ShouldSoIgnoreIntersection(bullet, psoOther);
}
} // namespace M_func_001803A8

namespace M_func_001803E0 {
struct E { int a; int b; int x8; int xc; char p0[8]; float g18; char pad[0x24]; };   /* an XP contact side (0x40) */
void AdjustHackNewXp(HACK *hack, struct E *ae, int i) __asm__("AdjustHackNewXp");
int IsBasicDerivedFrom__FP5BASIC3CID(int oid, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void StartHackDying(HACK *hack, int n) __asm__("StartHackDying");
// vtable slot 55: HACK's AdjustNewXp, then start dying on contact (with a hack object, or with anything unless it
// bounces); a bouncing bullet gets elasticity 1. (inferred)
// Also known as: OnPieceXpNew (upstream readable / Sly 1 name; OnPieceXpNew__FP3ALOP2XPi).
extern "C" void AdjustHackbulletNewXp(HACKBULLET *bullet, struct E *ae, int i)
{
    struct E *pe;
    int oid;
    AdjustHackNewXp(bullet, ae, i);
    pe = &ae[1 - i];
    oid = pe->xc;
    if (IsBasicDerivedFrom__FP5BASIC3CID(oid, 0x47) || !bullet->fBounce)
        StartHackDying(bullet, oid);
    if (bullet->fBounce) ae[2].g18 = 1.0f;
}
} // namespace M_func_001803E0

namespace M_func_00180478 {
void RefreshSwObjectCollisionFilter(SW *g, SO *pso) __asm__("RefreshSwObjectCollisionFilter");
// Sets the object that fired it (its tree's contacts are ignored, so the collision filter is refreshed when the bullet
// is its tree's root) and restarts the lifetime. (inferred; the symbol name is ours)
extern "C" void SetHackbulletMode(HACKBULLET *bullet, SO *psoOwner)
{
    if (psoOwner != bullet->psoOwner) {
        bullet->psoOwner = psoOwner;
        if (bullet->physicsTree->root == bullet) RefreshSwObjectCollisionFilter(&g_sw, bullet);
    }
    bullet->tExpire = (bullet->dtLifetime == 0.0f) ? 3.40282346638528859812e+38f : g_clock.time + bullet->dtLifetime;
}
} // namespace M_func_00180478

namespace M_InitHackenemy {
void SetHackenemyScale(HACKENEMY *enemy, float g) __asm__("SetHackenemyScale");
void SetHackenemyRange(HACKENEMY *enemy, float g) __asm__("SetHackenemyRange");
// HACKENEMY (cid 0x49) vtable slot 7: InitHack, defaults (speed 250, heading limits -/+FLT_MAX, the speed and heading
// curves' constant/linear terms, scales), entries. (inferred)
extern "C" void InitHackenemy(HACKENEMY *enemy)
{
    InitHack(enemy);
    enemy->clqSpeedDistance.g0 = 1.0f;
    enemy->clqSpeedTurn.g0 = 1.0f;
    enemy->clqHeading.g1 = 1.0f;
    enemy->uSpeed = 250.0f;
    enemy->lmHeading.minValue = -3.40282346638528859812e+38f;
    enemy->lmHeading.maxValue = 3.40282346638528859812e+38f;
    SetHackenemyScale(enemy, 1.0f);
    SetHackenemyRange(enemy, 1.0f);
    InitHackAnimEntries(enemy, 5, enemy->ahacke);
    enemy->state = -1;
}
} // namespace M_InitHackenemy

// vtable slot 12: LISTENER RemoveHierarchy + SetCid49State. (inferred)
/* HACKENEMY slot 12. */
extern "C" void RemoveHackenemyHierarchy(HACKENEMY *enemy)
{
    RemoveHackHierarchy(enemy);
    SetHackenemyState(enemy, -1);
}

namespace M_func_001805C8 {
SHAPE *NewSampledShape(int a, int b) __asm__("NewSampledShape");
void SetXfRole(SO *pso, XF *pxf, int role) __asm__("SetXfRole");
// vtable slot 15: PostHackLoad, the chase path shape, the transforms' roles, bind entries. (inferred)
// Also known as: PostLoadHackEnemy (upstream readable / Sly 1 name; PostLoadHackEnemy__FP2LO).
extern "C" void PostHackenemyLoad(HACKENEMY *enemy)
{
    PostHackLoad(enemy);
    enemy->pshapePath = NewSampledShape(0x2C9, 0x10);
    if (enemy->positionXf) SetXfRole(enemy, enemy->positionXf, 2);
    if (enemy->rotationXf) SetXfRole(enemy, enemy->rotationXf, 2);
    BindHackAnimEntries(enemy, 5, enemy->ahacke);
}
} // namespace M_func_001805C8

// vtable slot 26: base + SetCid49State. (inferred)
/* HACKENEMY slot 26. */
extern "C" void ResetHackenemyState(HACKENEMY *enemy, int n)
{
    ResetHackHealth(enemy, n);
    SetHackenemyState(enemy, 0);
}

namespace M_func_00180668 {
// UpdateHackenemy (Claude agent, lane G19, 2026-10-24): MATCH, no register pins. From laneH9v5 (aligned 3): the case-3 first refresh subtract (target - previousTarget) uses QwSubC, the same vsub with a clobber of the dead vf3. Reload hands out spill regs round-robin over {v0,v1,vf1,vf2,vf3}; after the case 1/2 vsub (target vf1, self vf2) the next VF reload would get vf3; with vf3 clobbered (and prev in vf1) the target reload gets vf2 as in the target.
// history: UpdateHackenemy laneH9v5 (Claude agent lane H9, from laneH6v1): indexed for loop + const-ref loop helpers (R*): the loop has 96 RTL insns in loop pass 2, so loop.c no longer hoists the D_002A2DA0 high. aligned 3.
typedef unsigned long ulong;
typedef union U{qword q;float f[4];int i[4];}U;
struct VU_FLOAT{qword qw;};

extern qword D_002A2DD0 __asm__("D_002A2DD0"),D_002A2DA0 __asm__("D_002A2DA0");
extern float D_002BE820 __asm__("D_002BE820"),D_002BE824 __asm__("D_002BE824"),D_002BE828 __asm__("D_002BE828");
extern CLQ D_002BE830 __asm__("D_002BE830"),D_002BE840 __asm__("D_002BE840"),D_002BE860 __asm__("D_002BE860");
extern LM D_002BE850 __asm__("D_002BE850"),g_lmZeroOne __asm__("g_lmZeroOne");
extern char D_002DE578[] __asm__("D_002DE578");
void UpdateSo(SO*) __asm__("UpdateSo");
int IsHackAnimDone(HACK*) __asm__("IsHackAnimDone");
qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
qword ConvertVecBetweenLos(void*,void*,qword) __asm__("ConvertVecBetweenLos");
qword ConvertXfVec(void*,void*,qword) __asm__("ConvertXfVec");
float atan2f(float,float) __asm__("atan2f");
float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");
int IsWithinLimits__FP2LMf(LM*,float) __asm__("IsWithinLimits__FP2LMf");
void FindPathzonePathCurve(void*,qword,qword,void*) __asm__("FindPathzonePathCurve");
void FindShapeClosestPointFromDistance(void*, qword, void*, qword*, float, qword*, float*, float*) __asm__("FindShapeClosestPointFromDistance");
float GetShapeMaxDistance(void*) __asm__("GetShapeMaxDistance");
float GetDifficulty(void*) __asm__("GetDifficulty");
int FindSwObjectsByClass(void*,int,int,void*,int,SO**) __asm__("FindSwObjectsByClass");
qword LimitVectorLength(qword,float) __asm__("LimitVectorLength");

// Helpers VuFloat, VuFloatFromMemWord, QwSub, QwSubC (small asm wrappers/accessors shaped for the match).
static inline VU_FLOAT VuFloat(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"r"(g));return v;}
// Broadcast the float word n into a VU0 register.
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"rV"(n));return v;}
// Return a - b (vf3 clobbered for allocation).
static inline qword QwSubC(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b):"vf3");return r;} /* clobber of the dead vf3 */
// Helpers QwSqrt, QwDivide, QwAddScaled, QwScaleX, GXOfQw, GXOfVf (small asm wrappers/accessors shaped for the match).
static inline qword QwAddScaled(qword base,qword v,VU_FLOAT g){qword r;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(base),"j"(v),"j"(g.qw));return r;}
// Return v scaled by g.
static inline qword QwScaleX(qword v,VU_FLOAT g){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(g.qw));return r;}
// x lane of qw as a float (keeps it in a VF register).
static inline float GXOfVf(qword qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw),"j"(qw));return x;} /* allocation-only "j": the value stays in a VF register */
// y lane of qw as a float.
static inline float GYOfQw(qword qw){qword r;__asm__("prot3w %0,%1":"=r"(r):"r"(qw));return QwordX(r);}
// Helpers Length, Heading (small asm wrappers/accessors shaped for the match).
static inline float Length(qword v){return QwordX(VuSqrtX(VuDot(v,v)));}
// Heading of v in the XY plane (atan2(y, x)).
static inline float Heading(qword v){return atan2f(GYOfQw(v),QwordX(v));}
// Unit vector of v, or the fallback when its length is below eps.
static inline qword QwNormalizeSafe(qword v,qword fallback,float eps){
 qword r=VuSqrtX(VuDot(v,v));float x;
 __asm__("mtc1 %1,%0":"=f"(x):"r"(r),"r"(fallback)); /* allocation-only "r"(fallback): the fallback is held in a GPR */
 if(x<eps)r=fallback;else r=VuDivByX(v,r);
 return r;}
// Helpers NormalizeInPlace, RSub, RDot, RAddScaled, REvaluate, RLimit, RGXOfQw, RGXOfVf (small asm wrappers/accessors shaped for the match).
static inline qword NormalizeInPlace(U*pv,const qword*pfallback,void*own){float eps=0.0001f;qword g=pv->q;qword v;qword d,one;
 __asm__("":"=j"(v):"0"(g),"r"(own)); /* empty asm: v is a VF copy of the GPR load g; allocation-only "r"(own) keeps p ahead of the rule giv in global-alloc priority */

 __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(v),"j"(v),"r"(g),"f"(eps)); /* allocation-only "r"(v): v is read back through a GPR; "f"(eps): eps is loaded first */
 qword len=VuSqrtX(d);
 if(GXOfVf(len)<eps)pv->q=*pfallback;
 else{qword r;__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(r):"j"(v),"j"(len),"f"(eps));pv->q=r;} /* allocation-only "f"(eps): eps is live into this block, so loop.c leaves its load in the loop */
 return len;}
// Return a - b.
static inline qword RSub(const qword&a,const qword&b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// Dot product of a and b (x lane).
static inline qword RDot(const qword&a,const qword&b){qword r,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=j"(one):"j"(a),"j"(b));return r;}
// Return base + v * g.
static inline qword RAddScaled(const qword&base,const qword&v,const VU_FLOAT&g){qword r;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(base),"j"(v),"j"(g.qw));return r;}
// Evaluate the quadratic a + b*t + c*t^2.
static inline float REvaluate(const CLQ&p,const float&t){return p.g0+t*(p.g1+t*p.g2);}
// Clamp f to [low, high].
static inline float RLimit(float f,const LM&p){float low=p.minValue,high=p.maxValue,r;if(f<low)r=low;else if(high<f)r=high;else r=f;return r;}
// x lane of qw as a float.
static inline float RGXOfQw(const qword&qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw));return x;}
// x lane of qw as a float (keeps it in a VF register).
static inline float RGXOfVf(const qword&qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw),"j"(qw));return x;} /* allocation-only "j": the value stays in a VF register */
// Broadcast g into a VU0 register.
static inline VU_FLOAT RVuFloat(const float&g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"r"(g));return v;}
// Helpers Alive, Evaluate, Limit, Clamp (small asm wrappers/accessors shaped for the match).
static inline ulong Alive(LO*p){return (int)p->isInWorld&1;}
// Evaluate the quadratic a + b*t + c*t^2.
static inline float Evaluate(CLQ*p,float t){return p->g0+t*(p->g1+t*p->g2);}
// Clamp f to [low, high].
static inline float Limit(float f,LM*p){float low=p->minValue,high=p->maxValue,r;if(f<low)r=low;else if(high<f)r=high;else r=f;return r;}
// Clamp f to [low, high].
static inline float Clamp(float f,LM*p){float low=p->minValue,high=p->maxValue;if(f<low)f=low;else if(high<f)f=high;return f;}

// vtable slot 16: UpdateSo; per-state rules: track a live target (aim/turn), switch states, anim completion. (inferred)
// Also known as: UpdateEnemy (upstream readable / Sly 1 name; UpdateEnemy__FP2LO).
extern "C" void UpdateHackenemy(HACKENEMY*enemy){
 U nearest,direction,unit;
 UpdateSo(enemy);
 if(enemy->state==2||enemy->state==4){if(IsHackAnimDone(enemy))SetHackenemyState(enemy,0);}
 if(enemy->state!=0&&enemy->state!=4){if(!enemy->ploTarget||!Alive(enemy->ploTarget))SetHackenemyState(enemy,0);}
 if(!enemy->fHoldState&&enemy->ploTarget&&Alive(enemy->ploTarget)){
  HACKRULE*rule=&enemy->arule[enemy->state];
  qword target=GetLoWorldPos(enemy->ploTarget);
  qword delta=VuSub(target,GetLoWorldPos(enemy));
  float distance=Length(delta);
  float heading=NormalizeAngle__Ff(Heading(ConvertVecBetweenLos(0,enemy,delta)));
  int accept;
  if(!rule->fDistance)accept=rule->fAngle;else accept=IsWithinLimits__FP2LMf(&rule->lmStayDistance,distance);
  if(accept){
   if(!rule->fAngle)goto dispatch;
   if(IsWithinLimits__FP2LMf(&rule->lmStayAngle,heading))goto dispatch;
  }
  {
   int state;
   for(state=4;state>0;state--){
    rule=&enemy->arule[state];
    if(!rule->fDistance)accept=rule->fAngle;else accept=IsWithinLimits__FP2LMf(&rule->lmEnterDistance,distance);
    if(!accept)continue;
    if(!rule->fAngle)break;
    if(IsWithinLimits__FP2LMf(&rule->lmEnterAngle,heading))break;
   }
   SetHackenemyState(enemy,state);
  }
 }
dispatch:
 switch(enemy->state){
 case 0:case 4:break;
 case 1:case 2:{
  if(enemy->fHoldAim)break;
  XF*frame=enemy->rotationXf;
  qword target=GetLoWorldPos(enemy->ploTarget);
  qword self=GetLoWorldPos(enemy);
  float heading=NormalizeAngle__Ff(Heading(ConvertXfVec(0,frame->parent,VuSub(target,self))));
  enemy->heading=Clamp(Evaluate(&enemy->clqHeading,heading),&enemy->lmHeading);
  break;
 }
 case 3:{
  qword target=GetLoWorldPos(enemy->ploTarget);
  qword self=GetLoWorldPos(enemy);
  enemy->GetVelocity();
  int refresh=0;
  if(D_002BE820<Length(QwSubC(target,enemy->posTargetPath)))refresh=1;
  else if(D_002BE820<Length(VuSub(self,enemy->posSelfPath)))refresh=1;
  else if(D_002BE824<g_clock.time-enemy->tPath)refresh=1;
  if(refresh){
   FindPathzonePathCurve(enemy->pvPathzone,self,target,enemy->pshapePath);
   enemy->sPath=0.0f;
   enemy->posTargetPath=target;
   enemy->posSelfPath=self;
   enemy->tPath=g_clock.time;
  }
  FindShapeClosestPointFromDistance(enemy->pshapePath,self,0,&nearest.q,enemy->sPath,&direction.q,0,&enemy->sPath);
  {
   float eps=0.0001f;qword d;
   __asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(QwAddScaled(nearest.q,direction.q,VuFloatFromMemWord(*(int*)&D_002BE828))),"j"(self),"f"(eps)); /* allocation-only "f"(eps): the epsilon is loaded before the subtract */
   unit.q=QwNormalizeSafe(d,D_002A2DD0,eps);
  }
  float remaining=GetShapeMaxDistance(enemy->pshapePath)-enemy->sPath;
  float turn=Heading(ConvertVecBetweenLos(0,enemy,unit.q));
  float response=GetDifficulty(D_002DE578);
  float speed=Limit(Evaluate(&enemy->clqSpeedDistance,remaining),&g_lmZeroOne)*Limit(Evaluate(&enemy->clqSpeedTurn,turn),&g_lmZeroOne)*Limit(Evaluate(&D_002BE830,response),&g_lmZeroOne)*enemy->uSpeed;
  qword force=QwScaleX(unit.q,VuFloat(speed));
  SO*objects[64];
  int n=FindSwObjectsByClass(&g_sw,0x105,0x49,0,64,objects);
  for(int i=0;i<n;i++){
   SO*object=objects[i];
   if(object!=enemy&&object->physicsTree!=enemy->physicsTree){
    U scalar;
    scalar.q=RSub(self,GetLoWorldPos(object));
    scalar.f[2]=0.0f;
    qword len=NormalizeInPlace(&scalar,&D_002A2DA0,enemy);
    float weight=Limit(REvaluate(D_002BE840,RGXOfVf(len)),&D_002BE850);
    weight*=Limit(REvaluate(D_002BE860,RGXOfQw(RDot(scalar.q,unit.q))),&g_lmZeroOne);
    force=RAddScaled(force,scalar.q,RVuFloat(weight));
   }
  }
  enemy->desiredVelocity=LimitVectorLength(force,speed);
  enemy->heading=atan2f(unit.f[1],unit.f[0]);
  break;
 }
 }
}
} // namespace M_func_00180668

namespace M_func_00180EE8 {

extern qword g_normalZ __asm__("g_normalZ");
extern MATRIX3 BuildAxisAngleRotationMatrix(qword v, float g) __asm__("BuildAxisAngleRotationMatrix");
extern MATRIX3 ConvertXfMat(XF *pxf, int n, MATRIX3 *pm) __asm__("ConvertXfMat");
extern void AccelSoTowardMatBounded(SO *pso, MATRIX3 *pm, void *pv) __asm__("AccelSoTowardMatBounded");
extern void AddSoAcceleration(SO *pso, qword v) __asm__("AddSoAcceleration");
// Helpers VSub, Bc (small asm wrappers/accessors shaped for the match).
static inline qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b) : "memory"); return r; }
// Broadcast int bits into a VU0 register.
static inline qword Bc(int g) { qword r; __asm__("qmtc2.ni %1, %0" : "=j"(r) : "r"(g)); return r; }
extern void PresetHackenemyAccel(HACKENEMY *enemy) __asm__("PresetHackenemyAccel");
// vtable slot 70 override. (inferred)
// Also known as: MoveEnemy (upstream readable / Sly 1 name; MoveEnemy__FP3ALO).
void PresetHackenemyAccel(HACKENEMY *enemy)
{
    if (enemy->state != 4) {
        MATRIX3 mat;
        mat = BuildAxisAngleRotationMatrix(g_normalZ, enemy->heading);
        if (enemy->state != 3)
            mat = ConvertXfMat(enemy->rotationXf->parent, 0, &mat);
        AccelSoTowardMatBounded(enemy, &mat, enemy->aScale);
    }
    if (enemy->state == 3) {
        qword v = enemy->desiredVelocity;
        qword w = enemy->GetVelocity();
        qword d, s, r;
        union { qword q; float f[4]; } u;
        d = VSub(v, w);
        {
        s = Bc(*(int *)&enemy->sRange);
        }
        u.q = s;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(d), "j"(s));
        AddSoAcceleration(enemy, r);
    }
}
} // namespace M_func_00180EE8

namespace M_func_00180FF0 {
struct E { int a; int b; int x8; char pad[0x34]; };   /* an XP contact side (0x40) */
extern int D_002DFB80 __asm__("D_002DFB80");
void AdjustHackNewXp(HACK *hack, struct E *ae, int i) __asm__("AdjustHackNewXp");
void StartHackDying(HACK *hack, int n) __asm__("StartHackDying");
// vtable slot 55: base, then start dying when it touches the ship. (inferred)
// Also known as: OnEnemyXpNew (upstream readable / Sly 1 name; OnEnemyXpNew__FP3ALOP2XPi).
extern "C" void AdjustHackenemyNewXp(HACKENEMY *enemy, struct E *ae, int i)
{
    AdjustHackNewXp(enemy, ae, i);
    if (enemy->cDamage) {
        struct E *pe = &ae[1 - i];
        if (pe->x8 == D_002DFB80)
            StartHackDying(enemy, pe->x8);
    }
}
} // namespace M_func_00180FF0

namespace M_func_00181060 {
typedef struct { qword r[3]; } MATRIX3;
struct MSG { ASEGA *pasega; float time; int kind; };   /* message 9: an animation event */
extern void HandlePoMessage(PO *ppo, int msg, struct MSG *pm) __asm__("HandlePoMessage");
extern MATRIX3 BuildAxisAngleRotationMatrix(qword v, float g) __asm__("BuildAxisAngleRotationMatrix");
extern qword GetStepAsegaTranslation(SO *pso, ASEGA *pasega, float time) __asm__("GetStepAsegaTranslation");
extern qword g_normalZ __asm__("g_normalZ");
extern void HandleHackenemyMessage(HACKENEMY *enemy, int msg, struct MSG *pm) __asm__("HandleHackenemyMessage");
// vtable slot 5: HandlePoMessage; on events of its current animation: 0x15E applies a translation step, 0x25..0x28
// set and clear the two hold flags. (inferred)
// Also known as: OnEnemyMessage (upstream readable / Sly 1 name; OnEnemyMessage__FP2LO5MSGIDPv).
void HandleHackenemyMessage(HACKENEMY *enemy, int msg, struct MSG *pm)
{
    HandlePoMessage(enemy, msg, pm);
    if (msg == 9) {
        if (pm->pasega == enemy->pasega || (enemy->psma && pm->pasega == enemy->psma->pasegaCur)) {
            switch (pm->kind) {
            case 0x15E: {
                MATRIX3 m;
                qword v, r;
                m = BuildAxisAngleRotationMatrix(g_normalZ, enemy->heading);
                v = GetStepAsegaTranslation(enemy, pm->pasega, pm->time);
                __asm__("vmulax.xyz ACC, %1, %4x\n\tvmadday.xyz ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
                    : "=&j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(v));
                enemy->desiredVelocity = r;
                enemy->SetVelocity(enemy->desiredVelocity);
                break;
            }
            case 0x25:
                enemy->fHoldAim = 1;
                break;
            case 0x26:
                enemy->fHoldAim = 0;
                break;
            case 0x27:
                enemy->fHoldState = 1;
                break;
            case 0x28:
                enemy->fHoldState = 0;
                break;
            }
        }
    }
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_MATRIX3[(sizeof(MATRIX3)==0x30)?1:-1];
typedef char layout_MSG[(sizeof(MSG)==12)?1:-1];
typedef char layout_MSG_animation[((unsigned long)&((MSG*)0)->pasega==0x0)?1:-1];
typedef char layout_MSG_time[((unsigned long)&((MSG*)0)->time==0x4)?1:-1];
typedef char layout_MSG_kind[((unsigned long)&((MSG*)0)->kind==0x8)?1:-1];
} // namespace M_func_00181060

namespace M_func_001811B8 {
int TakeHackDamage(HACK *hack, ZPR *pzpr) __asm__("TakeHackDamage");
// vtable slot 75: HACK's TakeDamage + state change. (inferred)
// Also known as: FAcceptEnemyContact (upstream readable / Sly 1 name; FAcceptEnemyContact__FP3ALOP5CTMSG).
extern "C" int TakeHackenemyDamage(HACKENEMY *enemy, ZPR *pzpr)
{
    register int four __asm__("$2") = 4; if (enemy->state == four) return 0;
    if (!TakeHackDamage(enemy, pzpr)) return 0;
    if (enemy->cHealth < 0) return 1;
    if (enemy->cHealth <= 0) BroadcastGameEvent(&g_game, 0x18, enemy);
    SetHackenemyState(enemy, 4);
    return 1;
}
} // namespace M_func_001811B8

// Reset health and state. (inferred)
/* HACKENEMY slot 83: reset. */
// Also known as: ResetEnemy (upstream readable / Sly 1 name; ResetEnemy__FP2LO).
void HACKENEMY::Reset()
{
    ResetHack(this);
    SetHackenemyState(this, 0);
}

namespace M_func_00181268 {
struct X { int x; int y; int z; };
// Zeroes the step velocity that animation event 0x15E sets (HandleHackenemyMessage): its x, y, z words.
extern "C" { static __inline__ void ClearStepVelocity(struct X *px) { px->x = px->y = px->z = 0; } }
// State switch; from -1 reset health; start state clip (StartHackAnim). (inferred)
// Also known as: SetEnemyState (upstream readable / Sly 1 name; SetEnemyState__FP2LOi).
extern "C" void SetHackenemyState(HACKENEMY *enemy, int state)
{
    if (enemy->state == state) return;
    if (enemy->state == -1) enemy->cHealth = enemy->cHealthMax;
    enemy->state = state;
    enemy->stateTime = g_clock.time;
    switch (state) {
    case 0: case 1: case 2: case 4:
        ClearStepVelocity((struct X *)&enemy->desiredVelocity);
        break;
    case 3:
        break;
    }
    StartHackAnim(enemy, state == -1 ? 0 : &enemy->ahacke[state]);
}
} // namespace M_func_00181268

namespace M_func_00181308 {
// Set entry i (<5): oid and flags. (inferred)
extern "C" void SetHackenemyEntry(HACKENEMY *enemy, unsigned int i, OID oid, int grf)
{
    if (i < 5) {
        enemy->ahacke[i].oid = oid;
        enemy->ahacke[i].grf = grf;
    }
}
} // namespace M_func_00181308

namespace M_func_00181330 {
// Enable state i's distance test with its enter and stay limits. (inferred)
extern "C" void SetHackenemyEntryVectors(HACKENEMY *enemy, unsigned int i, LM *plmEnter, LM *plmStay)
{
    if (i < 5) {
        HACKRULE *prule = &enemy->arule[i];
        prule->fDistance = 1;
        prule->lmEnterDistance = *plmEnter;
        prule->lmStayDistance = *plmStay;
    }
}
} // namespace M_func_00181330

namespace M_func_00181378 {
// Enable state i's angle test with its enter and stay limits. (inferred)
extern "C" void SetHackenemyEntrySecondVectors(HACKENEMY *enemy, unsigned int i, LM *plmEnter, LM *plmStay)
{
    if (i < 5) {
        HACKRULE *prule = &enemy->arule[i];
        prule->fAngle = 1;
        prule->lmEnterAngle = *plmEnter;
        prule->lmStayAngle = *plmStay;
    }
}
} // namespace M_func_00181378

// Scales the default enemy scale D_002BE870 by g (x and y only). (inferred)
// Also known as: SetEnemyTurnScale (upstream readable / Sly 1 name; SetEnemyTurnScale__FP6HACKSOf).
extern "C" void SetHackenemyScale(HACKENEMY *enemy, float g)
{
    enemy->aScale[0] = D_002BE870[0] * g;
    enemy->aScale[1] = D_002BE870[1] * g;
    enemy->aScale[2] = D_002BE870[2];
}

// Range = g * the default range D_002BE87C. (inferred)
extern "C" void SetHackenemyRange(HACKENEMY *enemy, float g)
{
    enemy->sRange = g * D_002BE87C;
}

// HACKLOCK (cid 0x4B) vtable slot 7: InitCid47 + 3 entries. (inferred)
void HACKLOCK::Init()
{
    InitHack(this);
    InitHackAnimEntries(this, 3, ahacke);
}

// vtable slot 15: PostCid47Load + bind entries. (inferred)
/* HACKLOCK slot 15. */
// Also known as: PostLoadDock (upstream readable / Sly 1 name; PostLoadDock__FP2LO).
extern "C" void PostHacklockLoad(HACKLOCK *lock)
{
    PostHackLoad(lock);
    BindHackAnimEntries(lock, 3, lock->ahacke);
}

namespace M_func_00181478 {
extern void UpdateSo(SO *pso) __asm__("UpdateSo");
extern qword GetLoWorldPos(LO *plo) __asm__("GetLoWorldPos");
extern int IsHackAnimDone(HACK *hack) __asm__("IsHackAnimDone");
extern void SetHacklockState(HACKLOCK *lock, int n) __asm__("SetHacklockState");
extern float D_002BE880 __asm__("D_002BE880");
// Distance between two points.
extern "C" { static __inline__ float GDist(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %1, %1" : "=j"(d) : "j"(d));
    __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "+j"(d));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
} }
// Whether g is below the closeness threshold D_002BE880.
extern "C" { static __inline__ int FClose(float g)
{
    int f = 1;
    if (g < D_002BE880) {} else f = 0;
    return f;
} }
// vtable slot 16: UpdateSo; state 0 waits for the ship singleton D_002DFB80 to come close to the trigger object,
// then state/anim progression. (inferred)
// Also known as: UpdateDock (upstream readable / Sly 1 name; UpdateDock__FP2LO).
extern "C" void UpdateHacklock(HACKLOCK *lock)
{
    int s;
    UpdateSo(lock);
    do {
        s = lock->state;
        switch (s) {
        case 0:
            if (D_002DFB80) {
                if (FClose(GDist(GetLoWorldPos(D_002DFB80), GetLoWorldPos(lock->ploTrigger)))) {
                    lock->pship = D_002DFB80;
                    SetHacklockState(lock, 1);
                }
            }
            break;
        case 1:
            if (IsHackAnimDone(lock))
                SetHacklockState(lock, 2);
            break;
        }
    } while (lock->state != s);
}
} // namespace M_func_00181478

namespace M_func_00181598 {
void SetHacklockState(HACKLOCK *lock, int n) __asm__("SetHacklockState");
void TriggerRsmg(SW *g, int crsmg, OID *arsmg, LO *plo, int fOn) __asm__("TriggerRsmg");
// Reset health + state, and its state-machine records. (inferred)
// Also known as: ResetDock (upstream readable / Sly 1 name; ResetDock__FP2LO).
extern "C" void ResetHacklock(HACKLOCK *lock)
{
    ResetHack(lock);
    SetHacklockState(lock, 0);
    TriggerRsmg(&g_sw, lock->crsmg, lock->arsmg[0], lock, 0);
}
} // namespace M_func_00181598

// State switch; drives the CID 0x48 singleton (SetCid48State) and game events. (inferred)
/* Sets the lock's state; state 1 hands the target to the ship. */
// Also known as: SetDockState (upstream readable / Sly 1 name; SetDockState__FP2LOi).
extern "C" void SetHacklockState(HACKLOCK *lock, int nState)
{
    int nStatePrev = lock->state;
    HACKE *phacke;
    if (nStatePrev != nState) {
        if (nStatePrev == 1 && lock->pship != 0) {
            ActivateHackshipFromHacklock(lock);
        }
        lock->state = nState;
        lock->stateTime = g_clock.time;
        switch (nState) {
        case 1:
            lock->pship->ploDock = lock->ploTrigger;
            SetHackshipState(lock->pship, 3);
            break;
        case 2:
            BroadcastGameEvent(&g_game, 0x39, lock);
            break;
        }
        if (nState != -1) {
            phacke = &lock->ahacke[nState];
        } else {
            phacke = 0;
        }
        StartHackAnim(lock, phacke);
    }
}

namespace M_func_001816C0 {
// Set entry i (<3): oid and flags. (inferred)
extern "C" void SetHacklockEntry(HACKLOCK *lock, unsigned int i, OID oid, int grf)
{
    if (i < 3) {
        lock->ahacke[i].oid = oid;
        lock->ahacke[i].grf = grf;
    }
}
} // namespace M_func_001816C0

namespace M_func_001816E8 {
void AddOrUpdateRsmg__FP2SWP4RSMGiPii3OIDN25(SW *g, OID *arsmg, int crsmgMax, int *pcrsmg, int x, int y, int z, int w) __asm__("AddOrUpdateRsmg__FP2SWP4RSMGiPii3OIDN25");
// AddOrUpdateRsmg on its RSMG table. (inferred)
extern "C" void AddHacklockRsmg(HACKLOCK *lock, int x, int y, int z, int w)
{
    AddOrUpdateRsmg__FP2SWP4RSMGiPii3OIDN25(&g_sw, lock->arsmg[0], 4, &lock->crsmg, x, y, z, w);
}
} // namespace M_func_001816E8

namespace M_func_00181728 {
// Release the ship (SetHackshipState 1) and send Splice event 6. (inferred)
extern "C" void ActivateHackshipFromHacklock(HACKLOCK *lock)
{
    SetHackshipState(lock->pship, 1);
    lock->pship = 0;
    lock->HandleSpliceEvent(6, 0, 0, 0, 0);
}
} // namespace M_func_00181728

namespace M_func_00181780 {
void TriggerRsmg(SW *g, int crsmg, OID *arsmg, LO *plo, int fOn) __asm__("TriggerRsmg");
// TriggerRsmg on its table, then Splice event 5. (inferred)
extern "C" void TriggerHacklockRsmg(HACKLOCK *lock)
{
    TriggerRsmg(&g_sw, lock->crsmg, lock->arsmg[0], lock, 1);
    lock->HandleSpliceEvent(5, 0, 0, 0, 0);
}
} // namespace M_func_00181780

namespace M_InitHackwall {
void InitDl__FP2DLi(DL *pdl, int ib) __asm__("InitDl__FP2DLi");
} // namespace M_InitHackwall
// HACKWALL (cid 0x4C) vtable slot 7: hack-object init, set bit 8 of the LO word +0x40 (as InitHack sets 2) and
// initialise its broken list. (inferred)
void HACKWALL::Init()
{
    using namespace M_InitHackwall;
    uptr v;
    InitHack(this);
    v = (uptr)ploupd;   /* LO's tagged word (lo.h) */
    if ((v ^ 1) & 1) ((LOUPD *)v)->grf |= 8;
    else inlineFlags = v | 8;
    InitDl__FP2DLi(&dlBroken, SLY2_OFFSETOF(HACKWALLPIECE, dle));
}

namespace M_func_00181840 {
/* A SOPART read with its flag word as one 64-bit value (matching-only: so.h's SOPART has a pointer there). */
struct R { XF *pxf; SO *pso; int unknown8; int n; unsigned long fl; char pad[0x98]; };
void InitDl__FP2DLi(DL *pdl, int ib) __asm__("InitDl__FP2DLi");
void *AllocClearedFromWorldHeap(struct SWALLOC *pmsg) __asm__("AllocClearedFromWorldHeap");
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv) __asm__("AppendDlEntry__FP2DLPv");
void AddSoParts(SO *pso) __asm__("AddSoParts");
// vtable slot 15: resolve the segments' shapes (dropping missing ones), allocate their pieces, PostHackLoad, then one
// extra part for the pieces (GetHackwallExtraPartCount). (inferred)
// Also known as: PostLoadHackSwarm (upstream readable / Sly 1 name; PostLoadHackSwarm__FP2LO).
extern "C" void PostHackwallLoad(HACKWALL *wall)
{
    int i, j;
    HACKWALLSEG *pseg;
    struct R *pr;
    for (i = 0; i < wall->cseg; i++) {
        pseg = &wall->aseg[i];
        pseg->pshape = (SHAPE *)FindSwObject(&g_sw, 4, pseg->oid, wall);
        if (!pseg->pshape) {
            i--;
            wall->cseg--;
            continue;
        }
        InitDl__FP2DLi(&pseg->dlPiece, SLY2_OFFSETOF(HACKWALLPIECE, dle));
        for (j = 0; j < pseg->cpiece; j++) {
            struct SWALLOC msg;
            HACKWALLPIECE *ppiece;
            msg.cb = 0x70;
            msg.cbAlign = 0x10;
            ppiece = (HACKWALLPIECE *)AllocClearedFromWorldHeap(&msg);
            ppiece->pseg = pseg;
            ppiece->state = -1;
            AppendDlEntry__FP2DLPv(&pseg->dlPiece, ppiece);
        }
    }
    PostHackLoad(wall);
    pr = &((struct R *)wall->parts)[wall->numParts++];
    pr->pxf = wall->rootTransform;
    pr->n = -1;
    pr->pso = wall;
    pr->fl |= 1UL << 48;
    pr->fl |= 1UL << 49;
    if ((int)wall->isInWorld & 1) AddSoParts(wall);
}
} // namespace M_func_00181840

namespace M_func_001819E0 {
void ClearDle__FP3DLE(DLE *pdle) __asm__("ClearDle__FP3DLE");
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv) __asm__("AppendDlEntry__FP2DLPv");
void ClearDl__FP2DL(DL *pdl) __asm__("ClearDl__FP2DL");
void ResetHackwallSegment(HACKWALL *wall, HACKWALLSEG *pseg) __asm__("ResetHackwallSegment");
// Reset: the broken pieces go back to their segments, then every segment is laid out again. (inferred)
// Also known as: ResetSwarm (upstream readable / Sly 1 name; ResetSwarm__FP2LO).
extern "C" void ResetHackwall(HACKWALL *wall)
{
    HACKWALLPIECE *ppiece;
    int i;
    ResetHack(wall);
    ppiece = (HACKWALLPIECE *)wall->dlBroken.head;
    while (ppiece) {
        HACKWALLPIECE *ppieceNext = (HACKWALLPIECE *)ppiece->dle.next;
        ClearDle__FP3DLE(&ppiece->dle);
        AppendDlEntry__FP2DLPv(&ppiece->pseg->dlPiece, ppiece);
        ppiece = ppieceNext;
    }
    ClearDl__FP2DL(&wall->dlBroken);
    for (i = 0; i < wall->cseg; i++) {
        HACKWALLSEG *pseg = &wall->aseg[i];
        pseg->cpieceActive = pseg->cpiece;
        ResetHackwallSegment(wall, pseg);
    }
}
} // namespace M_func_001819E0

namespace M_func_00181A90 {
void LayoutHackwallPieces(HACKWALL *wall, HACKWALLSEG *pseg) __asm__("LayoutHackwallPieces");
float GetShapeMaxDistance(SHAPE *pshape) __asm__("GetShapeMaxDistance");
// Reset one segment's pieces, spread evenly along its shape. (inferred)
// Also known as: ResetSwarmEnt (upstream readable / Sly 1 name; ResetSwarmEnt__FP2LOP5SWENT).
extern "C" void ResetHackwallSegment(HACKWALL *wall, HACKWALLSEG *pseg)
{
    int i = 0;
    float g;
    HACKWALLPIECE *ppiece;
    LayoutHackwallPieces(wall, pseg);
    g = GetShapeMaxDistance(pseg->pshape);
    for (ppiece = (HACKWALLPIECE *)pseg->dlPiece.head; ppiece; ppiece = (HACKWALLPIECE *)ppiece->dle.next) {
        ppiece->state = -1;
        SetHackwallSegmentState(wall, ppiece, 0);
        ppiece->s = g * ((float)i + 0.5f) / (float)pseg->cpieceActive;
        ppiece->ds = 0;
        ppiece->cHealth = pseg->cPieceHealth;
        i++;
    }
}
} // namespace M_func_00181A90

namespace M_func_00181B60 {
float GetShapeMaxDistance(SHAPE *pshape) __asm__("GetShapeMaxDistance");
// Distribute the pieces along the segment shape's length (GetShapeMaxDistance). (inferred)
// Also known as: LayoutSwarmEnt (upstream readable / Sly 1 name; LayoutSwarmEnt__FP5SWENTPv).
extern "C" void LayoutHackwallPieces(HACKWALL *wall, HACKWALLSEG *pseg)
{
    float g = GetShapeMaxDistance(pseg->pshape);
    int c = pseg->cpieceActive;
    float r = g * 0.5f / (float)c;
    int m;
    float fm, gap, step;
    LM lmS, lmSClosest;
    HACKWALLPIECE *ppiece;
    r = pseg->sRadius < r ? pseg->sRadius : r;
    m = pseg->cpieceMax < c ? pseg->cpieceMax : c;
    fm = (float)m;
    gap = (g - 2.0f * (float)c * r) / (float)(c - m + 1);
    step = gap + 2.0f * r;
    lmS.minValue = (2.0f * fm - 1.0f) * r - step * (float)(m - 1);
    lmS.maxValue = (2.0f * fm - 1.0f) * r + fm * gap - step * (float)(m - 1);
    lmSClosest.minValue = lmS.minValue - fm * r;
    lmSClosest.maxValue = lmS.maxValue + fm * r;
    for (ppiece = (HACKWALLPIECE *)pseg->dlPiece.head; ppiece; ppiece = (HACKWALLPIECE *)ppiece->dle.next) {
        ppiece->lmS = lmS;
        ppiece->lmSClosest = lmSClosest;
        lmS.minValue += step;
        lmS.maxValue += step;
        lmSClosest.minValue += step;
        lmSClosest.maxValue += step;
    }
}
} // namespace M_func_00181B60

namespace M_func_00181CB0 {
float GetShapeMaxDistance(SHAPE *pshape) __asm__("GetShapeMaxDistance");
// Per-frame piece goals along the shape: each piece follows the ship's closest point within its band. (inferred)
// Also known as: TargetSwarmEnt (upstream readable / Sly 1 name; TargetSwarmEnt__FP5SWENTPv).
extern "C" void UpdateHackwallPieces(HACKWALL *wall, HACKWALLSEG *pseg)
{
    float g = GetShapeMaxDistance(pseg->pshape);
    float r0 = g * 0.5f / (float)pseg->cpieceActive;
    HACKWALLPIECE *ppiece;
    int i;
    float r = pseg->sRadius < r0 ? pseg->sRadius : r0;
    for (i = 0, ppiece = (HACKWALLPIECE *)pseg->dlPiece.head; ppiece; ppiece = (HACKWALLPIECE *)ppiece->dle.next, i++) {
        int j = pseg->cpieceActive - i - 1;
        float lo, hi, x;
        float t = (pseg->sClosest - ppiece->lmSClosest.minValue) / (ppiece->lmSClosest.maxValue - ppiece->lmSClosest.minValue);
        if (t < 0.0f) t = 0.0f;
        else if (t > 1.0f) t = 1.0f;
        lo = (2.0f * (float)i + 1.0f) * r;
        hi = g - (2.0f * (float)j + 1.0f) * r;
        x = (1.0f - t) * ppiece->lmS.minValue + t * ppiece->lmS.maxValue;
        if (x < lo) x = lo;
        else if (hi < x) x = hi;
        ppiece->sGoal = x;
        x = (ppiece->lmS.minValue + ppiece->lmS.maxValue) * 0.5f;
        ppiece->negDistanceToShip = -__builtin_fabsf(x - pseg->sClosest);
    }
}
} // namespace M_func_00181CB0

// Iterate segments and advance their states. (inferred)
/* Advances a wall segment's state until it settles. */
extern "C" void UpdateHackwallSegments(HACKWALL *wall, HACKWALLPIECE *ppiece)
{
    int nState;
    do {
        nState = ppiece->state;
        if (nState == 1 && g_clock.time - ppiece->stateTime > D_002BE884) {
            SetHackwallSegmentState(wall, ppiece, 0);
        }
    } while (ppiece->state != nState);
}

namespace M_func_00181EB0 {
void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv) __asm__("RemoveDlEntry__FP2DLPv");
void AppendDlEntry__FP2DLPv(DL *pdl, void *pv) __asm__("AppendDlEntry__FP2DLPv");
void LayoutHackwallPieces(HACKWALL *wall, HACKWALLSEG *pseg) __asm__("LayoutHackwallPieces");
// Piece state switch; a broken piece (2) moves to the wall's broken list (game event 0x3A when its segment has none
// left) and the segment is laid out again. (inferred)
// Also known as: SetSwarmQState (upstream readable / Sly 1 name; SetSwarmQState__FP2LOP3SWQi).
extern "C" void SetHackwallSegmentState(HACKWALL *wall, HACKWALLPIECE *ppiece, int n)
{
    HACKWALLSEG *pseg;
    if (n == ppiece->state) return;
    ppiece->state = n;
    ppiece->stateTime = g_clock.time;
    if (n != 2) return;
    pseg = ppiece->pseg;
    pseg->cpieceActive--;
    RemoveDlEntry__FP2DLPv(&pseg->dlPiece, ppiece);
    AppendDlEntry__FP2DLPv(&wall->dlBroken, ppiece);
    if (!pseg->cpieceActive) BroadcastGameEvent(&g_game, 0x3A, wall);
    LayoutHackwallPieces(wall, pseg);
}
} // namespace M_func_00181EB0

namespace M_func_00181F60 {
struct V { float a[4]; };
extern char D_002BE888[] __asm__("D_002BE888");
extern float D_002BE898 __asm__("D_002BE898"), D_002BE89C __asm__("D_002BE89C"), D_002BE8A0 __asm__("D_002BE8A0"), D_002BE8A4 __asm__("D_002BE8A4");
void UpdateSo(SO *pso) __asm__("UpdateSo");
float GetShapeMaxDistance(SHAPE *pshape) __asm__("GetShapeMaxDistance");
qword GetLoWorldPos(LO *plo) __asm__("GetLoWorldPos");
void FindShapeClosestPoint(SHAPE *pshape, qword q, void *a, qword *b, struct V *pv, float *c, float *pg) __asm__("FindShapeClosestPoint");
void FindShapeClosestPointFromDistance(SHAPE *pshape, qword q, void *a, qword *b, struct V *pv, float *c, float *pg, float g) __asm__("FindShapeClosestPointFromDistance");
float RandomFloatInRange__Fff(float lo, float hi) __asm__("RandomFloatInRange__Fff");
void UpdateHackwallPieces(HACKWALL *wall, HACKWALLSEG *pseg) __asm__("UpdateHackwallPieces");
float SmoothValueWithAccel__FffffP4SMPAPf(void *pv, float *pg, float a, float b, float c, float dt) __asm__("SmoothValueWithAccel__FffffP4SMPAPf");
void EvaluateShapeAtDistance(SHAPE *pshape, void *ppos, void *pdir, float g) __asm__("EvaluateShapeAtDistance");
// One step of constant-acceleration motion: updates velocity *pv by a*dt and position *px by the average velocity.
extern "C" { static __inline__ void Integrate(float *px, float *pv, float a, float dt)
{
    float vOld = *pv;
    float vAvg;
    *pv = vOld + dt * a;
    vAvg = (vOld + *pv) * 0.5f;
    *px += dt * vAvg;
} }
// 0x181F58: dead-code scrap (sly2/scrap.h)
extern "C" { SLY2_SCRAP("daddu $2, $4, $0"); }

// vtable slot 16: UpdateSo, per segment: track the ship's closest point on the shape (searched again at random
// intervals), move the pieces toward their goals and wobble them (springs). (inferred)
// Also known as: UpdateSwarm (upstream readable / Sly 1 name; UpdateSwarm__FP2LO).
extern "C" void UpdateHackwall(HACKWALL *wall)
{
    int i;
    struct V v;
    UpdateSo(wall);
    if (!wall->ploTarget) return;
    for (i = 0; i < wall->cseg; i++) {
        HACKWALLSEG *pseg = &wall->aseg[i];
        SHAPE *pshape = pseg->pshape;
        HACKWALLPIECE *ppiece;
        GetShapeMaxDistance(pshape);
        if (!(g_clock.time <= pseg->tRetarget)) {
            float t;
            FindShapeClosestPoint(pshape, GetLoWorldPos(D_002DFB80), 0, 0, &v, 0, &pseg->sClosest);
            t = g_clock.time;
            pseg->tRetarget = t + RandomFloatInRange__Fff(0.5f, 0.75f);
        } else {
            FindShapeClosestPointFromDistance(pshape, GetLoWorldPos(D_002DFB80), 0, 0, &v, 0, &pseg->sClosest, pseg->sClosest);
        }
        UpdateHackwallPieces(wall, pseg);
        for (ppiece = (HACKWALLPIECE *)pseg->dlPiece.head; ppiece; ppiece = (HACKWALLPIECE *)ppiece->dle.next) {
            float a, b;
            ppiece->s = SmoothValueWithAccel__FffffP4SMPAPf(D_002BE888, &ppiece->ds, ppiece->s, ppiece->ds, ppiece->sGoal, g_clock.deltaTime);
            EvaluateShapeAtDistance(pshape, &ppiece->pos, &ppiece->dir, ppiece->s);
            a = D_002BE898 * ppiece->lateral + D_002BE89C * ppiece->dLateral;
            b = D_002BE8A0 * ppiece->roll + D_002BE8A4 * ppiece->dRoll;
            Integrate(&ppiece->lateral, &ppiece->dLateral, a, g_clock.deltaTime);
            /* the original integrates the roll's speed into the lateral offset too */
            Integrate(&ppiece->lateral, &ppiece->dRoll, b, g_clock.deltaTime);
            UpdateHackwallSegments(wall, ppiece);
        }
    }
}
} // namespace M_func_00181F60

namespace M_func_001821C0 {
// RenderHackwallSelf (Claude agent, lane G4): laneG3v1 with the outer loop as `do { if (count<=0) goto done; top: ... if (i<count) goto top; } while (0);`.
// The do/while(0) gives the real loop notes (alignment nop, fall-in entry) while the goto back edge keeps loop.c from hoisting the 0x30 stride.
// One pin (epsilon in $f1).

typedef union {qword q;float f[4];int i[4];} U;
struct M4 {U r[4];};
extern qword g_normalZ __asm__("g_normalZ");
extern void RenderSoSelf(SO*) __asm__("RenderSoSelf");
extern MATRIX3Q BuildOrthonormalMatrixZ(qword,qword) __asm__("BuildOrthonormalMatrixZ");
extern MATRIX3Q BuildAxisAngleRotationMatrix(qword,float) __asm__("BuildAxisAngleRotationMatrix");
extern qword*VECTOR_opAssign(qword*,qword) __asm__("VECTOR_opAssign");
extern void RenderLoAtMatrix(LO*,M4*,float,float) __asm__("RenderLoAtMatrix");
// Helpers Load, Cross, AddLateral (small asm wrappers/accessors shaped for the match).
static inline qword Load(const qword*p){return *p;}
// Cross product a x b.
static inline qword Cross(qword a,qword b){qword up=a,dir=b;__asm__("vopmula.xyz ACC,%0,%1\n\tvopmsub.xyz %0,%1,%0":"+j"(up):"j"(dir));return up;}
// Return a plus the node's lateral vector scaled by f.
static inline qword AddLateral(qword a,HACKWALLPIECE*n,float f){qword k;U u;__asm__("qmtc2.ni %1,%0":"=j"(k):"r"(f));u.q=k;qword b=Cross(g_normalZ,n->dir);__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(a):"j"(b),"j"(k));return a;}
// 3x3 matrix product.
static inline MATRIX3Q Mul(const MATRIX3Q &a, const MATRIX3Q &b) return r
{
    qword a0 = a.r[0].q, a1 = a.r[1].q, a2 = a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[0].q) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[1].q) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[2].q) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[2].q));
}

// vtable slot 19: RenderSoSelf then draw each segment piece with its own matrix. (inferred)
// Also known as: CollectHackSwarmDraws (upstream readable / Sly 1 name; CollectHackSwarmDraws__FP5SCENE).
extern "C" void RenderHackwallSelf(HACKWALL*wall){
 int i,count;MATRIX3Q m;M4 mm;qword pos;
 RenderSoSelf(wall);
 if(!wall->ploPiece)return;
 count=wall->cseg;i=0;
 do {
  if(count<=0) goto done;
 top:
  {
  HACKWALLPIECE*n=(HACKWALLPIECE*)wall->aseg[i].dlPiece.head;
  while(n){M4*pm=&mm;
   pos=Load(&n->pos);
   m=BuildOrthonormalMatrixZ(n->dir,g_normalZ);
   if(1.0f<__builtin_fabsf(n->lateral))pos=AddLateral(pos,n,n->lateral);
   {float roll=n->roll;register float epsilon __asm__("$f1")=0.01f; /* the one pin */if(epsilon<__builtin_fabsf(roll))m=Mul(m,BuildAxisAngleRotationMatrix(g_normalZ,roll));}
   VECTOR_opAssign(&mm.r[3].q,pos);
   pm->r[0].q=m.r[0].q;pm->r[1].q=m.r[1].q;pm->r[2].q=m.r[2].q;
   mm.r[0].i[3]=mm.r[1].i[3]=mm.r[2].i[3]=0;
   pm->r[3].f[3]=1.0f;
   RenderLoAtMatrix(wall->ploPiece,pm,1.0f,1.0f);
   n=(HACKWALLPIECE*)n->dle.next;
  }
 }

  count=wall->cseg;
  ++i;
  if(i<count) goto top;
 } while(0);
done:;
}
typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Pointer_Size[(sizeof(void*)==4)?1:-1];
typedef char Int_Size[(sizeof(int)==4)?1:-1];
typedef char Float_Size[(sizeof(float)==4)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char U_Align[(__alignof__(U)==16)?1:-1];
typedef char M3_Size[(sizeof(MATRIX3Q)==0x30)?1:-1];
typedef char M3_Align[(__alignof__(MATRIX3Q)==16)?1:-1];
typedef char M4_Size[(sizeof(M4)==0x40)?1:-1];
typedef char M3_row0_Offset[((unsigned)&((MATRIX3Q*)0)->r[0].q==0)?1:-1];
typedef char M3_row1_Offset[((unsigned)&((MATRIX3Q*)0)->r[1].q==0x10)?1:-1];
typedef char M3_row2_Offset[((unsigned)&((MATRIX3Q*)0)->r[2].q==0x20)?1:-1];
typedef char M4_row3_Offset[((unsigned)&((M4*)0)->r[3].q==0x30)?1:-1];
typedef char U_float_View[((unsigned)&((U*)0)->f[0]==0)?1:-1];
typedef char U_int_View[((unsigned)&((U*)0)->i[0]==0)?1:-1];
} // namespace M_func_001821C0

namespace M_func_001823C8 {
// AddHackwallXps: contact generator. From lane H3v1 (no pins); the plane distance's scalar leaves the VU via gcc's qmfc2 plus an mtc1 asm (GX), as in Lilly's GXOfQw.

typedef union U{qword q;float f[4];int i[4];}U;
typedef struct M4{U r[4];}M4;
typedef struct OWNER{char pad0[0x70];qword position;}OWNER;
typedef struct REQUEST{OWNER*owner;int unused4;float radius;int unusedC;long flags;int unused18,start;M4 matrix;char pad60[0x1C];qword*vertices;char pad80[0x10];int count;qword*world;}REQUEST;
typedef struct CONTACT{char pad[0xB0];}CONTACT;   /* the contact list (XP) */
typedef union SEGMENT{LSG lsg;struct{qword a,b,plane;};}SEGMENT;   /* sly2/lsg.h LSG */
typedef struct WORK{U direction,newEndpoint,oldEndpoint;SEGMENT segments[2];U scale,dot,one;}WORK;
extern qword D_002A2DA0 __asm__("D_002A2DA0");
extern void AddSoCustomXps(SO*,REQUEST*,int,int,int,CONTACT**) __asm__("AddSoCustomXps");
extern qword*InitVuTemp(qword*) __asm__("InitVuTemp");
extern int ClipEdgeToSphere(qword,float,qword,qword,int,SEGMENT*) __asm__("ClipEdgeToSphere");
extern void AddXp(void*,SOPART*,REQUEST*,qword,qword,int,HACKWALLPIECE*,void*,CONTACT**,float) __asm__("AddXp");

// Helpers Sub, Broadcast, BroadcastBits, Scalar, MAdd, MAdd2, Point (small asm wrappers/accessors shaped for the match).
extern "C" { static inline qword Sub(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;} }
// Broadcast int bits into a VU0 register.
extern "C" { static inline qword BroadcastBits(int b){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(b));return r;} }
// Return a + b * s.x, computed in a's register.
extern "C" { static inline qword MAdd(qword a,qword b,qword s){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(a):"j"(b),"j"(s));return a;} }
// Return a + b * s.x.
extern "C" { static inline qword MAdd2(qword a,qword b,qword s){qword r;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(a),"j"(b),"j"(s));return r;} }
// Transform point p by the 4x4 matrix m.
extern "C" { static inline qword Point(const M4*m,qword p){qword r=m->r[0].q;__asm__("vmulax.xyz ACC,%0,%4x\n\tvmadday.xyz ACC,%1,%4y\n\tvmaddaz.xyz ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w":"+j"(r):"j"(m->r[1].q),"j"(m->r[2].q),"j"(m->r[3].q),"j"(p));return r;} }
// Length of the direction's projection on the plane (helper of the wall layout).
extern "C" { static inline float Planar(U*p){
 qword direction=p->q;
 qword d,root;
 qword dv; /* allocation-only "=j"(dv) tied to direction: the asm leaves it unchanged, it gives the vdiv a VF copy so direction's GPR dies here */
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(root),"=j"(dv):"2"(direction),"r"(direction),"r"(direction)); /* allocation-only "r": direction is reloaded through a GPR (lq+qmtc2) */
 __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(root):"j"(d));
 if(VuToFloat(root)<0.0001f)p->q=D_002A2DA0;
 else{__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(d):"j"(dv),"j"(root));p->q=d;}
 return VuToFloat(root);
} }
// Lane x as a float.
// Signed distance of a point from a plane, returning the VU temporaries.
extern "C" { static inline float Distance(U*dot,U*one,qword plane,qword point){
 qword d=point,o;
 __asm__("vmul.xyz %0,%2,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddaz.x ACC,%1,%0z\n\tvmsubw.x %0,%1,%2w":"+j"(d),"=&j"(o):"j"(plane));
 one->q=o;dot->q=d;return QwordX(d);
} }
// vtable slot 60: SO base then contacts against each segment (AddXp) for requests with flag bit 49. (inferred)
// Also known as: AddSwarmXps (upstream readable / Sly 1 name; AddSwarmXps__FP3ALOP3SOEiiiPP2XP).
extern "C" void AddHackwallXps(HACKWALL*wall,REQUEST*q,int unused2,int unused3,int unused4,CONTACT**pout){
 WORK w;int i,count;qword center;CONTACT**out=pout;
 AddSoCustomXps(wall,q,unused2,unused3,unused4,pout);
 if(!(q->flags&0x2000000000000L))return;
 center=q->owner->position;
 count=wall->cseg;i=0;
 do{
  if(count<=0)goto done;
 top:
  {
  HACKWALLSEG*entry=&wall->aseg[i];HACKWALLPIECE*n=(HACKWALLPIECE*)entry->dlPiece.head;float radius=q->radius+entry->sRadius;
  for(;n;n=(HACKWALLPIECE*)n->dle.next){
   float distance;
   w.direction.q=Sub(n->pos,center);w.direction.i[2]=0;
   distance=Planar(&w.direction);
   if(distance<radius){
    w.newEndpoint.q=VuFromFloat<qword>(-q->radius);
    AddXp(&g_sw,&wall->parts[wall->numParts-1],q,MAdd(center,w.direction.q,w.newEndpoint.q),w.direction.q,10,n,0,out,distance-radius);
   }else{
    int j;
    for(j=q->start;j<q->count;j++){
     w.newEndpoint.q=Point(&q->matrix,q->vertices[j]);w.oldEndpoint.q=q->world[j];
     w.newEndpoint.f[2]=w.oldEndpoint.f[2]=((U*)&n->pos)->f[2];
     if(ClipEdgeToSphere(n->pos,radius,w.newEndpoint.q,w.oldEndpoint.q,2,w.segments)>0){
      qword plane,point,result;
      w.scale.q=BroadcastBits(*(int*)&q->radius);
      plane=w.segments[0].plane;point=w.oldEndpoint.q;
      result=MAdd2(point,plane,w.scale.q);
      InitVuTemp(&w.dot.q);InitVuTemp(&w.one.q);
      distance=Distance(&w.dot,&w.one,plane,point)-q->radius;
      AddXp(&g_sw,&wall->parts[wall->numParts-1],q,result,plane,10,n,0,out,distance);
      break;
     }
    }
   }
  }
  }
  ++i;
  if(i<wall->cseg)goto top;
 }while(0);
 done:;
}
typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Long_Size[(sizeof(long)==8)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char M4_Size[(sizeof(M4)==0x40)?1:-1];
typedef char CONTACT_Size[(sizeof(CONTACT)==0xB0)?1:-1];
typedef char SEGMENT_Size[(sizeof(SEGMENT)==0x60)?1:-1];
typedef char SegmentBuffer_Size[(sizeof(SEGMENT[2])==0xC0)?1:-1];
typedef char OWNER_position_Offset[((unsigned)&((OWNER*)0)->position==112)?1:-1];
typedef char REQUEST_owner_Offset[((unsigned)&((REQUEST*)0)->owner==0)?1:-1];
typedef char REQUEST_radius_Offset[((unsigned)&((REQUEST*)0)->radius==8)?1:-1];
typedef char REQUEST_flags_Offset[((unsigned)&((REQUEST*)0)->flags==16)?1:-1];
typedef char REQUEST_start_Offset[((unsigned)&((REQUEST*)0)->start==28)?1:-1];
typedef char REQUEST_matrix_Offset[((unsigned)&((REQUEST*)0)->matrix==32)?1:-1];
typedef char REQUEST_vertices_Offset[((unsigned)&((REQUEST*)0)->vertices==124)?1:-1];
typedef char REQUEST_count_Offset[((unsigned)&((REQUEST*)0)->count==144)?1:-1];
typedef char REQUEST_world_Offset[((unsigned)&((REQUEST*)0)->world==148)?1:-1];
typedef char SEGMENT_plane_Offset[((unsigned)&((SEGMENT*)0)->plane==32)?1:-1];
typedef char WORK_direction_Offset[((unsigned)&((WORK*)0)->direction==0)?1:-1];
typedef char WORK_newEndpoint_Offset[((unsigned)&((WORK*)0)->newEndpoint==16)?1:-1];
typedef char WORK_oldEndpoint_Offset[((unsigned)&((WORK*)0)->oldEndpoint==32)?1:-1];
typedef char WORK_segments_Offset[((unsigned)&((WORK*)0)->segments==48)?1:-1];
typedef char WORK_scale_Offset[((unsigned)&((WORK*)0)->scale==240)?1:-1];
typedef char WORK_dot_Offset[((unsigned)&((WORK*)0)->dot==256)?1:-1];
typedef char WORK_one_Offset[((unsigned)&((WORK*)0)->one==272)?1:-1];

} // namespace M_func_001823C8

namespace M_func_00182700 {
struct E { int a; int b; int x8; int xc; char pad[0x30]; };   /* an XP contact side (0x40) */
void AdjustHackNewXp(HACK *hack, struct E *ae, int i) __asm__("AdjustHackNewXp");
int IsBasicDerivedFrom__FP5BASIC3CID(int oid, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void StartHackDying(HACK *hack, int n) __asm__("StartHackDying");
// vtable slot 55: base, then start dying on contact with a hack object. (inferred)
// Also known as: OnSwarmXpNew (upstream readable / Sly 1 name; OnSwarmXpNew__FP3ALOP2XPi).
extern "C" void AdjustHackwallNewXp(HACKWALL *wall, struct E *ae, int i)
{
    AdjustHackNewXp(wall, ae, i);
    if (wall->cDamage) {
        struct E *pe = &ae[1 - i];
        int oid = pe->xc;
        if (IsBasicDerivedFrom__FP5BASIC3CID(oid, 0x47))
            StartHackDying(wall, oid);
    }
}
} // namespace M_func_00182700

// HACKWALL vtable slot 63 (GetExtraPartCount): one extra collision part, for the wall segments. (inferred)
/* HACKWALL slot 63. */
extern "C" int GetHackwallExtraPartCount()
{
    return 1;
}

namespace M_func_00182780 {
void UpdateSoTreeBounds(SO *pso) __asm__("UpdateSoTreeBounds");
void GetShapeBounds(SHAPE *pshape, qword *pa, qword *pb) __asm__("GetShapeBounds");
void ExtendSoTreeBounds(SO *pso, qword q, float g) __asm__("ExtendSoTreeBounds");
// HACKWALL vtable slot 49 (tree bounds update): the SO bounds, extended to cover both ends of every wall segment
// (with the piece radius).
// Also known as: UpdateSwarmBounds (upstream readable / Sly 1 name; UpdateSwarmBounds__FP3ALO).
extern "C" void UpdateHackwallTreeBounds(HACKWALL *wall)
{
    int i;
    UpdateSoTreeBounds(wall);
    for (i = 0; i < wall->cseg; i++) {
        HACKWALLSEG *pseg = &wall->aseg[i];
        qword a, b;
        GetShapeBounds(pseg->pshape, &a, &b);
        ExtendSoTreeBounds(wall, a, pseg->sRadius);
        ExtendSoTreeBounds(wall, b, pseg->sRadius);
    }
}
} // namespace M_func_00182780

namespace M_func_00182810 {
// The piece nearest to a world position. (inferred)
// Also known as: PqNearestSwarm (upstream readable / Sly 1 name; PqNearestSwarm__FP2LOG9VU_VECTOR).
extern "C" HACKWALLPIECE *FindNearestHackwallSegment(HACKWALL *wall, qword pos)
{
    int i;
    HACKWALLPIECE *ppieceBest = 0;
    float gBest = 3.4028235e38f;
    for (i = 0; i < wall->cseg; i++) {
        HACKWALLPIECE *ppiece;
        HACKWALLSEG *pseg = &wall->aseg[i];
        for (ppiece = (HACKWALLPIECE *)pseg->dlPiece.head; ppiece; ppiece = (HACKWALLPIECE *)ppiece->dle.next) {
            qword b;
            register qword a __asm__("vf1");
            register qword one __asm__("vf2");
            float g;
            __asm__("qmtc2.ni %1, %0" : "=j"(b) : "r"(pos));
            a = ppiece->pos;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(a) : "j"(a), "j"(b));
            __asm__("vmul.xyz %0, %1, %1" : "=j"(a) : "j"(a));
            __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "+j"(a));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
            if (g < gBest) {
                gBest = g;
                ppieceBest = ppiece;
            }
        }
    }
    return ppieceBest;
}
} // namespace M_func_00182810

namespace M_func_001828A8 {
extern HACKWALLPIECE *FindNearestHackwallSegment(HACKWALL *wall, qword pos) __asm__("FindNearestHackwallSegment");
extern float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
extern void ExplodeExplParams(EXPL *pexpl, int k, int n, qword a, qword b, float g, float h) __asm__("ExplodeExplParams");
extern float D_002BE8A8 __asm__("D_002BE8A8");
extern qword g_normalZ __asm__("g_normalZ");
extern qword D_002A2DD0 __asm__("D_002A2DD0");
// vtable slot 75: damage the piece nearest the hit; while it holds, kick its wobble (state 1); when it breaks, play
// the explosion there and break it (state 2). (inferred)
// Also known as: FAcceptSwarmContact (upstream readable / Sly 1 name; FAcceptSwarmContact__FP3ALOP5CTMSG).
extern "C" int TakeHackwallDamage(HACKWALL *wall, ZPR *pzpr)
{
    HACKWALLPIECE *ppiece;
    if (!(pzpr->grfzpr & 8))
        return 0;
    ppiece = FindNearestHackwallSegment(wall, pzpr->position);
    if (ppiece) {
        ppiece->cHealth += pzpr->dhp;
        if (ppiece->cHealth > 0) {
            qword c, d, one;
            float g;
            __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(c) : "j"(g_normalZ), "j"(ppiece->dir));
            __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(pzpr->velocity), "j"(c));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
            ppiece->dLateral = g * D_002BE8A8;
            ppiece->dRoll = RandomFloatInRange__Fff(-15.0f, 15.0f);
            SetHackwallSegmentState(wall, ppiece, 1);
        } else {
            if (wall->pexpl)
                ExplodeExplParams(wall->pexpl, 4, 0, D_002A2DD0, ppiece->pos, 0.0f, 0.0f);
            SetHackwallSegmentState(wall, ppiece, 2);
        }
    }
    return 1;
}
} // namespace M_func_001828A8

namespace M_func_001829D8 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
qword GetLoWorldPos(LO *plo) __asm__("GetLoWorldPos");
HACKWALLPIECE *FindNearestHackwallSegment(HACKWALL *wall, qword q) __asm__("FindNearestHackwallSegment");
void func_002333B4(void *pv, int b, int cb) __asm__("memset");
int DeliverZapToSo(ZPR *pzpr) __asm__("DeliverZapToSo");
void ExplodeExplParams(EXPL *pexpl, int a, int b, qword q1, qword q2, float g, float h) __asm__("ExplodeExplParams");
// vtable slot 82: area damage (credited to the killer) at the piece nearest the killer; if it lands, the explosion
// there and the piece breaks. (inferred)
// Also known as: KillSwarm (upstream readable / Sly 1 name; KillSwarm__FP2LOT0).
extern "C" void KillHackwall(HACKWALL *wall, LO *ploKiller)
{
    HACKWALLPIECE *ppiece;
    ZPR zpr;
    if (!ploKiller || !wall->cDamage) return;
    ppiece = FindNearestHackwallSegment(wall, GetLoWorldPos(ploKiller));
    func_002333B4(&zpr, 0, 0x60);
    zpr.unknown08 = ploKiller;
    zpr.ploSource = wall;
    zpr.position = ppiece->pos;
    zpr.zpk = 0x100;
    zpr.dhp = -wall->cDamage;
    if (DeliverZapToSo(&zpr)) {
        if (wall->pexpl) ExplodeExplParams(wall->pexpl, 4, 0, D_002A2DD0, ppiece->pos, 0.0f, 0.0f);
        SetHackwallSegmentState(wall, ppiece, 2);
    }
    wall->fDying = 0;
}
} // namespace M_func_001829D8

namespace M_func_00182AB8 {
extern int D_002BE8AC __asm__("D_002BE8AC");
// Append a segment (max 4): shape oid, piece count, pieces side by side at most, piece radius; default piece health
// (30), no retarget time yet. (inferred)
// Also known as: AddSwarmEntry (upstream readable / Sly 1 name; AddSwarmEntry__FP2LOiiif).
extern "C" void AddHackwallSegment(HACKWALL *wall, OID oid, int cpiece, int cpieceMax, float sRadius)
{
    if ((unsigned int)wall->cseg < 4) {
        HACKWALLSEG *pseg = &wall->aseg[wall->cseg++];
        pseg->oid = oid;
        pseg->cpiece = cpiece;
        pseg->cpieceMax = cpieceMax;
        pseg->sRadius = sRadius;
        pseg->tRetarget = -1.0f;
        pseg->cPieceHealth = D_002BE8AC;
    }
}
} // namespace M_func_00182AB8

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
























HACKSHIP *D_002DFB80;   /* the ship singleton */

// ---- Virtual pointer-to-member constants by address name (docs/guides/tu-merging.md "Data"): 8 bytes {delta 0,
// vtable index, vptr offset 0xC} as the ELF has them; own namespace so no declaration above has to agree.
namespace ANPTR_hack {
} // namespace ANPTR_hack

// ---- hack's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
VECTOR4 D_002BE7D0 = {300.0f, 50.0f, 0.0f, 0.0f};
LM D_002BE7E0 = {0.0f, 800.0f};
float D_002BE7E8 = 1000.0f;
SMPA D_002BE7F0 = {1.0f, 0.0f, 0.5f, 0.0f};
SMPA D_002BE800 = {25.0f, 0.0f, 0.25f, 0.0f};
CLQ D_002BE810 = {{{0.0f, 5.0f, 0.0f, 0.0f}}};
float D_002BE820 = 100.0f;
float D_002BE824 = 5.0f;
float D_002BE828 = 200.0f;
CLQ D_002BE830 = {{{1.0f, -0.5f, 0.0f, 0.0f}}};
CLQ D_002BE840 = {{{1500.0f, -5.0f, 0.0f, 0.0f}}};
LM D_002BE850 = {0.0f, 1000.0f};
CLQ D_002BE860 __attribute__((aligned(16))) = {{{0.0f, -1.0f, 0.0f, 0.0f}}};
float D_002BE870[3] = {25.0f, 0.0f, 0.25f};
float D_002BE87C = 5.0f;
float D_002BE880 = 50.0f;
float D_002BE884 = 0.15f;
SMPA D_002BE888 = {1500.0f, 0.0f, 0.5f, 4500.0f};
float D_002BE898 = -100.0f;
float D_002BE89C = -10.0f;
float D_002BE8A0 = -15.0f;
float D_002BE8A4 = -2.5f;
float D_002BE8A8 = 0.25f;
int D_002BE8AC = 30;
