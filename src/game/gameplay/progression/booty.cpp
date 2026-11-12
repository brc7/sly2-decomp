// booty.cpp -- BOOTY (CID 0x67), the carried booty (docs/systems/progression-and-saves.md section 8): entry table,
// states (idle, in container, transfer, carried, dropped and returning, marker), rendering, and its container
// CID37 (CID 0x25) with the saved container kind. Entry points: the BOOTY and CID37 vtables, the Splice setters
// (AddBootyEntry, AddBootyEntryObject, SetBootyContainerKind) and the world callbacks ReturnDroppedBooty /
// SpawnBootyInContainer. TU 0x13BDF0..0x13D558 (28 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_booty
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_booty
#include "sly2/port.h"   /* uptr */
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/clock.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
#include "sly2/util.h"
#include "sly2/cref.h"
#include "sly2/game.h"
#include "sly2/save.h"
#include "sly2/vu0.h"
#include "sly2/emitter.h"   /* EXPL, EXPLSO */

// 0x13D400 StaticInit_booty and GlobalConstructors_booty: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef union { qword q; float f[4]; } VU;
typedef struct { qword q[4]; } M4;

/* Hand-made virtual calls (slot 1 of the rip, LINKE; one scheduling-pinned SetVelocity): the vtable pointer of an
   object is at +0xC. */
struct VT { VTE a[64]; };

#define CID_34 ((CID)0x34)   /* basic.h's CID has no enumerators yet */
struct MATCH;
struct CID37V;
struct HERO;

/* A world object with a vtable. */
struct RLO { void *scriptFrame; void *scriptVariables; void *firstSubscriber; VT *pvt; };

/* A booty entry (0x2C bytes; at most 0x20, LoadBootyFromBrx copies the used ones to the SW heap). */
struct BOOTYE { int id; int unknown4; char pad8[4]; int hasTimer; int allowedDuringJob; int containerKind; int partsId; unsigned int c1C; OID aoid[3]; };
/* hasTimer: carrying it runs the HUD timer; allowedDuringJob: may be put in a container during a job; containerKind: the
   container kind (CID37V::acceptedKind) it fits; partsId: SO parts shown while it is current, -1 none. (inferred) */
typedef char assert_BOOTYE_size[sizeof(BOOTYE) == 0x2C ? 1 : -1];

