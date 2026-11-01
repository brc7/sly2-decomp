#include "m2c_prelude.h"
s32 func_0020F0B0(M2C_UNK *);                       /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1409B8
 * Purpose: vt98: returns 0 normally, -1 while the world is hands-off (FSwHandsOff g_sw)
 * Info: proposed name STEPPLAYER_VUnk97 (T2); class STEPPLAYER slot98; TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
s32 func_001409B8(void) {
    return (func_0020F0B0(&D_003DD490) != 0) ? 0 : -1;
}
