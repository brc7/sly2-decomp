#include "m2c_prelude.h"
M2C_UNK EndTask(s32);                         /* extern */
s32 FIsCurrentTask();                                /* extern */

/** @sly2-annot addr=0x163CE0
 * Purpose: If p is the current task, EndTask(p)
 * Info: proposed name DeactivateTask (T1); TU game
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void DeactivateTask(s32 arg0) {
    if (FIsCurrentTask() != 0) {
        EndTask(arg0);
    }
}
