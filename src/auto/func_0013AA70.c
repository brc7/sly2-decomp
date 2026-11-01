#include "m2c_prelude.h"
M2C_UNK func_0013BC60(s32, s32);                    /* extern */
M2C_UNK func_001F6068();                            /* extern */

/** @sly2-annot addr=0x13AA70
 * Purpose: BOMB vt76: SO Clone then reset owner to self (0x13BC60)
 * Info: proposed name CloneBomb (T2); class BOMB slot76; TU bomb
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void func_0013AA70(s32 arg0) {
    func_001F6068();
    func_0013BC60(arg0, arg0);
}
