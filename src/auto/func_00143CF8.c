#include "m2c_prelude.h"
M2C_UNK func_002041C0();                            /* extern */

/** @sly2-annot addr=0x143CF8
 * Purpose: vt29: base 0x2041C0 then clear jtplaces count (+0x9E4)
 * Info: proposed name TranslateStepplayerToPos (T1); class STEPPLAYER slot29; TU stepplayer
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00143CF8(void *arg0) {
    func_002041C0();
    M2C_FIELD(arg0, s32 *, 0x9E4) = 0;
}
