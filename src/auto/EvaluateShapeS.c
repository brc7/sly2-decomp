#include "m2c_prelude.h"
f32 UFromShapeS();                                /* extern */
M2C_UNK EvaluateShapeU(s32, s32, s32, f32);          /* extern */

/** @sly2-annot addr=0x1E6F10
 * Purpose: EvaluateShapeU(UFromShapeS(s))
 * Info: proposed name EvaluateShapeS (T1); TU shape
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void EvaluateShapeS(s32 arg0, s32 arg1, s32 arg2) {
    EvaluateShapeU(arg0, arg1, arg2, UFromShapeS());
}
