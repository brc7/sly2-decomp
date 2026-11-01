#include "m2c_prelude.h"
M2C_UNK TriggerRumbleRumk__FP6RUMBLE4RUMKf(void *); /* extern */

/** @sly2-annot addr=0x187E28
 * Purpose: If rumble enabled and pad is a ready dualshock (joys>=3, joyk==3) TriggerRumbleRumk on the JOY's
 *   RUMBLE
 * Info: proposed name TriggerJoyRumbleRumk (T1); TU joy
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/animation.md#7-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00187E28(void *arg0) {
    if ((M2C_FIELD(arg0, s32 *, 0x10E8) != 0) && (M2C_FIELD(arg0, s32 *, 0x1010) >= 3) && (M2C_FIELD(arg0, s32 *, 0x100C) == 3)) {
        TriggerRumbleRumk__FP6RUMBLE4RUMKf(arg0 + 0x10CC);
    }
}
