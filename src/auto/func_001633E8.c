#include "m2c_prelude.h"
extern s32 D_003E0F9C;

/** @sly2-annot addr=0x1633E8
 * Purpose: p == g_sw+0x3B0C (current job)
 * Info: proposed name FIsCurrentJob (T1); TU game
 * Notes: names_v2 v1 proposed FUN_001e5e60 (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
s32 func_001633E8(s32 arg0) {
    return D_003E0F9C == arg0;
}