struct BOOTYV
{
    void *scriptFrame; void *scriptVariables; void *firstSubscriber;
    VT *pvt;                /* 0x0C */
    void *parent; void *root; char pad18[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback; char pad38[0x3C - 0x38]; void *proxyRecord;
    int taggedFlags;        /* 0x40 tagged flags word */
    void *unknown44; char pad48[0x54 - 0x48]; void *transforms; void *positionXf; void *rotationXf; void *rootTransform; void *unknown64; void *unknown68; char pad6C[0x70 - 0x6C]; void *ploRemovedFirst; void *unknown74; char pad78[0x80 - 0x78]; void *unknown80; char pad84[0xA4 - 0x84]; void *unknownA4; void *unknownA8; char padAC[0xB0 - 0xAC]; void *unknownB0; void *unknownB4; char padB8[0xBC - 0xB8]; void *unknownBC; void *unknownC0; void *unknownC4; void *unknownC8; char padCC[0xD0 - 0xCC]; void *unknownD0; char padD4[0xD8 - 0xD4]; void *unknownD8; void *unknownDC;
    int cpart;              /* 0xE0 */
    char *apart;            /* 0xE4 SOPARTs (0xB0 each) */
    void *physics; void *physicsTree; char padF0[0x100 - 0xF0]; void *unknown100; char pad104[0x134 - 0x104]; void *psobounds; void *pxa; char pad13C[0x144 - 0x13C]; void *prender; char pad148[0x150 - 0x148];
    int cbootye;            /* 0x150 */
    BOOTYE *abootye;        /* 0x154 */
    BOOTYE *pbootyeCur;     /* 0x158 */
    CID37V *pcont;           /* 0x15C container */
    HERO *phero;            /* 0x160 carrier */
    RLO *plo164;            /* 0x164 */
    char pad168[0x170 - 0x168];
    VU vScale;              /* 0x170 */
    float tDrop;            /* 0x180 */
    int st;                 /* 0x184 state */
    float tState;           /* 0x188 time of the last state change */
    MATCH *pmatch;          /* 0x18C */
};
/* AddBootyEntryObject's symbol names the class ALARM (cid_table's old name for BOOTY). */
struct ALARM : BOOTYV { };

/* The container (CID 0x25): a pickup-like object holding at most one booty. */
struct CID37V
{
    void *scriptFrame; void *scriptVariables; void *firstSubscriber;
    VT *pvt;                /* 0x0C */
    void *parent;           /* 0x10 LO::parent */
    void *root; char pad18[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback; char pad38[0x3C - 0x38]; void *proxyRecord; char pad40[0x44 - 0x40]; ASEGBL *unknown44; char pad48[0x4C - 0x48]; void *unknown4C; void *parentTransform; char pad54[0x7C - 0x54]; void *unknown7C; char pad80[0xAC - 0x80]; void *unknownAC; char padB0[0xEC - 0xB0]; void *unknownEC; char padF0[0x100 - 0xF0]; void *unknown100; void *unknown104; void *unknown108; void *unknown10C; char pad110[0x114 - 0x110]; void *relay; char pad118[0x120 - 0x118]; void *unknown120; void *unknown124; void *unknown128; char pad12C[0x130 - 0x12C]; void *unknown130; void *unknown134; char pad138[0x13C - 0x138]; void *unknown13C; char pad140[0x150 - 0x140]; void *unknown150; void *unknown154; char pad158[0x16C - 0x158]; void *unknown16C; char pad170[0x180 - 0x170];
    union { int id; unsigned short idShort; };  /* 0x180 */
    int kind;               /* 0x184 */
    int acceptedKind;       /* 0x188 the booty kind it takes (inferred) */
    float timerSeconds;     /* 0x18C HUD timer started when booty goes in (inferred) */
    int isLocked;           /* 0x190 refuses booty while set (inferred) */
    BOOTYV *pbooty;          /* 0x194 */
    char pad198[4];
};

/* The hero as far as booty goes. */
struct HERO { char pad[0x11A4]; RLO *pr; BOOTYV *pbooty; };

/* The rip that flies the booty between holders. */
struct SA0 { int x0; int x4; float g8; };
struct MATCH
{
    char pad[0x18]; float g18; void *p1C; char pad1[0xA0 - 0x20]; SA0 sA0;
    char pad2[0xEC - 0xAC]; VT *pvt; char pad3[0x104 - 0xF0]; void *p104;
};

struct RANGE { float a; float b; };
struct STACKSCOPEC { int a[4]; };

extern "C" {
extern qword D_002A2DD0;
extern qword D_002A2DE0;
union VQ { float af[4]; qword q; };   /* a 16-byte vector that can be initialized, read as a qword */
extern const VQ D_002A4B60;   /* defined below where the ELF has it (.rodata order) */
extern const float D_002A4B30[3] = {0.0f, 0.0f, 1.5f};
extern RANGE g_lmZeroOne;
extern const LONGU D_002A4B58;
extern const LONGU D_002A4BB0;
extern char D_003DA0C0[];
extern const char D_002A4BB8[];   /* defined below where the ELF has it (.rodata order) */
extern const char D_002A4BC0[];   /* defined below where the ELF has it (.rodata order) */

void InitSo(BOOTYV *);
STACKSCOPEC *STACKSCOPE_ctor(STACKSCOPEC *);
void STACKSCOPE_ctor_1AE4A0(STACKSCOPEC *, int) __asm__("_$_10STACKSCOPE");
void *AllocClearedFromStack(SWALLOC *);
void *AllocCopyFromWorldHeap(SWALLOC *, void *);
void LoadSoFromBrx(BOOTYV *, void *);
void PostSoLoad(BOOTYV *);
void UpdateSo(BOOTYV *);
void DropStepplayerBooty(HERO *);
void RenderSoSelf(BOOTYV *);
qword GetLoWorldPos(void *);
void *GetLoWorldMatrix(void *);
M4 LoadMatrixFromPosRotScale(qword, void *, VU *);
void RenderLoAtMatrix(BOOTYV *, M4 *, float, float);
void HandleBasicMessage(BOOTYV *, int, MATCH *);
int ShouldSoIgnoreIntersection(BOOTYV *, struct XPM *);
void AdjustSoNewXp(BOOTYV *, struct XPE *, int);
void EnsureSwCallback(SW *, void *, long, int);
int CanSoTakeDamage(BOOTYV *, void *);
int IsLoInWorld(CID37V *);
void UnlockProxyObject(void *pv, BOOTYV *p);
void LockProxyObject(void *pv, BOOTYV *p);
void HideSoAllGlobs(BOOTYV *p);
void ShowSoGlobsById(BOOTYV *p, int n);
void HideSoPart(BOOTYV *p, void *pv);
void ShowSoPartsById(BOOTYV *p, int n);
void HideSoPartsById(BOOTYV *p, int n);
void ShowCountdownHud(SW *psw, float g);
void ReplaceHeldSwNoise(SW *psw, int n, qword v, CID37V *pq, void *pv);
void BroadcastGameEvent(void *pv, int n, void *p);
void HideCountdownHud(SW *psw);
void SetSoCnstrForce(BOOTYV *p, int n);
void SetSoCnstrTorque(BOOTYV *p, int n);
void SetSoGravity(BOOTYV *p, qword v);
void SetLoParent(BOOTYV *p, void *pv, int n);
void SetLoXfFromMat(BOOTYV *p, void *pv);
void FadeSoIn(BOOTYV *p, float g);
MATCH *NewRipInGroup(SW *psw, int n, int m);
void SubscribeRipObject(MATCH *pe, BOOTYV *p);
float RandomFloatInRange__Fff(float a, float b);
void InitTriggerGrabspot(CID37V *);
void PostTriggerGrabspotLoad(CID37V *);
void OnTriggerGrabspotAdd(CID37V *);
void FireTriggerGrabspot(CID37V *, HERO *);
BOOTYV *AcquireProxyObject(void *, int);
void PostNotice(void *pv, int n, float g);
void *CreateSfxSound(int n);
void SetSndSourceLo(void *pv, int n);
void UpdateStepplayerBootySave(HERO *q, int n, CID37V *p);
short FindSaveSymbol(void *, const char *);
void *GetLevelListEntry(int);
struct SVKEYS;
void ConcatSaveKeys(void *, SVKEYS *);

void SetBootyState(BOOTYV *p, int st);
void ClearCurrentBootyEntry(BOOTYV *p);
void ReturnDroppedBooty(BOOTYV *p, int);
void SelectBootyEntry(BOOTYV *p, int id);
void SpawnBootyInContainer(CID37V *p, int);
void GetBootyContainerSaveKey(CID37V *p);
/* GetBootyContainerSaveKey leaves ConcatSaveKeys's result (the save key) in v0 (the call is its tail); its callers read it. */
void *PvContSaveKey(CID37V *p) __asm__("GetBootyContainerSaveKey");
int GetSavedBootyContainerKind(CID37V *p);
}
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);    /* IsBasicDerivedFrom__FP5BASIC3CID */
void LaunchMatch(MATCH *pmatch);                    /* LaunchMatch__FP5MATCH */
int GetCurrentLevelIndex();                                    /* GetCurrentLevelIndex__Fv */

// BOOTY (cid 0x67) vtable slot 7: InitSo, default scale vector (+0x170), state -1, set a flag bit. (inferred)
extern "C" void InitBooty(BOOTYV *booty) {
    uptr x;
    InitSo(booty);
    booty->vScale.q = D_002A2DE0; booty->st = -1; x = (uptr)((LO *)booty)->ploupd;   /* LO's tagged word (lo.h) */
    if ((x ^ 1) & 1) ((LOUPD *)x)->grf |= 2;
    else ((LO *)booty)->inlineFlags = x | 2;
}

// Fills an allocation request.
static inline SWALLOC *SetReq(SWALLOC *pr, int cb, int cbAlign) { pr->cb = cb; pr->cbAlign = cbAlign; return pr; }
// BOOTY (cid 0x67) vt14: LoadSoFromBrx into a stack-allocated entry table (0x580), then copy the used entries (0x2C each) to
// SW heap. (inferred)
extern "C" void LoadBootyFromBrx(BOOTYV *booty, void *a) {
    STACKSCOPEC ctx;
    SWALLOC req;
    STACKSCOPE_ctor(&ctx);
    booty->abootye = (BOOTYE *)AllocClearedFromStack(SetReq(&req, 0x580, 0x10));
    LoadSoFromBrx(booty, a);
    ClearCurrentBootyEntry(booty);
    booty->abootye = (BOOTYE *)AllocCopyFromWorldHeap(SetReq(&req, booty->cbootye * 0x2c, 0x10), booty->abootye);
    STACKSCOPE_ctor_1AE4A0(&ctx, 2);
}

// BOOTY (cid 0x67) vtable slot 15: PostSoLoad then SetBootyState(0). (inferred)
// Also known as: PostLoadBooty (upstream readable / Sly 1 name; PostLoadBooty__FP2LO).
void BOOTY::PostLoad() { BOOTYV *booty = (BOOTYV *)this;
    PostSoLoad(booty);
    SetBootyState(booty, 0);
}

// BOOTY (cid 0x67) vtable slot 27: SetBootyState(0). (inferred)
extern "C" void ClearBootyState(BOOTYV *booty) {
    SetBootyState(booty, 0);
}

// cid 0x67 vtable slot 16: UpdateSo; state 5 (dropped) returns to its source after 5 s; state 4 (carried) forces a drop
// (DropStepplayerBooty) once the level timer D_003E1BFC passes. (inferred)
// Also known as: UpdateBooty (upstream readable / Sly 1 name; UpdateBooty__FP2LO).
extern "C" void UpdateBooty(BOOTYV *booty) {
    int st;
    UpdateSo(booty);
    st = booty->st;
    switch (st) {
    case 5:
        if ((g_clock.time - booty->tState) >= 5.0f) {
            ReturnDroppedBooty(booty, 0);
            return;
        }
        return;
    case 4:
        if ((booty->pbootyeCur->hasTimer != 0) && (g_clock.time >= g_sw.hudTimerEndTime)) {
            DropStepplayerBooty(booty->phero);
        }
        break;
    }
}

// Clamps g to [gMin, gMax].
// cid 0x67 vtable slot 19: when carried, scale-pop animation (CLQ over time) and draw at the player; in states 1/5 draw
// normally. (inferred)
// Also known as: FBootyDrawDelta (upstream readable / Sly 1 name; FBootyDrawDelta__FP3ALO).
extern "C" void RenderBootySelf(BOOTYV *booty)
{
    switch (booty->st) {
    case 4: {
        VU v;
        M4 m;
        float dt = g_clock.time - booty->tState;
        if (booty->tDrop <= dt) {
            v = booty->vScale;
        } else {
            float u = dt / booty->tDrop;
            float g = Clamp(D_002A4B30[0] + u * (D_002A4B30[1] + u * D_002A4B30[2]), g_lmZeroOne.a, g_lmZeroOne.b);
            int i;
            for (i = 0; i < 3; i++)
                v.f[i] = (1.0f - g) + g * booty->vScale.f[i];
        }
        M4 mT = LoadMatrixFromPosRotScale(GetLoWorldPos(booty), GetLoWorldMatrix(booty), &v);   /* initialised, not assigned: g++ builds an assigned call result in a temporary first (the C file had no copy) */
        m = mT;
        RenderLoAtMatrix(booty, &m, 1.0f, 1.0f);
        break;
    }
    case 1:
    case 5:
        RenderSoSelf(booty);
        break;
    case 2:
    case 3:
        break;
    }
}

// cid 0x67 vtable slot 5: base; msg 4 while dropped -> state 0; msg 8 from its container -> state 4 (from 2) or 1 (from 3).
// (inferred)
// Also known as: OnBootyMessage (upstream readable / Sly 1 name; OnBootyMessage__FP2LOii).
extern "C" void HandleBootyMessage(BOOTYV *booty, int msg, MATCH *a) {
    HandleBasicMessage(booty, msg, a);
    switch (msg) {
    case 4:
        if (booty->st == 5) SetBootyState(booty, 0);
        break;
    case 8:
        if (a == booty->pmatch) {
            switch (booty->st) {
            case 2: SetBootyState(booty, 4); break;
            case 3: SetBootyState(booty, 1); break;
            }
        }
        break;
    }
}

struct XPO { BASIC *pbasic; };
struct XPM { char pad[0x14]; XPO *pa; };
// BOOTY (cid 0x67) vtable slot 61: while dropped (5) ignore STEPPLAYERs (3), else base. (inferred)
// Also known as: OverlapKindBooty (upstream readable / Sly 1 name; OverlapKindBooty__FP3ALOT0).
extern "C" int ShouldBootyIgnoreIntersection(BOOTYV *booty, XPM *pm) {
    if (booty->st == 5 && IsBasicDerivedFrom(pm->pa->pbasic, CID_34)) return 3;
    return ShouldSoIgnoreIntersection(booty, pm);
}

struct XPE { int f0; int f4; BASIC *pbasic; char pad[0x34]; };
// BOOTY (cid 0x67) vtable slot 55: base, and when dropped and touched by a non-player post callback D_002A4B58. (inferred)
// Also known as: OnBootyXpNew (upstream readable / Sly 1 name; OnBootyXpNew__FP3ALOP2XPi).
extern "C" void AdjustBootyNewXp(BOOTYV *booty, XPE *ae, int i) {
    AdjustSoNewXp(booty, ae, i);
    if (booty->st == 5) { XPE *pe = &ae[1 - i]; if (!IsBasicDerivedFrom(pe->pbasic, CID_34)) EnsureSwCallback(&g_sw, booty, D_002A4B58.l, 0); }
}

// BOOTY (cid 0x67) vtable slot 75: if zap qualifies and dropped, reset to state 0 and return 1. (inferred)
// Also known as: FAcceptBootyContact (upstream readable / Sly 1 name; FAcceptBootyContact__FP3ALO).
extern "C" int TakeBootyDamage(BOOTYV *booty, void *pv) {
    if ((CanSoTakeDamage(booty, pv) == 0) || (booty->st != 5)) {
        return 0;
    }
    SetBootyState(booty, 0);
    return 1;
}

// Splice: append entry {id, a, b} (max 32, 0x2C bytes) and make it current (+0x158). (inferred)
extern "C" void AddBootyEntry(BOOTYV *booty, int id, int unknown4, int containerKind) {
    int c;
    BOOTYE *pe;
    ClearCurrentBootyEntry(booty);
    c = booty->cbootye;
    if (c < 0x20) {
        booty->cbootye = c + 1;
        pe = booty->abootye + c;
        booty->pbootyeCur = pe;
        pe->id = id;
        pe->unknown4 = unknown4;
        pe->containerKind = containerKind;
        pe->partsId = -1;
        pe->c1C = 0;
    }
}

// current entry (+0x158) = 0. (inferred)
void ClearCurrentBootyEntry(BOOTYV *booty) {
    booty->pbootyeCur = 0;
}

// Splice setter: adds an object name (at most three) to the current booty entry. The mangled symbol still carries
// Sly 1's ALARM class name (BOOTY's old label) in its signature: renaming the type is a separate pass.
// Also known as: AddAlarmAlbrk (upstream readable / Sly 1 name).
void AddBootyEntryObject(ALARM *booty, OID oid)
{
    BOOTYE *entry = booty->pbootyeCur;
    if (entry->c1C < 3) entry->aoid[entry->c1C++] = oid;
}

// id of the current entry or -1. (inferred)
// Also known as: IdBooty (upstream readable / Sly 1 name; IdBooty__FP2LO).
extern "C" int GetCurrentBootyEntryId(BOOTYV *booty) {
    BOOTYE *pe;
    int id;
    pe = booty->pbootyeCur;
    id = -1;
    if (pe != 0) {
        id = pe->id;
    }
    return id;
}

// make the entry with this id current (else the first). (inferred)
// Also known as: SelectBootyRecord (upstream readable / Sly 1 name; SelectBootyRecord__FP2LOi).
void SelectBootyEntry(BOOTYV *booty, int id) {
    int i;
    for (i = 0; i < booty->cbootye; i++) {
        if (id == booty->abootye[i].id) { booty->pbootyeCur = &booty->abootye[i]; return; }
    }
    booty->pbootyeCur = booty->abootye;
}

// returns current entry pointer. (inferred)
extern "C" BOOTYE *GetCurrentBootyEntry(BOOTYV *booty) {
    return booty->pbootyeCur;
}

// If dropped: reset to 0 and, if its container is still in world, put it back there (state 1). (inferred)
// Also known as: ReturnThrownBooty (upstream readable / Sly 1 name; ReturnThrownBooty__FP2LO).
void ReturnDroppedBooty(BOOTYV *booty, int) {
    CID37V *pcont;
    BOOTYE *pe;
    if (booty->st == 5) {
        pcont = booty->pcont;
        booty->pcont = 0;
        pe = booty->pbootyeCur;
        SetBootyState(booty, 0);
        if ((pcont != 0) && (IsLoInWorld(pcont) != 0)) {
            booty->pcont = pcont;
            booty->pbootyeCur = pe;
            SetBootyState(booty, 1);
        }
    }
}

// MATCH (Claude agent, laneC43 2026-10-24): v7 + case 5 `pvt = p->pvt` read after the qmtc2 asm, which gets a "memory" clobber (lw then can't fill the mfc1->qmtc2 slot). No register pins.
// v7 (Claude agent, laneC42 2026-10-24): aligned 12 -> 1 with NO register pins. Dist3 ends with the guide's mtc1-from-qword helper `__asm__("mtc1 %1, %0" : "=f"(g) : "r"(d), "f"(k))` (gcc emits the qmfc2 v1 itself, asm cost 1 so max.s follows the mtc1 like the target); the $3 pin and the qmfc2 asm are gone. Left (1): case 5 lw pvt fills the slot between the reload mfc1 and the qmtc2 asm. A "$3" clobber on the qmtc2 asm fixes the order but pvt then takes a2 (with the vmulx "$2") or v0 (without it); pins on pvt/pfn on top: 3-11. The game's mfc1 VU_FLOAT ctor asm: 16-22.
// v6 (Claude agent, laneC33 2026-10-24): aligned 1 -> 12 (WORSE, policy version). The case-3 mtc1 asm is replaced by a pointer pun `return *(float *)&t;` (gcc emits the same mtc1 v1,f1), "f"(k) moved to the qmfc2 asm as the allocation-only operand. gcc's own mtc1 (movsf) has scheduler cost 2, so the lw p->pe again fills the max.s hazard slot, and the p1C lw gets a0 (target v0). union{int;float} 14, float-typed $3 qmfc2 output 14, no "f"(k) 17, "f"(k) on a separate empty asm 26.
// v5 (Claude agent, laneC32 2026-10-24): aligned 11 -> 1. (1) Case 3: its own Dist3/LINKE3 with qmfc2 into an int (ONE register pin, $3 like the target) plus a separate mtc1 asm, which has latency 1 in the scheduler (the reload movsf had 2), so max.s issues right after the mtc1 and the lw p->pe follows it (target keeps the mtc1 hazard nop). An allocation-only "f"(k) (k = the 250.0f GMax limit) on the mtc1 gives 250 f0 / distance f1 like the target. (2) Case 5: `qword vv = *pv` before the qmtc2 (lqc2 before mfc1, mfc1 reuses v0), asm outputs into plain qwords sq/vq copied into the unions afterwards, a pvt local passed as an allocation-only "r"(pvt) to the vmulx (sched1 otherwise sinks the vtable lw below the union stores) and a clobber of the dead v0 on the vmulx (pvt v1, pfn v0). Left: lw pvt fills the mfc1 -> qmtc2 stall (target: qmtc2 right after the mfc1, same reload-latency effect as case 3).
// v4 (Claude agent, laneC6 2026-10-23): 13 -> 11: case 3 writes `p->pe = NewRipInGroup(...); if (p->pe) LINKE(p->pe, pot->pr, p)` (no pe local). Left: case 3 mtc1->max.s hazard nop (our lw p->pe fills it).
// v3 (Claude agent, laneC5 2026-10-23): aligned 46 -> 13: case-4 `pot4 = p->pot` read (gives the s0 copy), Dist sqrt asm split from its qmfc2, LINKE macro as a plain block (do/while(0) added a nop). Left: case 3 LINKE scheduling.
// ~235/598 diffs, mostly offsets after a few small sched diffs (Dist/vsqrt scheduling, GMax regs, loop nops); frame 0x110 and layout match.
struct MSG { int k; void *p; };
typedef void (*PFNQ)(void *, qword);
typedef void (*PFNE)(void *, qword, int, float);
// Distance between two points.
static inline float Dist(qword a, qword b)
{
    qword d;
    float g;
#ifdef SLY2_EE_ASM
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x $vf2, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, $vf2, %0z"
        : "+j"(d) : : "vf2");
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
#else
    d = VuSub(a, b);
    d = VuKeepW(d, VuDot(d, d));
    d = VuSqrtX(d);
    VuCopyBits(g, d);
#endif
    return g;
}
// Distance between two points, at least k (inferred from the use with 250).
static inline float Dist3(qword a, qword b, float k)
{
    qword d;
    float g;
#ifdef SLY2_EE_ASM
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x $vf2, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, $vf2, %0z"
        : "+j"(d) : : "vf2");
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d), "f"(k)); /* mtc1 from a qword (gcc emits the qmfc2); "f"(k): allocation-only operand (Claude agent) */
#else
    d = VuSub(a, b);
    d = VuKeepW(d, VuDot(d, d));
    d = VuSqrtX(d);
    VuCopyBits(g, d);
