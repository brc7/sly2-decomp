#include "m2c_prelude.h"
s32 func_001C4660();                                /* extern */
M2C_UNK func_001C4738(s32, s32);                    /* extern */

/** @sly2-annot addr=0x1C46C0
 * Purpose: GrfPressedWithin then ClearJoyPressed on the result (consume the press)
 * Info: proposed name POCTL_GrfConsumePressed (T1); class POCTL; TU po
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
s32 func_001C46C0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001C4660();
    func_001C4738(arg0, temp_v0);
    return temp_v0;
}
