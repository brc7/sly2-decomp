#include "m2c_prelude.h"
extern f32 D_002DDF0C;

/** @sly2-annot addr=0x212A30
 * Purpose: if state==2, state time = g_clock.t (restarts the visible-duration timeout)
 * Info: proposed name ResetWidgetVisibleTime (T2); TU hud
 * Notes: names_v2 v1 proposed StopTimer__FP5TIMER (0.006)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00212A30(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x58) == 2) {
        M2C_FIELD(arg0, f32 *, 0x5C) = (f32) D_002DDF0C;
    }
}
