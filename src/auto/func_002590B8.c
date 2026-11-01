#include "m2c_prelude.h"
M2C_UNK CloneXfm();                            /* extern */

/** @sly2-annot addr=0x2590B8
 * Purpose: EXPLO/EXPLS vt41 [game linkonce (inline/template)]
 * Info: proposed name CloneExplo (T1); class EXPLO slot41; TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002590B8(void *arg0) {
    s32 *temp_v1;

    CloneXfm();
    temp_v1 = M2C_FIELD(arg0, s32 **, 0xE4);
    *temp_v1 += 1;
}
