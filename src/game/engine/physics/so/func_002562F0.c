#include "m2c_prelude.h"
M2C_UNK func_001F4BF8(s32);                         /* extern */

/** @sly2-annot addr=0x2562F0
 * Purpose: set (p->x24)->s16@0x14 then MarkSoTreeDirty [game linkonce (inline/template)]
 * Info: proposed name SetSoOptS14 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002562F0(void *arg0, s16 arg1) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x24), s16 *, 0x14) = arg1;
    func_001F4BF8(M2C_FIELD(arg0, s32 *, 0x20));
}
