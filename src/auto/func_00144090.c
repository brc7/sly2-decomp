#include "m2c_prelude.h"
s32 func_001A23C0(s32);                             /* extern */
s32 func_001C41B8();                                /* extern */

/** @sly2-annot addr=0x144090
 * Purpose: vt80: PO FCanPoBeHealed and hurt state != 2
 * Info: proposed name STEPPLAYER_FCanBeHealed (T1); class STEPPLAYER slot80; TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_00144090(s32 arg0) {
    if (func_001C41B8() == 0) {
        return 0;
    }
    return func_001A23C0(arg0 + 0xDF0) != 2;
}
