#include "m2c_prelude.h"
M2C_UNK func_001619A0(M2C_UNK *, M2C_UNK, s32);     /* extern */
extern M2C_UNK D_002DEB30;

/** @sly2-annot addr=0x162DD0
 * Purpose: Broadcasts game event 0x1D for the job
 * Info: proposed name SignalJobCompleteEvent (T2); TU game
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162DD0(s32 arg0) {
    func_001619A0(&D_002DEB30, 0x1D, arg0);
}
