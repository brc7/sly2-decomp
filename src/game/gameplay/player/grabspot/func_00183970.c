#include "m2c_prelude.h"
M2C_UNK func_001CB518();                            /* extern */

/** @sly2-annot addr=0x183970
 * Purpose: vt26: SO slot 26 base then state x17C=0
 * Info: proposed name BARREL_vt26 (T2); class BARREL slot26; TU grabspot
 * Notes: v3 class identity: CID88_vt26 -> BARREL_vt26 | T2: placeholder token vt26
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183970(void *arg0) {
    func_001CB518();
    M2C_FIELD(arg0, s32 *, 0x17C) = 0;
}
