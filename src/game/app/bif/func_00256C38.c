#include "m2c_prelude.h"
/** @sly2-annot addr=0x256C38
 * Purpose: CPair ref-pointer constructor from a raw pointer: incref (if non-null) and store, no release
 * Info: proposed name PairPtr_ctor (T2); TU game_linkonce
 * Notes: v3: superseded PairPtr_Init | top50_round (0.55): Body ++*p; *pp=p. Same smart-pointer family as
 *   PairPtr_Assign 0x256C58 / PairPtr_Release 0x256D40; only caller PpairBuildSpliceEventArgs 0x132ED8 uses
 *   it to initialise a fresh list head and the return value | v2 draft wa
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: sdk_names+top50_round
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00256C38(s32 **arg0, s32 *arg1) {
    if (arg1 != NULL) {
        *arg1 += 1;
    }
    *arg0 = arg1;
}
