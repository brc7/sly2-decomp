#include "m2c_prelude.h"
extern s32 D_002DEB44;

/** @sly2-annot addr=0x165DE8
 * Purpose: Stores the current task in g_sw+0x3B10 and its oid in g_game+0x14
 * Info: proposed name SetSwTaskCur (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00165DE8(void *arg0, void *arg1) {
    M2C_FIELD(arg0, void **, 0x3B10) = arg1;
    if (arg1 != NULL) {
        D_002DEB44 = M2C_FIELD(arg1, s32 *, 0x18);
        return;
    }
    D_002DEB44 = -1;
}
