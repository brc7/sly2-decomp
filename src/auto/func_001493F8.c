#include "m2c_prelude.h"
M2C_UNK func_002311B0();                            /* extern */

/** @sly2-annot addr=0x1493F8
 * Purpose: vt41
 * Info: proposed name CloneVolbtn (T1); class VOLBTN slot41; TU button
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_001493F8(void *arg0) {
    func_002311B0();
    M2C_FIELD(arg0, void **, 0x130) = arg0;
}
