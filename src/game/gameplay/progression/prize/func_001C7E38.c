#include "m2c_prelude.h"
M2C_UNK func_001C4E50();                            /* extern */

/** @sly2-annot addr=0x1C7E38
 * Purpose: CID 95 Init (clue bottle)
 * Info: proposed name InitClue (T1); class CLUE slot7; TU prize
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C7E38(void *arg0) {
    func_001C4E50();
    M2C_FIELD(arg0, f32 *, 0x1DC) = 50.0f;
}
