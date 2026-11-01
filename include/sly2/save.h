/* include/sly2/save.h -- the game save blob (g_gs = D_003D4A60, 0x5400 bytes; written to the memory card).
 * Field names are the game's own save-variable names, decoded from the in-RAM save-variable schema at g_sw+0x3A18
 * (SW::savevars; devtools/pine_toolkit/data/savevars.json, 4472 variables) -- CONFIRMED runtime for name and offset.
 * Tags: [R] runtime-confirmed meaning, [S] name/offset from the schema only (meaning not checked).
 * Everything from +0x1A4 on is the per-episode / per-level tree (episode.<world>.dt, percent_complete,
 * level.<name>.booty.aoidAssembly[15], afClue (32 bits), fVault, ...); see savevars.json for exact offsets. */
#ifndef SLY2_SAVE_H
#define SLY2_SAVE_H
#include "sly2/basic.h"

/* per-character block, 0x18 bytes: sly at +0x50, bentley at +0x68, murray at +0x80 */
struct GSCHAR
{
      int hpCur;            /* +0x0 [R] current hp (JT jthurt.php points here; sly.hpCur 37 -> 40 between S10 and S11) */
      int mpCur;            /* +0x4 [R] current gadget power (STEPPLAYER::pmpCur points here) */
      int oidBooty;         /* +0x8 [S] */
      int apukCur[3];       /* +0xC [S] equipped power-ups (gadget ids) per slot */
};

struct GS
{
      int episodeCurrent;   /* +0x00 [S] */
      int episodeBest;      /* +0x04 [S] */
      int widCurrent;       /* +0x08 [S] world id */
      OID oidPoCurrent;     /* +0x0C [R] oid of the current playable character (== SW::ppoHero's oid in 71/73 states; keeps
                               the last character while the char-select PO is current) */
      float dt;             /* +0x10 [R] total play time (s); 655.6 -> 843.0 increasing over S02..S12 */

      float suck_history_mission[3];   /* +0x14 suck_history.mission_0..2 [S] */
      int lang;             /* +0x20 flags.lang [S] */
      int fNoVibration;     /* +0x24 flags.* [S] */
      int nMixerMode;       /* +0x28 */
      int fAutosaveWarned;  /* +0x2C */
      int fReverseLookAround;  /* +0x30 */
      int fReverseAim;      /* +0x34 */
      int fNoShowBudDistractWarning;  /* +0x38 */
      float uMusicVolume;   /* +0x3C */
      float uEffectsVolume; /* +0x40 */
      int fUseHeadset;      /* +0x44 */
      float dxScreenCenter; /* +0x48 */
      float dyScreenCenter; /* +0x4C */
      GSCHAR sly;           /* +0x50 [R] */
      GSCHAR bentley;       /* +0x68 [S] */
      GSCHAR murray;        /* +0x80 [S] */
      uint mppukfAcquired[2];  /* +0x98 powerUps.mppukfAcquired: 43 bits, one per power-up/gadget id [S] */
      int ccoin;            /* +0xA0 loot.ccoin [R] coins (6 -> 9 S03 -> S04; 22 -> 23 S10 -> S11; 23 -> 27 S11 -> S12) */
      int mplootkc[64];     /* +0xA4 loot.mplootkc [S] per-loot-kind counts */
      undefined4 unk_0x1a4[(0x5400 - 0x1a4) / 4];   /* +0x1A4 episode tree (see header comment) */
};
typedef char assert_GSCHAR_size[sizeof(GSCHAR) == 0x18 ? 1 : -1];   /* schema stride sly/bentley/murray (CONFIRMED) */
typedef char assert_GS_size[sizeof(GS) == 0x5400 ? 1 : -1];   /* save blob size (runtime: slygame G_GS) */
typedef char assert_GS_dt[SLY2_OFFSETOF(GS, dt) == 0x10 ? 1 : -1];   /* schema (CONFIRMED) */
typedef char assert_GS_sly[SLY2_OFFSETOF(GS, sly) == 0x50 ? 1 : -1];   /* schema; JT jthurt.php target (CONFIRMED runtime) */
typedef char assert_GS_murray[SLY2_OFFSETOF(GS, murray) == 0x80 ? 1 : -1];   /* schema (CONFIRMED) */
typedef char assert_GS_mppukfAcquired[SLY2_OFFSETOF(GS, mppukfAcquired) == 0x98 ? 1 : -1];   /* schema (CONFIRMED) */
typedef char assert_GS_ccoin[SLY2_OFFSETOF(GS, ccoin) == 0xA0 ? 1 : -1];   /* schema + runtime coin counts (CONFIRMED) */
typedef char assert_GS_mplootkc[SLY2_OFFSETOF(GS, mplootkc) == 0xA4 ? 1 : -1];   /* schema (CONFIRMED) */

#endif /* SLY2_SAVE_H */
