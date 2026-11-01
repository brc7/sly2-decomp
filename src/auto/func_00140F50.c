#include "m2c_prelude.h"
M2C_UNK func_001A19A0(s32);                         /* extern */

/** @sly2-annot addr=0x140F50
 * Purpose: vt75: forwards to the hurt component (+0xDF0) 0x1A19A0
 * Info: proposed name STEPPLAYER_FTakeDamage (T1); class STEPPLAYER slot75; TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140F50(s32 arg0) {
    func_001A19A0(arg0 + 0xDF0);
}