#endif
    return g;
}
// Larger of two floats.
#define LINKE(PE, PA, PB) { \
    VTE *pvte = &(PE)->pvt->a[1]; \
    float g; \
    ((PFNE)((void (*)())pvte->pfn))((char *)(PE) + pvte->d, GetLoWorldPos(PA), 0, 1.0f); \
    SubscribeRipObject(booty->pmatch, booty); \
    g = Dist(GetLoWorldPos(PA), GetLoWorldPos(PB)); \
    booty->pmatch->p1C = (PB); \
    booty->pmatch->g18 = StdMax(250.0f, g) * 0.002f; \
    { SA0 *ps = &booty->pmatch->sA0; ps->x0 = 0; ps->g8 = -3000.0f; ps->x4 = 0; } \
    booty->pmatch->p104 = (PA); \
    LaunchMatch(booty->pmatch); \
    ((LO *)booty)->Add(); \
}
#define LINKE3(PE, PA, PB) { \
    VTE *pvte = &(PE)->pvt->a[1]; \
    float g; \
    ((PFNE)((void (*)())pvte->pfn))((char *)(PE) + pvte->d, GetLoWorldPos(PA), 0, 1.0f); \
    SubscribeRipObject(booty->pmatch, booty); \
    g = Dist3(GetLoWorldPos(PA), GetLoWorldPos(PB), 250.0f); \
    booty->pmatch->p1C = (PB); \
    booty->pmatch->g18 = StdMax(250.0f, g) * 0.002f; \
    { SA0 *ps = &booty->pmatch->sA0; ps->x0 = 0; ps->g8 = -3000.0f; ps->x4 = 0; } \
    booty->pmatch->p104 = (PA); \
    LaunchMatch(booty->pmatch); \
    ((LO *)booty)->Add(); \
}
// Explodes the effect (EXPL slot 51, ExplodeExplso) with the first two words of an EXPLSO: flags k and the effect
// itself as the reference object (the rest of the record is not read for these flags).
static inline void Notify(RLO *pr, int k)
{
    MSG msg;
    msg.p = pr;
    msg.k = k;
    ((EXPL *)pr)->ExplodeExplso((EXPLSO *)&msg);
}
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_func_0013C4E0() __asm__("ReturnDroppedBooty");
extern const unsigned int r_D_002A4B58[2] __asm__("D_002A4B58") = {0xFFFF0000, (unsigned int)p_func_0013C4E0};
}
namespace RO_ORDER { extern const VQ r_D_002A4B60 __asm__("D_002A4B60") = {{0.0f, 0.0f, -2500.0f, 0.0f}}; }
// BOOTY state machine (-1 inactive .. 6): leaves the old state (unregisters from the world booty manager g_sw+0x4638,
// stops effects) and enters the new one (registers, picks the container or carrier, starts the transfer/drop effects,
// stamps the time). States: 0 idle, 1 in container, 2/3 transfer, 4 carried, 5 dropped and returning, 6 marker
// (inferred from the callers).
// Also known as: SetBootyState (upstream readable / Sly 1 name; SetBootyState__FP2LOi).
void SetBootyState(BOOTYV *booty, int st)
{
    int stOld;
    HERO *pot;
    int i;
    HERO *pot4;
    if (booty->st == st)
        return;
    if (g_sw.bootyProxySource != 0) {
        int fOld = booty->st >= 2;
        int fNew = st >= 2;
        if (fOld) {
            if (!fNew)
                UnlockProxyObject(g_sw.bootyProxySource, booty);
        } else if (fNew) {
            LockProxyObject(g_sw.bootyProxySource, booty);
        }
    }
    stOld = booty->st;
    pot = booty->phero;
    switch (stOld) {
    case -1:
        break;
    case 0:
        HideSoAllGlobs(booty);
        for (i = 0; i < (int)booty->pbootyeCur->c1C; i++)
            ShowSoGlobsById(booty, booty->pbootyeCur->aoid[i]);
        for (i = 0; i < booty->cpart; i++)
            HideSoPart(booty, booty->apart + i * 0xB0);
        if (booty->pbootyeCur->partsId != -1)
            ShowSoPartsById(booty, booty->pbootyeCur->partsId);
        break;
    case 1:
        if (booty->pbootyeCur->hasTimer)
            ShowCountdownHud(&g_sw, booty->pcont->timerSeconds + 0.99f);
        ReplaceHeldSwNoise(&g_sw, 1, GetLoWorldPos(booty->pcont), booty->pcont, (char *)booty->pcont + 0x198);
        if (st == 2)
            BroadcastGameEvent(&g_game, 0x2F, booty);
        booty->pcont->pbooty = 0;
        booty->pcont = 0;
        ((LO *)booty)->HandleSpliceEvent(5, 0, 0, 0, 0);
        break;
    case 2:
        booty->pmatch = 0;
        if (st == 4)
            booty->tDrop = 0.3f;
        break;
    case 3:
        booty->pmatch = 0;
        break;
    case 4:
        pot4 = booty->phero;
        booty->phero = 0;
        ((LO *)pot4->pr)->Remove();
        pot4->pbooty = 0;
        if (booty->pbootyeCur->partsId != -1)
            ShowSoPartsById(booty, booty->pbootyeCur->partsId);
        if (booty->pbootyeCur->hasTimer)
            HideCountdownHud(&g_sw);
        break;
    case 5: {
        RLO *pr = booty->plo164;
        if (pr)
            Notify(pr, 1);
        SetSoCnstrForce(booty, 1);
        SetSoCnstrTorque(booty, 1);
        ((LO *)booty)->SetVelocity(D_002A2DD0);
        ((LO *)booty)->SetAngularVelocity(D_002A2DD0);
        SetSoGravity(booty, D_002A2DD0);
        break;
    }
    case 6:
        booty->tDrop = 0.0f;
        break;
    }
    booty->st = st;
    booty->tState = g_clock.time;
    switch (st) {
    case 0:
        booty->pbootyeCur = 0;
        SetLoParent(booty, 0, 0);
        ((LO *)booty)->Remove();
        break;
    case 1:
        ReplaceHeldSwNoise(&g_sw, -1, D_002A2DD0, 0, (char *)booty->pcont + 0x198);
        SetLoXfFromMat(booty, (void *)((LO *)booty->pcont)->GetWorldMatrix());
        SetLoParent(booty, (void *)booty->pcont->parent, 3);
        booty->pcont->pbooty = booty;
        if (stOld != 3) {
            ((LO *)booty)->Remove();
            FadeSoIn(booty, 0.5f);
        }
        ((LO *)booty)->HandleSpliceEvent(6, 0, 0, 0, 0);
        break;
    case 2:
    case 6:
        ((LO *)booty->phero->pr)->Add();
        if (booty->st == 2) {
            booty->pmatch = NewRipInGroup(&g_sw, 6, 0);
        } else {
            booty->pmatch = 0;
            ((LO *)booty)->HandleSpliceEvent(6, 0, 0, 0, 0);
            ((LO *)booty)->HandleSpliceEvent(5, 0, 0, 0, 0);
        }
        {
            MATCH *pe = booty->pmatch;
            if (pe)
                LINKE(pe, booty, booty->phero->pr);
        }
        SetLoXfFromMat(booty, (void *)((LO *)booty->phero->pr)->GetWorldMatrix());
        SetLoParent(booty, booty->phero->pr, 3);
        if (!booty->pmatch)
            SetBootyState(booty, 4);
        break;
    case 3: {
        SetLoXfFromMat(booty, (void *)((LO *)booty->pcont)->GetWorldMatrix());
        SetLoParent(booty, (void *)booty->pcont->parent, 3);
        booty->pmatch = NewRipInGroup(&g_sw, 6, 0);
        if (booty->pmatch)
            LINKE3(booty->pmatch, pot->pr, booty);
        if (!booty->pmatch)
            SetBootyState(booty, 1);
        break;
    }
    case 4:
        booty->phero->pbooty = booty;
        if (booty->pbootyeCur->partsId != -1)
            HideSoPartsById(booty, booty->pbootyeCur->partsId);
        break;
    case 5: {
        union { qword q; float f[4]; } v;
        union { qword q; float f[4]; } u;
        union { qword q; float f[4]; } s;
        union { qword q; float f[4]; } w;
        float a, b, c;
        float lo = -6.2831855f, hi = 6.2831855f;
        (void)&w;
        SetLoParent(booty, 0, 3);
        SetSoCnstrForce(booty, 0);
        SetSoCnstrTorque(booty, 0);
        {
            qword *pv = (qword *)((LO *)booty)->GetWorldMatrix();
            qword vv = *pv;
            VT *pvt;
            qword sq, vq;
#ifdef SLY2_EE_ASM
            __asm__("qmtc2.ni %1, %0" : "=j"(sq) : "r"(1000.0f) : "memory"); /* "memory" clobber (Claude agent): with pvt read after this asm, keeps the vtable lw after the qmtc2 like the target */
#else
            sq = VuWiden<__typeof__(sq)>(1000.0f);
#endif
            pvt = booty->pvt;
#ifdef SLY2_EE_ASM
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(vq) : "j"(vv), "j"(sq), "r"(pvt) : "$2"); /* "r"(pvt): allocation-only (Claude agent), keeps the vtable load before the vmulx like the target; "$2": clobber of the dead v0 (Claude agent), so pvt takes v1 like the target */
#else
            vq = VuScaleByX(vv, sq);
#endif
            s.q = sq;
            v.q = vq;
#if defined(SLY2_EE_LAYOUT)
            ((PFNQ)((void (*)())pvt->a[35].pfn))((char *)booty + pvt->a[35].d, v.q);
#else
            ((SO *)booty)->SetVelocity(v.q);   /* slot 35 */
#endif
        }
        a = RandomFloatInRange__Fff(lo, hi);
        b = RandomFloatInRange__Fff(lo, hi);
        c = RandomFloatInRange__Fff(lo, hi);
        u.f[0] = a;
        u.f[1] = b;
        u.f[2] = c;
        ((LO *)booty)->SetAngularVelocity(u.q);
        SetSoGravity(booty, D_002A4B60.q);
        BroadcastGameEvent(&g_game, 0x30, booty);
        break;
    }
    }
}

