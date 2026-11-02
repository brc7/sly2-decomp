#include "m2c_prelude.h"
M2C_UNK SetMcDialogState(void *, s32);                 /* extern */
M2C_UNK func_001DA770();                            /* extern */

/** @sly2-annot addr=0x1ACAE8
 * Purpose: UI slot 12: base 0x1DA770 then SetMcDialogState
 * Info: proposed name CMemcardDialog_vt12 (T2); class CMemcardDialog; TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ACAE8(void *arg0) {
    s32 temp_a1;
    void *temp_a0;

    func_001DA770();
    temp_a0 = M2C_FIELD(arg0, void **, 0x24);
    if (temp_a0 != NULL) {
        M2C_FIELD(temp_a0, s32 *, 0xB0) = 1;
        temp_a1 = M2C_FIELD(arg0, s32 *, 0x40);
        if (temp_a1 != -1) {
            SetMcDialogState(arg0, temp_a1);
        }
    }
}
