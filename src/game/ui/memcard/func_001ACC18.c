#include "m2c_prelude.h"
extern f32 D_002DDF0C;

/** @sly2-annot addr=0x1ACC18
 * Purpose: vt14: x28=mode, x2C=time if changed
 * Info: proposed name CMemcardDialog_SetMode (T2); class CMemcardDialog; TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ACC18(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x28)) {
        M2C_FIELD(arg0, s32 *, 0x28) = arg1;
        M2C_FIELD(arg0, f32 *, 0x2C) = (f32) D_002DDF0C;
    }
}
