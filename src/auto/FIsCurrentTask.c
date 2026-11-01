#include "m2c_prelude.h"
extern s32 D_003E0FA0;

/** @sly2-annot addr=0x164018
 * Purpose: p == g_sw+0x3B10
 * Info: proposed name FIsCurrentTask (T1); TU game
 * Notes: names_v2 C proposed FActiveCplcy__FP5CPLCY (0.638)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r3b_rest
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
s32 FIsCurrentTask(s32 arg0) {
    return D_003E0FA0 == arg0;
}
