#include "m2c_prelude.h"
f32 F32Read__18CBinaryInputStream(s32);             /* extern */

/** @sly2-annot addr=0x12CC68
 * Purpose: Reads one F32
 * Info: proposed name LoadAcFloatConstFromBrx (T1); class AcFloatConst; TU act
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012CC68(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, f32 *, 4) = F32Read__18CBinaryInputStream(arg1);
}
