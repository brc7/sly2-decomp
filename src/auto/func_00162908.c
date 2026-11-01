#include "m2c_prelude.h"
M2C_UNK func_00162A60(s32);                         /* extern */
s32 func_001633E8();                                /* extern */

/** @sly2-annot addr=0x162908
 * Purpose: If p is the current job, EndJob(p)
 * Info: proposed name EndCurrentJob (T1); TU game
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162908(s32 arg0) {
    if (func_001633E8() != 0) {
        func_00162A60(arg0);
    }
}
