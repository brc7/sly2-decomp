#include "m2c_prelude.h"
M2C_UNK FUN_0018bc90(void *, s32, M2C_UNK, s32);    /* extern */
s32 func_00243EF0(s32, s32, s32, s32);              /* extern */

/** @sly2-annot addr=0x1AC7A0
 * Purpose: sceMcOpen wrapper + sync
 * Info: proposed name McOpen (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void McOpen(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    FUN_0018bc90(arg0 + 0xFD5C, func_00243EF0(M2C_FIELD(arg0, s32 *, 0), M2C_FIELD(arg0, s32 *, 4), arg1, arg2), 2, arg3);
}
