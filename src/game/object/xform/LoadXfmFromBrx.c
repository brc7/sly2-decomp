#include "m2c_prelude.h"
M2C_UNK LoadOptionsFromBrx__FPvP18CBinaryInputStream(s32, s32);                    /* extern */
M2C_UNK LoadXfmXfFromBrx();                            /* extern */

/** @sly2-annot addr=0x231170
 * Purpose: 0x231768 (XF ref + matrix) then LoadOptionsFromBrx
 * Info: proposed name LoadXfmFromBrx (T1); class XFM slot14; TU xform
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8+parse-verified
 * See: docs/engine/animation.md#7-key-functions, docs/engine/object-model.md#9-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14
 * @end-sly2-annot */
void LoadXfmFromBrx(s32 arg0, s32 arg1) {
    LoadXfmXfFromBrx();
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(arg0, arg1);
}
