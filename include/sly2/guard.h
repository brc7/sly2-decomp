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
      undefined4 unk_0xf0[(0x180 - 0xf0) / 4];
};
typedef char assert_DETECTOR_size[sizeof(DETECTOR) == 0x180 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_DETECTOR_pvolNear[SLY2_OFFSETOF(DETECTOR, pvolNear) == 0xE8 ? 1 : -1];

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
      undefined4 unk_0x340[(0x38c - 0x340) / 4];
      LO *ploTarget;        /* +0x38C [R] current target (Sly while chasing) */
      undefined4 unk_0x390[(0x3cc - 0x390) / 4];
      void *pvEngage;       /* +0x3CC [R] engagement record (non-zero while in combat) [H type] */
      undefined4 unk_0x3d0[(0x404 - 0x3d0) / 4];
      GUARDLOOT aloot[8];   /* +0x404 [R] pickpocket loot list */
      int cloot;            /* +0x484 [R] entries used in aloot */
      undefined4 unk_0x488[(0xf60 - 0x488) / 4];
      int nFiringWindow;    /* +0xF60 [R] firing window (shooting guards) */
      DETECTOR *pdetector;  /* +0xF64 [R] vision detector (CID 0x10; its +0xE0 owner == this guard) */
      undefined4 unk_0xf68[(0xfe0 - 0xf68) / 4];
      DLE dleGuard;         /* +0xFE0 [R] link in the in-world guard list (DL at 0x3E1444, ibDle 0xFE0) */
      uint grfCapability;   /* +0xFE8 [R] capability bits (bit 3 = may shoot) */
      undefined4 unk_0xfec[(0xffc - 0xfec) / 4];
      int cBusy;            /* +0xFFC [R] busy count */
      undefined4 unk_0x1000[4];
    virtual void VSlot87();   /* slot 87: not yet identified (STEPGUARD adds 2 slots to STEP's 87) */
    virtual void VSlot88();   /* slot 88: not yet identified */
};
typedef char assert_STEPGUARD_size[sizeof(STEPGUARD) == 0x1010 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_STEPGUARD_grfsg[SLY2_OFFSETOF(STEPGUARD, grfsg) == 0x2C0 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_hp[SLY2_OFFSETOF(STEPGUARD, hp) == 0x33C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_ploTarget[SLY2_OFFSETOF(STEPGUARD, ploTarget) == 0x38C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_aloot[SLY2_OFFSETOF(STEPGUARD, aloot) == 0x404 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_cloot[SLY2_OFFSETOF(STEPGUARD, cloot) == 0x484 ? 1 : -1];   /* CONFIRMED runtime */
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
