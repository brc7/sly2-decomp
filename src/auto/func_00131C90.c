#include "m2c_prelude.h"
M2C_UNK func_0012CED8();                            /* extern */

/** @sly2-annot addr=0x131C90
 * Purpose: Clears that binding
 * Info: proposed name AsegaChnDetach (T2); TU asega
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00131C90(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x20) != 0) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 8), void **, 0xA8), s32 *, 0xD0) = 0;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 8), void **, 0xA8), s32 *, 0xD4) = 0;
        return;
    }
    func_0012CED8();
}
