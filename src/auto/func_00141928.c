#include "m2c_prelude.h"
M2C_UNK func_00130658(s32);                         /* extern */
M2C_UNK func_001EECD0(s32);                         /* extern */
M2C_UNK func_001F2D48();                            /* extern */

/** @sly2-annot addr=0x141928
 * Purpose: STEPPLAYER vt12: base 0x1F2D48 then stops two actions (0x130658 x2) and 0x1EECD0
 * Info: proposed name RemoveHierarchyStepplayer (T2); class STEPPLAYER slot12; TU stepplayer
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00141928(s32 arg0) {
    func_001F2D48();
    func_00130658(arg0 + 0xEFC);
    func_00130658(arg0 + 0xF14);
    func_001EECD0(arg0 + 0x26C);
}
