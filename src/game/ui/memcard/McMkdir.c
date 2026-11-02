#include "m2c_prelude.h"
s32 McSyncResult(void *, s32, M2C_UNK, M2C_UNK);    /* extern */
s32 func_00244018(s32, s32, s32);                   /* extern */

/** @sly2-annot addr=0x1AC748
 * Purpose: sceMcMkdir wrapper + sync
 * Info: proposed name McMkdir (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
s32 McMkdir(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = McSyncResult(arg0 + 0xFD5C, func_00244018(M2C_FIELD(arg0, s32 *, 0), M2C_FIELD(arg0, s32 *, 4), arg1), 0xB, 0);
    return ((temp_v0 ^ 0x18) == 0) ? 0 : temp_v0;
}
