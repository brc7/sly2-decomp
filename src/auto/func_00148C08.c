#include "m2c_prelude.h"
M2C_UNK func_00148738(s32, s32);                    /* extern */
M2C_UNK func_001F31E0();                            /* extern */

/** @sly2-annot addr=0x148C08
 * Purpose: vt14: LoadSoFromBrx then LoadBtn(+0x150, this)
 * Info: proposed name LoadButtonFromBrx (T1); class BUTTON slot14; TU button
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+parse-verified
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00148C08(s32 arg0) {
    func_001F31E0();
    func_00148738(arg0 + 0x150, arg0);
}
