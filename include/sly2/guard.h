/* include/sly2/guard.h -- STEPGUARD (CID 0x38) and the guard root behaviour BHV156 (CID 0x9C).
 * Written by hand in types_round2 from runtime findings (runtime, runtime2, runtime4, runtime7a FINDINGS; devtools/ram_inspector
 * guard decoder). Only offsets with evidence are named; the rest is padding. Tags: [R] runtime-confirmed (offset and meaning
 * checked in save-state RAM), [S] static, [H] hypothesis. No src/ file includes this header yet. */
#ifndef SLY2_GUARD_H
#define SLY2_GUARD_H
#include "sly2/basic.h"
#include "sly2/step.h"
#include "sly2/xfm.h"

/* one pickpocket-able loot entry carried by a guard (16 bytes) */
struct GUARDLOOT
{
      OID oid;              /* +0x0 [R] loot object oid */
      undefined4 unk_0x4[3];  /* [?] */
};

/* DETECTOR (CID 0x10, cb 0x180): a guard's vision detector. Only the fields the option accessors below use. [S] */
struct DETECTOR : public XFM
{
      undefined4 unk_0xe0[2];
      VOL *pvolNear;        /* +0xE8 [S] near volume (tu_1B9F80's view; VOL type [H]) */
      VOL *pvolFar;         /* +0xEC [S] far volume */
      undefined4 unk_0xf0[(0xfc - 0xf0) / 4];
      int cUnkFC;           /* +0xFC [S] a total the guard's +0x3A4 contribution is added to (stepguard.cpp func_002087C8) */
      undefined4 unk_0x100[(0x180 - 0x100) / 4];
    virtual CLASSINFO *Pclassinfo() __asm__("GetDetectorClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitDetector");   /* slot 7 */
    virtual void VUnk15() __asm__("UpdateDetector");   /* slot 16 */
};
typedef char assert_DETECTOR_size[sizeof(DETECTOR) == 0x180 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_DETECTOR_cUnkFC[SLY2_OFFSETOF(DETECTOR, cUnkFC) == 0xFC ? 1 : -1];
typedef char assert_DETECTOR_pvolNear[SLY2_OFFSETOF(DETECTOR, pvolNear) == 0xE8 ? 1 : -1];

/* An animation binding of a guard (16 bytes; stepguard.cpp ResolveAnimBinding/ReleaseAnimBinding): its kind (2 and 3
   hold a handle released by func_0020C030 / func_0020CA20; -1 none), the handle, and two words. [S] */
struct ANIMBINDING
{
      int kind;             /* +0x0 [S] */
      int h;                /* +0x4 [S] */
      int n8;               /* +0x8 [S] */
      int nC;               /* +0xC [S] */
};
/* A guard's general animation binding record (32 bytes): the binding and 16 more bytes. [S] */
struct ANIMBINDINGREC
{
      ANIMBINDING binding;  /* +0x0 [S] */
      undefined4 unk_0x10[4];
};

/* STEPGUARD: class descriptor cb 0x1010, vtable 89 entries (types/cid_table.csv, CONFIRMED runtime) */
struct STEPGUARD : public STEP
{
      unsigned long grfsg;          /* +0x2C0 [R] guard flag word == union of the flags of the behaviours on bhvstk (all
                                       guards in all states, ram_inspector check). 64-bit: on ee-gcc long = 8 bytes, long long = 16 */
      undefined4 unk_0x2c8[(0x330 - 0x2c8) / 4];
      int fKO;              /* +0x330 [R] knocked out */
      undefined4 unk_0x334;
      int hpMax;            /* +0x338 [R] max hp */
      int hp;               /* +0x33C [R] current hp */
      undefined4 unk_0x340[(0x34c - 0x340) / 4];
      ASEGA *pasega;        /* +0x34C [S] an animation the guard holds (released with ReleaseAsega(&pasega)) */
      undefined4 unk_0x350[(0x38c - 0x350) / 4];
      LO *ploTarget;        /* +0x38C [R] current target (Sly while chasing) */
      int nUnk390;          /* +0x390 [S] */
      int nUnk394;          /* +0x394 [S] cleared with nUnk3A4 */
      undefined4 unk_0x398[(0x3a4 - 0x398) / 4];
      int cUnk3A4;          /* +0x3A4 [S] the guard's contribution to pdetector->cUnkFC */
      int nUnk3A8;          /* +0x3A8 [S] */
      undefined4 unk_0x3ac[(0x3c0 - 0x3ac) / 4];
      int nUnk3C0;          /* +0x3C0 [S] */
      undefined4 unk_0x3c4[(0x3cc - 0x3c4) / 4];
      void *pvEngage;       /* +0x3CC [R] engagement record (non-zero while in combat) [H type] */
      undefined4 unk_0x3d0[(0x404 - 0x3d0) / 4];
      GUARDLOOT aloot[8];   /* +0x404 [R] pickpocket loot list */
      int cloot;            /* +0x484 [R] entries used in aloot */
      undefined4 unk_0x488[(0x4c0 - 0x488) / 4];
      int cbindingrec;      /* +0x4C0 [S] entries used in abindingrec */
      undefined4 unk_0x4c4[3];
      ANIMBINDINGREC abindingrec[32];   /* +0x4D0 [S] */
      ANIMBINDING aabindingZap[3][26];  /* +0x8D0 [S] three zap-reaction binding sets (ResolveZapReactions) */
      undefined4 unk_0xdb0[(0xf04 - 0xdb0) / 4];
      float tAttackNext;    /* +0xF04 [S] earliest time of the next attack choice (guardbhv FBhvcombatChooseAttack) */
      undefined4 unk_0xf08[(0xf3c - 0xf08) / 4];
      int fUnkF3C;          /* +0xF3C [S] set by StartBhvstrike */
      undefined4 unk_0xf40[(0xf60 - 0xf40) / 4];
      int nFiringWindow;    /* +0xF60 [R] firing window (shooting guards) */
      DETECTOR *pdetector;  /* +0xF64 [R] vision detector (CID 0x10; its +0xE0 owner == this guard) */
      int nUnkF68;          /* +0xF68 [S] copied with qwUnkF70 into the root behaviour's sub-behaviour in mode 0x10 */
      undefined4 unk_0xf6c;
      qword qwUnkF70;       /* +0xF70 [S] */
      undefined4 unk_0xf80[(0xf94 - 0xf80) / 4];
      int cUnkF94;          /* +0xF94 [S] FBhvguardHasTargetList: non-zero with cUnkFA8 means a target list */
      undefined4 unk_0xf98[(0xfa8 - 0xf98) / 4];
      int cUnkFA8;          /* +0xFA8 [S] */
      undefined4 unk_0xfac[(0xfe0 - 0xfac) / 4];
      DLE dleGuard;         /* +0xFE0 [R] link in the in-world guard list (DL at 0x3E1444, ibDle 0xFE0) */
      uint grfCapability;   /* +0xFE8 [R] capability bits (bit 3 = may shoot) */
      undefined4 unk_0xfec[(0xffc - 0xfec) / 4];
      int cBusy;            /* +0xFFC [R] busy count */
      undefined4 unk_0x1000[4];
    virtual CLASSINFO *Pclassinfo() __asm__("GetStepguardClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleStepguardMessage");   /* slot 5 */
    virtual void Init() __asm__("InitStepguard");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveStepguardHierarchy");   /* slot 12 */
    virtual void VUnk14() __asm__("PostStepguardLoad");   /* slot 15 */
    virtual void VUnk15() __asm__("UpdateStepguard");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderStepguardSelf");   /* slot 19 */
    virtual void Freeze() __asm__("OnStepguardRemove");   /* slot 22 */
    virtual void Unfreeze() __asm__("OnStepguardAdd");   /* slot 23 */
    virtual void VUnk25(LO *plo) __asm__("ResetStepguardToHome");   /* slot 26 */
    virtual void VUnk26(LO *plo) __asm__("ClearStepguardState");   /* slot 27 */
    virtual void Clone(LO *ploBase) __asm__("CloneStepguard");   /* slot 41 */
    virtual void VUnk58() __asm__("func_00209C88");   /* slot 59 */
    virtual int FIgnoreIntersection(SO *psoOther) __asm__("FIgnoreStepguardIntersection");   /* slot 61 */
    virtual int FTakeDamage(ZPR *pzpr) __asm__("FTakeStepguardDamage");   /* slot 75 */
    virtual int FInvulnerable(ZPK zpk) __asm__("FInvulnerableStepguard");   /* slot 76 */
    virtual int JthsCurrent() __asm__("JthsCurrentStepguard");   /* slot 77 */
    virtual int VUnk84() __asm__("func_0020B090");   /* slot 85 */
    virtual int VUnk85(ZPR *pzpr) __asm__("func_00207530");   /* slot 86 */
    virtual void VUnk86(int fOn) __asm__("func_001F1F38");   /* slot 87 */
    virtual void VSlot87() __asm__("PloChooseStepguardTarget");   /* slot 87: not yet identified (STEPGUARD adds 2 slots to STEP's 87) */
    virtual void VSlot88() __asm__("FStepguardSenseStimulus");   /* slot 88: not yet identified */
};
typedef char assert_STEPGUARD_size[sizeof(STEPGUARD) == 0x1010 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_STEPGUARD_grfsg[SLY2_OFFSETOF(STEPGUARD, grfsg) == 0x2C0 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_hp[SLY2_OFFSETOF(STEPGUARD, hp) == 0x33C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_ploTarget[SLY2_OFFSETOF(STEPGUARD, ploTarget) == 0x38C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_aloot[SLY2_OFFSETOF(STEPGUARD, aloot) == 0x404 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_cloot[SLY2_OFFSETOF(STEPGUARD, cloot) == 0x484 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_abindingrec[SLY2_OFFSETOF(STEPGUARD, abindingrec) == 0x4D0 ? 1 : -1];   /* stepguard.cpp */
typedef char assert_STEPGUARD_aabindingZap[SLY2_OFFSETOF(STEPGUARD, aabindingZap) == 0x8D0 ? 1 : -1];   /* stepguard.cpp */
typedef char assert_STEPGUARD_tAttackNext[SLY2_OFFSETOF(STEPGUARD, tAttackNext) == 0xF04 ? 1 : -1];   /* guardbhv.cpp */
typedef char assert_STEPGUARD_cUnkFA8[SLY2_OFFSETOF(STEPGUARD, cUnkFA8) == 0xFA8 ? 1 : -1];   /* guardbhv.cpp */
typedef char assert_STEPGUARD_qwUnkF70[SLY2_OFFSETOF(STEPGUARD, qwUnkF70) == 0xF70 ? 1 : -1];   /* stepguard.cpp */
typedef char assert_STEPGUARD_pdetector[SLY2_OFFSETOF(STEPGUARD, pdetector) == 0xF64 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_dleGuard[SLY2_OFFSETOF(STEPGUARD, dleGuard) == 0xFE0 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_cBusy[SLY2_OFFSETOF(STEPGUARD, cBusy) == 0xFFC ? 1 : -1];   /* CONFIRMED runtime */

/* Option accessors (inline). g_aeopid takes their addresses, which is why out-of-line copies of them exist in the
 * linkonce area; src/game/app/eopid_linkonce.cpp takes the same addresses to emit them. See docs/guides/tu-merging.md
 * "Inline functions and linkonce copies". Free functions (not members): see there why. Names are placeholders. */
inline void func_00259D38(STEPGUARD *pstepguard, VOL *pvol) { pstepguard->pdetector->pvolNear = pvol; }   /* set */
inline void func_00259D48(STEPGUARD *pstepguard, VOL *pvol) { pstepguard->pdetector->pvolFar = pvol; }    /* set */
inline VOL *func_00259D60(STEPGUARD *pstepguard) { return pstepguard->pdetector->pvolNear; }   /* get */
inline VOL *func_00259D70(STEPGUARD *pstepguard) { return pstepguard->pdetector->pvolFar; }    /* get */

/* BHV156 (CID 0x9C, cb 0xB0): root of every guard's behaviour stack; its mode drives calm/search/combat */
struct BHV156 : public BHV
{
      int nMode;            /* +0x30 [R] 0 start, 1 calm, 2 search, 3 alert anim, 4 combat, 5 script, 7 juggled, 11 KO
                               (runtime2/4/7a; ram_inspector ROOT_MODE); 17 also seen (S28, meaning unknown) */
      undefined4 unk_0x34[3];
      LO *ploMode;          /* +0x40 [R] object of the mode (stimulus / target) */
      undefined4 unk_0x44[(0xb0 - 0x44) / 4];
};
typedef char assert_BHV156_size[sizeof(BHV156) == 0xB0 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_BHV156_nMode[SLY2_OFFSETOF(BHV156, nMode) == 0x30 ? 1 : -1];   /* CONFIRMED runtime */

#endif /* SLY2_GUARD_H */
