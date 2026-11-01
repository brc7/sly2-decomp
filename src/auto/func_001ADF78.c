#include "m2c_prelude.h"
extern f32 D_002DDF0C;

/** @sly2-annot addr=0x1ADF78
 * Purpose: vt14 same body as 0x1ACC18
 * Info: proposed name CMcStatus_SetMode (T2); class CMcStatus; TU memcard
 * Notes: runtime4 CORRECTION: x1AC4 = Fists of Flame overlay ASEG (oid 0x156); x1AC8/x1ACC are constants
 * Confidence: low (draft) [T2 0.30] - evidence: runtime (save-state RAM); sources:
 *   r4_17F090+runtime-corrected
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ADF78(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x28)) {
        M2C_FIELD(arg0, s32 *, 0x28) = arg1;
        M2C_FIELD(arg0, f32 *, 0x2C) = (f32) D_002DDF0C;
    }
}
