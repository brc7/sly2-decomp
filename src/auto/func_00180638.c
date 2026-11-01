#include "m2c_prelude.h"
M2C_UNK func_0017F170();                            /* extern */
M2C_UNK func_00181268(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x180638
 * Purpose: vt26: base + SetCid49State
 * Info: proposed name HACKENEMY_vt26 (T2); class HACKENEMY slot26; TU tu_17F090
 * Notes: names_v2 v1 proposed FUN_001caad0 (0.000) | CID49->CID73 (CID 0x49=73, vtable) | class identity from
 *   whole_game WE names/object strings: CID73_vt26 -> HACKENEMY_vt26
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00180638(s32 arg0) {
    func_0017F170();
    func_00181268(arg0, 0);
}
