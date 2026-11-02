#include "m2c_prelude.h"
M2C_UNK func_0012CE98(void *, void *);              /* extern */

/** @sly2-annot addr=0x131C40
 * Purpose: Writes channel binding (+0x20/+0x24) into the target object's +0xA8 record
 * Info: proposed name AsegaChnAttach (T2); TU asega
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00131C40(void *arg0) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x20);
    if (temp_a0 != 0) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 8), void **, 0xA8), s32 *, 0xD0) = temp_a0;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 8), void **, 0xA8), s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0x24) + 0x1C);
        return;
    }
    func_0012CE98(arg0, arg0);
}
