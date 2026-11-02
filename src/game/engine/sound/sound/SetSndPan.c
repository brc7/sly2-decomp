#include "m2c_prelude.h"
M2C_UNK SetSndFlags(void *, M2C_UNK, M2C_UNK);    /* extern */

/** @sly2-annot addr=0x1FB268
 * Purpose: Normalize degrees mod 360; set pan, dirty 0x20
 * Info: proposed name SetSndPan (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetSndPan(void *arg0, s32 arg1) {
    s32 temp_a1;
    s32 temp_hi;

    temp_hi = arg1 % 360;
    temp_a1 = (temp_hi > -1) ? temp_hi : (temp_hi + 0x168);
    if (temp_a1 != M2C_FIELD(arg0, s32 *, 0x14)) {
        M2C_FIELD(arg0, s32 *, 0x14) = temp_a1;
        SetSndFlags(arg0, 0x20, 0x20);
    }
}
