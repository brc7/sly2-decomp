#include "m2c_prelude.h"
M2C_UNK GoToTask(s32);                         /* extern */
extern s32 D_003E0FA4;

/** @sly2-annot addr=0x162DF8
 * Purpose: GoToTask on the pending task g_sw+0x3B14, else on the job's start task (+0x64)
 * Info: proposed name RestartJob (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162DF8(void *arg0) {
    if (D_003E0FA4 != 0) {
        GoToTask(D_003E0FA4);
        return;
    }
    GoToTask(M2C_FIELD(arg0, s32 *, 0x64));
}
