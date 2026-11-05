/* include/sly2/clock.h -- CLOCK, the game clock (one instance: g_clock = D_002DDED0; core/time/clock.cpp).
 *
 * Moved out of sly2/basic.h (which includes it) so that files that only read the clock can share the one layout
 * instead of private views (docs/guides/tu-merging.md, "Views unified"). Depends on common.h only. */
#ifndef SLY2_CLOCK_H
#define SLY2_CLOCK_H
#include "common.h"
#define SLY2_CLOCK_OFFSETOF(T, f) ((int)(char *)&(((T *)0)->f))

/* CLOCK: from src/game/engine/camera/cm/UpdateCmMat4__FP2CM.cpp (15 copies in Lilly ports).
   One global instance: g_clock = D_002DDED0 (CONFIRMED runtime: ram_inspector/slygame G_CLOCK).
   Tags: [R] runtime-confirmed meaning (save-state RAM, runtime* FINDINGS / devtools), [S] static (matched code / Sly 1
   name, not checked at runtime), [H] hypothesis. */
struct CLOCK
{
      float t;              /* +0x00 [R] game time (s); < tReal in every state (does not advance while rt == 0) */
      float dt;             /* +0x04 [R] game frame delta (s) = dtReal * rt in all 73 states; 1/60 in gameplay */
      float rt;             /* +0x08 [R] game rate (rDt) = product of art[] in all 73 states (1.0 normally, 0 paused) */
      float rtCm;           /* +0x0C [R] camera rate (rDtCm; independent of the game-rate slots) */
      float dtCm;           /* +0x10 [R] camera frame delta (s) */
      float tCm;            /* +0x14 [R] camera time (s) */
      float art[9];         /* (CRK_Max) +0x18..+0x38 [R] 9 rate slots multiplied into rt: slot 0 = 0 in S54/S71 (paused), slot 1
                               (CRK_PowerUp) = 0.25 in S72 (slow-motion power-up active), slot 7 = 0 in S61 */

      float tReal;          /* +0x3C [R] real time (s), keeps running during pause; used for UI/menu repeat (DxSelectionJoy) */
      float dtReal;         /* +0x40 [R] real frame delta (s) */
      uint unk_0x44;        /* +0x44 [?] */

      ulong tickFrame;      /* +0x48 (TICK) */
      ulong tickLastRaw;    /* +0x50 */
      int cWrapAround;
      uint unk_0x5c;
};

typedef char assert_CLOCK_size[sizeof(CLOCK) == 0x60 ? 1 : -1];   /* g_clock 0x2DDED0..0x2DDF30 (S: Lilly layout; next global g_cm 0x2DDF40) */
typedef char assert_CLOCK_dt[SLY2_CLOCK_OFFSETOF(CLOCK, dt) == 0x4 ? 1 : -1];        /* CONFIRMED runtime */
typedef char assert_CLOCK_rt[SLY2_CLOCK_OFFSETOF(CLOCK, rt) == 0x8 ? 1 : -1];        /* CONFIRMED runtime (rDt) */
typedef char assert_CLOCK_rtCm[SLY2_CLOCK_OFFSETOF(CLOCK, rtCm) == 0xC ? 1 : -1];    /* CONFIRMED runtime */
typedef char assert_CLOCK_dtCm[SLY2_CLOCK_OFFSETOF(CLOCK, dtCm) == 0x10 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_CLOCK_tCm[SLY2_CLOCK_OFFSETOF(CLOCK, tCm) == 0x14 ? 1 : -1];     /* CONFIRMED runtime */
typedef char assert_CLOCK_art[SLY2_CLOCK_OFFSETOF(CLOCK, art) == 0x18 ? 1 : -1];     /* CONFIRMED runtime */
typedef char assert_CLOCK_tReal[SLY2_CLOCK_OFFSETOF(CLOCK, tReal) == 0x3C ? 1 : -1]; /* CONFIRMED runtime + match (DxSelectionJoy reads +0x3C) */
typedef char assert_CLOCK_dtReal[SLY2_CLOCK_OFFSETOF(CLOCK, dtReal) == 0x40 ? 1 : -1]; /* CONFIRMED runtime */

#endif
