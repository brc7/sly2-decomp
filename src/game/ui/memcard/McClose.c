#include "m2c_prelude.h"
M2C_UNK McSyncResult(s32, s32, M2C_UNK, M2C_UNK);   /* extern */
s32 func_00244050(s32);                             /* extern */

/** @sly2-annot addr=0x1AC808
 * Purpose: sceMcClose wrapper + sync
 * Info: proposed name McClose (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void McClose(s32 arg0, s32 arg1) {
    McSyncResult(arg0 + 0xFD5C, func_00244050(arg1), 3, 0);
}
