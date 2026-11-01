#include "m2c_prelude.h"
extern s32 D_002DEB40;

/** @sly2-annot addr=0x165DC0
 * Purpose: Stores the current job in g_sw+0x3B0C and its oid in g_game+0x10
 * Info: proposed name SetSwJobCur (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00165DC0(void *arg0, void *arg1) {
    M2C_FIELD(arg0, void **, 0x3B0C) = arg1;
    if (arg1 != NULL) {
        D_002DEB40 = M2C_FIELD(arg1, s32 *, 0x18);
        return;
    }
    D_002DEB40 = -1;
}
