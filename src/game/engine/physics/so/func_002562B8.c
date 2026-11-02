#include "m2c_prelude.h"
M2C_UNK func_001F4BF8(s32);                         /* extern */

/** @sly2-annot addr=0x2562B8
 * Purpose: set (p->x24)->f8 then MarkSoTreeDirty(p->x20) [game linkonce (inline/template)]
 * Info: proposed name SetSoOptF08 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002562B8(void *arg0, f32 fparg0) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x24), f32 *, 8) = fparg0;
    func_001F4BF8(M2C_FIELD(arg0, s32 *, 0x20));
}
