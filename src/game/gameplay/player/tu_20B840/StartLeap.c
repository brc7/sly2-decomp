#include "m2c_prelude.h"
M2C_UNK LEAP_ctor(void *);                      /* extern */
M2C_UNK SetLeapState(void *, M2C_UNK);             /* extern */
M2C_UNK memset(void *, M2C_UNK, M2C_UNK);           /* extern */

/** @sly2-annot addr=0x20BF80
 * Purpose: construct CID 0x91 leap in place and enter state 0
 * Info: proposed name StartLeap (T1); TU tu_20B840
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void StartLeap(void *arg0, s32 arg1, s32 arg2, void *arg3) {
    if (M2C_FIELD(arg0, s32 *, 4) != 0) {
        memset(arg3, 0, 0x70);
        memset(arg3, 0, 0x70);
        LEAP_ctor(arg3);
        M2C_FIELD(arg3, void **, 0x30) = arg0;
        M2C_FIELD(arg3, s32 *, 0x34) = arg1;
        M2C_FIELD(arg3, s32 *, 0x3C) = arg2;
        SetLeapState(arg3, 0);
    }
}
