#include "m2c_prelude.h"
M2C_UNK func_0017F110();                            /* extern */
M2C_UNK func_00181268(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x180598
 * Purpose: vt12: CID47 RemoveHierarchy + SetCid49State
 * Info: proposed name RemoveHackenemyHierarchy (T2); class HACKENEMY slot12; TU tu_17F090
 * Notes: names_v2 v1 proposed AddStepguardEffect__FP9STEPGUARD3OID3ZPK (0.638) | CID49->CID73 (CID 0x49=73,
 *   vtable) | class identity from whole_game WE names/object strings: RemoveCid73Hierarchy ->
 *   RemoveHackenemyHierarchy
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00180598(s32 arg0) {
    func_0017F110();
    func_00181268(arg0, -1);
}