// cid 0x25 vtable slot 7: base init 0x183F08, default kind 0x2BC, -1, 15.0. (inferred)
extern "C" void InitBootyContainer(CID37V *container) {
    InitTriggerGrabspot(container);
    container->kind = 0x2BC;
    container->acceptedKind = -1;
    container->timerSeconds = 15.0f;
}

// cid 0x25 vtable slot 15: base 0x183F50, unique id from a g_sw counter. (inferred)
void CID37::PostLoad() { CID37V *container = (CID37V *)this; PostTriggerGrabspotLoad(container); container->id = g_sw.nextBootyContainerId++; }

// If none yet and the manager exists: look up saved kind (0x13D248), clone a booty from the template, select its
// entry, link and set state 1. (inferred)
// Also known as: AttachStandBooty (upstream readable / Sly 1 name; AttachStandBooty__FP2LO).
void SpawnBootyInContainer(CID37V *container, int) {
    SW *pg;
    int i;
    BOOTYV *pa;
    if (container->pbooty) return;
    pg = &g_sw;
    if (!pg->bootyProxySource) return;
    i = GetSavedBootyContainerKind(container);
    if (i == -1) return;
    pa = AcquireProxyObject(pg->bootyProxySource, 0);
    container->pbooty = pa;
    if (pa) {
        SelectBootyEntry(pa, i);
        container->pbooty->pcont = container;
        SetBootyState(container->pbooty, 1);
    }
}

