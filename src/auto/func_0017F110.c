#include "m2c_prelude.h"
M2C_UNK RemoveSoHierarchy();                            /* extern */

/** @sly2-annot addr=0x17F110
 * Purpose: vt12: RemoveSoHierarchy then clear dying flag x18C
 * Info: proposed name RemoveHackHierarchy (T2); class HACK slot12; TU tu_17F090
 * Notes: CID47->CID71 (CID 0x47=71, vtable) | v3 class identity: RemoveCid71Hierarchy -> RemoveHackHierarchy
 *   | T2 forced: v3: coined class word (Hack) without descriptor/string backing
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F110(void *arg0) {
    RemoveSoHierarchy();
    M2C_FIELD(arg0, s32 *, 0x18C) = 0;
}
