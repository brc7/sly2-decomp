#include "m2c_prelude.h"
M2C_UNK func_001F31E0();                            /* extern */

/** @sly2-annot addr=0x223FE0
 * Purpose: LoadSoFromBrx then psophys(0xE8)->0xD4 = 0xD8 = 2.0
 * Info: proposed name LoadCid100FromBrx (T2); class CID100 slot14; TU tu_223F70
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r8_21BB88+parse-verified
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
void func_00223FE0(void *arg0) {
    void *temp_v0;

    func_001F31E0();
    temp_v0 = M2C_FIELD(arg0, void **, 0xE8);
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, f32 *, 0xD4) = 2.0f;
        M2C_FIELD(M2C_FIELD(arg0, void **, 0xE8), f32 *, 0xD8) = 2.0f;
    }
}
