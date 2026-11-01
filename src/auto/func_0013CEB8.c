#include "m2c_prelude.h"
M2C_UNK func_00183F08();                            /* extern */

/** @sly2-annot addr=0x13CEB8
 * Purpose: cid 0x25 vt7: base init 0x183F08, default kind 0x2BC, -1, 15.0
 * Info: proposed name InitBootyContainer (T2); class CID37 slot7; TU booty
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013CEB8(void *arg0) {
    func_00183F08();
    M2C_FIELD(arg0, s32 *, 0x184) = 0x2BC;
    M2C_FIELD(arg0, s32 *, 0x188) = -1;
    M2C_FIELD(arg0, f32 *, 0x18C) = 15.0f;
}