// cid 0x25 vtable slot 23: base 0x183FB8 then EnsureSwCallback(D_002A4BB0). (inferred)
// Also known as: OnBootyStandRegistered (upstream readable / Sly 1 name; OnBootyStandRegistered__FP2LO).
extern "C" void OnBootyContainerAdd(CID37V *container) { OnTriggerGrabspotAdd(container); EnsureSwCallback(&g_sw, container, D_002A4BB0.l, 0); }

// cid 0x25 vtable slot 51: when a STEPPLAYER touches: refuse (message 0x124, sound 0x36) if locked, broadcast event 0x32 on
// mismatch, else swap booty between player and container (UpdateStepplayerBootySave case 0) and sound 0x27.
// (inferred)
// Also known as: FUseBootyStand (upstream readable / Sly 1 name; FUseBootyStand__FP2LOP5ACTOR).
extern "C" void OnBootyContainerTouched(CID37V *container, HERO *q)
{
    BOOTYV *pqa, *ppa;
    if (!IsBasicDerivedFrom((BASIC *)q, CID_34))
        return;
    pqa = q->pbooty;
    ppa = container->pbooty;
    if (!pqa && !ppa)
        return;
    if (ppa && g_sw.currentJob && !ppa->pbootyeCur->allowedDuringJob) {
        PostNotice(D_003DA0C0, 0x124, 15.0f);
        SetSndSourceLo(CreateSfxSound(0x36), 0);
        return;
    }
    if (container->isLocked || (pqa && pqa->pbootyeCur->containerKind != container->acceptedKind)) {
        BroadcastGameEvent(&g_game, 0x32, container);
        return;
    }
    if (container->pbooty)
        SetSndSourceLo(CreateSfxSound(0x27), 0);
    UpdateStepplayerBootySave(q, 0, container);
    if (q->pbooty && container->pbooty) {
        q->pbooty->pcont = container;
        SetBootyState(q->pbooty, 3);
        container->pbooty->phero = q;
        SetBootyState(container->pbooty, 2);
    } else if (q->pbooty) {
        q->pbooty->pcont = container;
        SetBootyState(q->pbooty, 3);
    } else {
        container->pbooty->phero = q;
        SetBootyState(container->pbooty, 2);
    }
    FireTriggerGrabspot(container, q);
}

