#include "m2c_prelude.h"
/** @sly2-annot addr=0x1443D8
 * Purpose: Return jtmode.nState (+0xF04) in 0..2 (unsigned compare; -1 = no jt mode)
 * Info: proposed name FStepplayerInJtmode (T1); TU stepplayer
 * Notes: top50_round (0.65): Lilly's STEPPLAYER/JTMODE headers: JTMODE {pstepplayer; nState; tEnter; ...;
 *   nKind(+0x20)}; with jtmode at +0xF00 nState is +0xF04 and nKind +0xF20 (siblings FStepplayerJtmodeKind0/1
 *   test +0xF20). Callers BENTLEY/MURRAY_
 * Confidence: mid (draft) [T1 0.65] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r2b_rest+top50_round
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
s32 FStepplayerInJtmode(void *arg0) {
    return (u32) M2C_FIELD(arg0, u32 *, 0xF04) < 3U;
}
