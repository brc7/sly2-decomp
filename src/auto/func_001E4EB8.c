#include "m2c_prelude.h"
M2C_UNK func_00143DE8(s32, void *);                 /* extern */

/** @sly2-annot addr=0x1E4EB8
 * Purpose: Set prelaycmdJt (+0x40) and the controlled character pjt (+0x44), detaching the old one and
 *   attaching the new one via 0x143DE8(pjt, pjsg)
 * Info: proposed name SetJsgJt (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E4EB8(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;

    if (arg1 != M2C_FIELD(arg0, s32 *, 0x40)) {
        M2C_FIELD(arg0, s32 *, 0x40) = arg1;
    }
    temp_a0 = M2C_FIELD(arg0, s32 *, 0x44);
    if (arg2 != temp_a0) {
        if (temp_a0 != 0) {
            func_00143DE8(temp_a0, NULL);
        }
        M2C_FIELD(arg0, s32 *, 0x44) = arg2;
        if (arg2 != 0) {
            func_00143DE8(arg2, arg0);
        }
    }
}