struct SVKEYS { int c; short *as; };
// Build the save key for this container (level entry + two symbols + id). (inferred)
// Also known as: AkeyStand (upstream readable / Sly 1 name; AkeyStand__FP2LO).
void GetBootyContainerSaveKey(CID37V *container) {
    short as[3];
    SVKEYS par;
    int i;
    int v;
    i = 0;
    v = GetCurrentLevelIndex();
    if (v == -1) v = i;
    i = v;
    char *pv = (char *)&g_sw.savevars;
    pv += 8;
    as[0] = FindSaveSymbol(pv, D_002A4BB8);
    short r = FindSaveSymbol(pv, D_002A4BC0);
    register int t __asm__("$3") = container->idShort;
    register int c __asm__("$5") = 3;
    par.c = c;
#ifdef SLY2_EE_ASM
    __asm__("" : : : "memory");
#endif
    as[1] = r;
    as[2] = t;
    par.as = as;
    ConcatSaveKeys(GetLevelListEntry(i), &par);
}

extern "C" { void Decref__4CRef(void *); }
extern "C" {
void GetGlobalSaveVar(CRefData *, void *, void *);
CRef SetGlobalSaveVar(void *, const CRef &, void *);
}
// Reads an integer save variable through a CRef.
static inline int NFromRef(void *pv)
{
    CRefData ref;
    int n;
    GetGlobalSaveVar(&ref, pv, &g_gs);
    n = ref.u.n;
    Decref__4CRef(&ref);
    return n;
}
// Read saved kind for this container (default +0x184); remap 0x2BF..0x2C1 -> 0x2BC..0x2BE, 7..9 -> -1.
// Also known as: IdStandSaved (upstream readable / Sly 1 name; IdStandSaved__FP2LO).
int GetSavedBootyContainerKind(CID37V *container) {
    STACKSCOPEC ctx;
    int n;
    STACKSCOPE_ctor(&ctx);
    n = NFromRef(PvContSaveKey(container));
    if (n == 0) n = container->kind;
    else if (n >= 0) { if (n < 10) { if (n >= 7) n = -1; } }
    switch (n) {
    case 0x2bf: n = 0x2bc; break;
    case 0x2c0: n = 0x2bd; break;
    case 0x2c1: n = 0x2be; break;
    }
    STACKSCOPE_ctor_1AE4A0(&ctx, 2);
    return n;
}

