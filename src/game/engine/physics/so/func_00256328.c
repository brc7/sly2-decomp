#include "m2c_prelude.h"
M2C_UNK func_001F4BF8(s32);                         /* extern */

/** @sly2-annot addr=0x256328
 * Purpose: set (p->x24)->int@0x18 then MarkSoTreeDirty [game linkonce (inline/template)]
 * Info: proposed name SetSoOptI18 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00256328(void *arg0, s32 arg1) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x24), s32 *, 0x18) = arg1;
    func_001F4BF8(M2C_FIELD(arg0, s32 *, 0x20));
}
