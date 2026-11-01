#include "m2c_prelude.h"
/** @sly2-annot addr=0x1AB398
 * Purpose: existing
 * Info: proposed name CompareSaves__FP4SAVET0 (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
s32 CompareSaves__FP4SAVET0(void *arg0, void *arg1) {
    s32 var_v0;

    var_v0 = M2C_FIELD(arg0, u16 *, 0x54) - M2C_FIELD(arg1, u16 *, 0x54);
    if (var_v0 == 0) {
        var_v0 = M2C_FIELD(arg0, u8 *, 0x52) - M2C_FIELD(arg1, u8 *, 0x52);
        if (var_v0 == 0) {
            var_v0 = M2C_FIELD(arg0, u8 *, 0x53) - M2C_FIELD(arg1, u8 *, 0x53);
            if (var_v0 == 0) {
                var_v0 = M2C_FIELD(arg0, u8 *, 0x56) - M2C_FIELD(arg1, u8 *, 0x56);
                if (var_v0 == 0) {
                    var_v0 = M2C_FIELD(arg0, u8 *, 0x57) - M2C_FIELD(arg1, u8 *, 0x57);
                    if (var_v0 == 0) {
                        return M2C_FIELD(arg0, u8 *, 0x58) - M2C_FIELD(arg1, u8 *, 0x58);
                    }
                }
            }
        }
    }
    return var_v0;
}