// Stores a CRef into a global save variable.
static inline void SetPv(void *pv, const CRef &r, void *) { CRef arg(r); const CRef &ret = SetGlobalSaveVar(pv, arg, &g_gs); }
// Splice: reset current booty, save kind n, and spawn if n != -1. (inferred)
// Also known as: SetStandBooty (upstream readable / Sly 1 name; SetStandBooty__FP2LOi).
extern "C" void SetBootyContainerKind(CID37V *container, int n)
{
    if (container->pbooty) SetBootyState(container->pbooty, 0);
    STACKSCOPE frame;
    {
        void *po = PvContSaveKey(container);
        CRef ref(n, 0x15);
        SetPv(po, ref, &g_gs);
    }
    if (n != -1) SpawnBootyInContainer(container, 0);
}

// ---- Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_booty {
void p_func_0013C4E0() __asm__("ReturnDroppedBooty");
void p_func_0013CF38() __asm__("SpawnBootyInContainer");

extern const unsigned int D_002A4BB0[2] __asm__("D_002A4BB0") = {0xFFFF0000, (unsigned int)p_func_0013CF38};

} // namespace ANPTR_booty

namespace RO_ORDER { extern const char r_D_002A4BB8[] __asm__("D_002A4BB8") = "booty"; }
namespace RO_ORDER { extern const char r_D_002A4BC0[] __asm__("D_002A4BC0") = "aoidAssembly"; }