#include "m2c_prelude.h"
s32 func_00161798(M2C_UNK *);                       /* extern */
M2C_UNK func_001617E0(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_002DEB30;

/** @sly2-annot addr=0x1C67A0
 * Purpose: increments a game counter via 0x1617E0/0x161798
 * Info: proposed name AwardDprize91 (T2); TU prize
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/game-progression.md#4-collectibles-and-loot-prizecpp-0x1c4e50,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C67A0(void) {
    func_001617E0(&D_002DEB30, func_00161798(&D_002DEB30) + 1);
}
