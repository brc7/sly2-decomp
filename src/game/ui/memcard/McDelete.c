#include "m2c_prelude.h"
s32 FUN_0018bc90(void *, s32, M2C_UNK, M2C_UNK);    /* extern */
s32 func_00244918(s32, s32, s32);                   /* extern */

/** @sly2-annot addr=0x1ACA40
 * Purpose: sceMcDelete wrapper
 * Info: proposed name McDelete (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
s32 McDelete(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = FUN_0018bc90(arg0 + 0xFD5C, func_00244918(M2C_FIELD(arg0, s32 *, 0), M2C_FIELD(arg0, s32 *, 4), arg1), 0xF, 0);
    return ((temp_v0 ^ 0xE) == 0) ? 0 : temp_v0;
}
