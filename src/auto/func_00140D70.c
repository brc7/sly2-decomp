#include "m2c_prelude.h"
M2C_UNK func_00199298(void *);                      /* extern */
M2C_UNK func_00204768();                            /* extern */

/** @sly2-annot addr=0x140D70
 * Purpose: vt58: STEP base 0x204768, then jtair hook 0x199298 if jtair state != -1
 * Info: proposed name STEPPLAYER_VUnk57 (T2); class STEPPLAYER slot58; TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140D70(void *arg0) {
    func_00204768();
    if (M2C_FIELD(arg0, s32 *, 0x2DC) != -1) {
        func_00199298(arg0 + 0x2C0);
    }
}
