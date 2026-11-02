#include "m2c_prelude.h"
M2C_UNK ReleaseAct(s32);                         /* extern */
M2C_UNK ApplyBombThrow();                            /* extern */

/** @sly2-annot addr=0x1B1F10
 * Purpose: MISSILE vt77: BOMB base (0x13B250) then retract asega at +0x268
 * Info: proposed name ExplodeMissile (T2); class MISSILE slot77; TU missile
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void func_001B1F10(s32 arg0) {
    ApplyBombThrow();
    ReleaseAct(arg0 + 0x268);
}
