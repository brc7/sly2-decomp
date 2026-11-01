#include "m2c_prelude.h"
M2C_UNK EndCurrentJob(s32);                         /* extern */
M2C_UNK BeginJob(s32);                         /* extern */
s32 FIsCurrentJob();                                /* extern */
extern s32 D_003E0F9C;

/** @sly2-annot addr=0x1628B8
 * Purpose: If p is not the current job: end the current one (EndCurrentJob) and begin p (BeginJob)
 * Info: proposed name SetCurrentJob (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void SetCurrentJob(s32 arg0) {
    if (FIsCurrentJob() == 0) {
        if (D_003E0F9C != 0) {
            EndCurrentJob(D_003E0F9C);
        }
        BeginJob(arg0);
    }
}
