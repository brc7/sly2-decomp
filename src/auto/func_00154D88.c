#include "m2c_prelude.h"
f32 func_00154C90();                                /* extern */
M2C_UNK func_00154DC8(s32, f32);                    /* extern */

/** @sly2-annot addr=0x154D88
 * Purpose: Adds a delta to the current difficulty (SetDifficulty(GGetDifficulty()+d))
 * Info: proposed name ChangeDifficulty (T1); TU difficulty
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/game-progression.md#difficulty-0x154a60-0x1553f0
 * @end-sly2-annot */
void func_00154D88(s32 arg0, f32 fparg0) {
    func_00154DC8(arg0, func_00154C90() + fparg0);
}
