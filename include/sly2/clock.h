/* include/sly2/clock.h -- CLOCK, the game clock (one instance: g_clock at 0x2DDED0; core/time/clock.cpp).
 *
 * Moved out of sly2/basic.h (which includes it) so that files that only read the clock can share the one layout
 * instead of private views (docs/guides/tu-merging.md, "Views unified"). Depends on common.h only. */
#ifndef SLY2_CLOCK_H
#define SLY2_CLOCK_H
#include "common.h"
#define SLY2_CLOCK_OFFSETOF(T, f) ((int)(char *)&(((T *)0)->f))

/* CLOCK: from src/game/engine/camera/cm/UpdateCmMat4__FP2CM.cpp (15 copies in Lilly ports).
   One global instance: g_clock at 0x2DDED0 (CONFIRMED runtime: ram_inspector/slygame G_CLOCK).
   Tags: [R] runtime-confirmed meaning (save-state RAM, runtime* FINDINGS / devtools), [S] static (matched code / Sly 1
   name, not checked at runtime), [H] hypothesis. */
struct CLOCK
{
      float time;           /* +0x00 [R] game time (s); < realTime in every state (does not advance while rate == 0) */
      union {
            float deltaTime;  /* +0x04 [R] game frame delta (s) = realDeltaTime * rate in all 73 states; 1/60 in gameplay */
            int deltaTimeBits; /* the same word as raw bits: handed to VU0 (qmtc2) or VU_FLOAT as an int (cm, rip, prize, step) */
      };
      float rate;           /* +0x08 [R] game rate (rDt) = product of art[] in all 73 states (1.0 normally, 0 paused) */
      float cameraRate;     /* +0x0C [R] camera rate (rDtCm; independent of the game-rate slots) */
      float cameraDeltaTime; /* +0x10 [R] camera frame delta (s) */
      float cameraTime;     /* +0x14 [R] camera time (s) */
      float art[9];         /* (CRK_Max) +0x18..+0x38 [R] 9 rate slots multiplied into rate: slot 0 = 0 in S54/S71 (paused), slot 1
                               (CRK_PowerUp) = 0.25 in S72 (slow-motion power-up active), slot 7 = 0 in S61 */

      float realTime;       /* +0x3C [R] real time (s), keeps running during pause; used for UI/menu repeat (GetJoySelectionStepX) */
      float realDeltaTime;  /* +0x40 [R] real frame delta (s) */
      uint unknown44;       /* +0x44 [?] */

      ulong frameTick;      /* +0x48 (TICK) */
      ulong lastRawTick;    /* +0x50 the previous raw tick count; a smaller new count bumps numTickWraps (clock.cpp) */
      int numTickWraps;
      uint unknown5C;
};

typedef char assert_CLOCK_size[sizeof(CLOCK) == 0x60 ? 1 : -1];   /* g_clock 0x2DDED0..0x2DDF30 (S: Lilly layout; next global g_cm 0x2DDF40) */
typedef char assert_CLOCK_deltaTime[SLY2_CLOCK_OFFSETOF(CLOCK, deltaTime) == 0x4 ? 1 : -1]; /* CONFIRMED runtime */
typedef char assert_CLOCK_rate[SLY2_CLOCK_OFFSETOF(CLOCK, rate) == 0x8 ? 1 : -1];    /* CONFIRMED runtime (rDt) */
typedef char assert_CLOCK_cameraRate[SLY2_CLOCK_OFFSETOF(CLOCK, cameraRate) == 0xC ? 1 : -1]; /* CONFIRMED runtime */
typedef char assert_CLOCK_cameraDeltaTime[SLY2_CLOCK_OFFSETOF(CLOCK, cameraDeltaTime) == 0x10 ? 1 : -1]; /* CONFIRMED runtime */
typedef char assert_CLOCK_cameraTime[SLY2_CLOCK_OFFSETOF(CLOCK, cameraTime) == 0x14 ? 1 : -1]; /* CONFIRMED runtime */
typedef char assert_CLOCK_art[SLY2_CLOCK_OFFSETOF(CLOCK, art) == 0x18 ? 1 : -1];     /* CONFIRMED runtime */
typedef char assert_CLOCK_realTime[SLY2_CLOCK_OFFSETOF(CLOCK, realTime) == 0x3C ? 1 : -1]; /* CONFIRMED runtime + match (GetJoySelectionStepX reads +0x3C) */
typedef char assert_CLOCK_realDeltaTime[SLY2_CLOCK_OFFSETOF(CLOCK, realDeltaTime) == 0x40 ? 1 : -1]; /* CONFIRMED runtime */
typedef char assert_CLOCK_frameTick[SLY2_CLOCK_OFFSETOF(CLOCK, frameTick) == 0x48 ? 1 : -1]; /* match (hud.cpp reads +0x48) */

/* g_clock: the game clock (clock.cpp defines it under its address name). */
extern CLOCK g_clock __asm__("g_clock");

#endif
