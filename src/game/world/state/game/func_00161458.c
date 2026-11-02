#include "m2c_prelude.h"
M2C_UNK func_00154A80(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002DE578;

/** @sly2-annot addr=0x161458
 * Purpose: Clears the difficulty samples of the global difficulty object (ClearDifficulty(&D_002DE578))
 * Info: proposed name ClearGameDifficulty (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00161458(void) {
    func_00154A80(&D_002DE578);
}
