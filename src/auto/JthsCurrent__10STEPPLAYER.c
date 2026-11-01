#include "m2c_prelude.h"
M2C_UNK func_001A23C0(s32);                         /* extern */

/** @sly2-annot addr=0x140F70
 * Purpose: vt77: returns the hurt component's state (0x1A23C0 on +0xDF0)
 * Info: proposed name STEPPLAYER_JthsCurrent (T1); class STEPPLAYER slot77; TU stepplayer
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void JthsCurrent__10STEPPLAYER(s32 arg0) {
    func_001A23C0(arg0 + 0xDF0);
}
