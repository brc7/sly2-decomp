#include "m2c_prelude.h"
M2C_UNK func_001A4E08(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002E1E88;

/** @sly2-annot addr=0x1660F8
 * Purpose: Re-initialises the game sequence queue D_002E1E88
 * Info: proposed name ResetGameSeq (T2); TU game
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_001660F8(void) {
    func_001A4E08(&D_002E1E88);
}
