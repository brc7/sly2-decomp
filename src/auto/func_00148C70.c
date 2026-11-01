#include "m2c_prelude.h"
M2C_UNK func_001F6068();                            /* extern */

/** @sly2-annot addr=0x148C70
 * Purpose: vt41: SO Clone then btn owner = self
 * Info: proposed name CloneButton (T1); class BUTTON slot41; TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00148C70(void *arg0) {
    func_001F6068();
    M2C_FIELD(arg0, void **, 0x150) = arg0;
}
