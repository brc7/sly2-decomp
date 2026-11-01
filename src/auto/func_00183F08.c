#include "m2c_prelude.h"
M2C_UNK func_00182C70(s32);                         /* extern */
M2C_UNK func_00231100();                            /* extern */

/** @sly2-annot addr=0x183F08
 * Purpose: CID35 vt7: InitXfm, InitGrabspot(+0xE0), x0F0=1, radius 200, player mask 0
 * Info: proposed name InitCid35 (T2); class CID35 slot7; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558,
 *   docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183F08(void *arg0) {
    func_00231100();
    func_00182C70(arg0 + 0xE0);
    M2C_FIELD(arg0, s32 *, 0xF0) = 1;
    M2C_FIELD(arg0, f32 *, 0x110) = 200.0f;
    M2C_FIELD(arg0, s32 *, 0xFC) = 0;
}
