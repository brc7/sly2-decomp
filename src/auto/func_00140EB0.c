#include "m2c_prelude.h"
M2C_UNK func_001619A0(M2C_UNK *, M2C_UNK, s32);     /* extern */
extern M2C_UNK D_002DEB30;

/** @sly2-annot addr=0x140EB0
 * Purpose: vt93: broadcast game event 0x10 on D_002DEB30 with the player
 * Info: proposed name OnStepplayerHeroDied (T1); class STEPPLAYER slot93; TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_00140EB0(s32 arg0) {
    func_001619A0(&D_002DEB30, 0x10, arg0);
}
