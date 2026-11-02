#include "m2c_prelude.h"
s32 FJobLive();                                /* extern */
M2C_UNK ActivateJob(void *);                      /* extern */
M2C_UNK CompleteJob(void *);                      /* extern */
M2C_UNK SetSwTaskPending(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x162CD0
 * Purpose: Called when a task of the job reaches state>=2: start task -> ActivateJob, end task ->
 *   CompleteJob, task with a pending-warp (+0xB8!=-1) -> set pending task
 * Info: proposed name OnJobTaskComplete (T1); TU game
 * Notes: names_v2 v1 proposed FireTurret__FP6TURRET (0.438)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void OnJobTaskComplete(void *arg0, void *arg1) {
    if (FJobLive() != 0) {
        if (arg1 == M2C_FIELD(arg0, s32 *, 0x64)) {
            ActivateJob(arg0);
        }
        if (arg1 == M2C_FIELD(arg0, s32 *, 0x68)) {
            CompleteJob(arg0);
        }
        if (M2C_FIELD(arg1, s32 *, 0xB8) != -1) {
            SetSwTaskPending(&D_003DD490, arg1);
        }
    }
}
