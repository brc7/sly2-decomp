#include "m2c_prelude.h"
f32 GGetDifficulty();                                /* extern */
M2C_UNK SetDifficulty(s32, f32);                    /* extern */

/** @sly2-annot addr=0x154D88
 * Purpose: Adds a delta to the current difficulty (SetDifficulty(GGetDifficulty()+d))
 * Info: proposed name ChangeDifficulty (T1); TU difficulty
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/game-progression.md#difficulty-0x154a60-0x1553f0
 * @end-sly2-annot */
void ChangeDifficulty(s32 arg0, f32 fparg0) {
    SetDifficulty(arg0, GGetDifficulty() + fparg0);
}
