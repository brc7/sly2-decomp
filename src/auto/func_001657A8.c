#include "m2c_prelude.h"
M2C_UNK func_001619A0(M2C_UNK *, M2C_UNK, s32);     /* extern */
extern M2C_UNK D_002DEB30;

/** @sly2-annot addr=0x1657A8
 * Purpose: Broadcasts event 0x1E with the task's job
 * Info: proposed name AnnounceTaskJob (T1); TU game
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_001657A8(void *arg0) {
    func_001619A0(&D_002DEB30, 0x1E, M2C_FIELD(arg0, s32 *, 0x7C));
}
