#include "m2c_prelude.h"
M2C_UNK SetBootyState(s32, M2C_UNK);                /* extern */
M2C_UNK PostSoLoad();                            /* extern */

/** @sly2-annot addr=0x13BF00
 * Purpose: cid 0x67 vt15: PostSoLoad then SetBootyState(0)
 * Info: proposed name PostBootyLoad (T1); class BOOTY slot15; TU booty
 * Notes: names_v2 v1 proposed CloneAlarm__FP5ALARMT0 (0.041)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void PostBootyLoad(s32 arg0) {
    PostSoLoad();
    SetBootyState(arg0, 0);
}
