#include "m2c_prelude.h"
M2C_UNK EndJob(s32);                         /* extern */
s32 FIsCurrentJob();                                /* extern */

/** @sly2-annot addr=0x162908
 * Purpose: If p is the current job, EndJob(p)
 * Info: proposed name EndCurrentJob (T1); TU game
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void EndCurrentJob(s32 arg0) {
    if (FIsCurrentJob() != 0) {
        EndJob(arg0);
    }
}
