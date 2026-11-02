#include "m2c_prelude.h"
s32 FMemcardThreadValid__FP7MEMCARD(M2C_UNK *);     /* extern */
M2C_UNK HideMcDialogWidget(void *);                      /* extern */
M2C_UNK func_001DA808();                            /* extern */
extern f32 D_002DDF0C;
extern M2C_UNK D_002F633C;

/** @sly2-annot addr=0x1ADEE8
 * Purpose: vt7 (vtable 0x2D36D0): base 0x1DA808; when shown >1s and thread idle hide (0x1ADE70)
 * Info: proposed name CMcStatus_Update (T2); class CMcStatus; TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ADEE8(void *arg0) {
    s32 temp_a0;
    void *temp_v1;

    func_001DA808();
    temp_v1 = M2C_FIELD(arg0, void **, 0x24);
    if (temp_v1 != NULL) {
        temp_a0 = M2C_FIELD(temp_v1, s32 *, 0x58);
        if ((temp_a0 == 2) && (M2C_FIELD(temp_v1, s32 *, 0x64) == temp_a0) && ((D_002DDF0C - M2C_FIELD(temp_v1, f32 *, 0x5C)) > 1.0f) && (FMemcardThreadValid__FP7MEMCARD(&D_002F633C) == 0)) {
            HideMcDialogWidget(arg0);
        }
    }
}
