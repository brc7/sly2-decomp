#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitSo();                            /* extern */
M2C_UNK func_001F7918(void *, M2C_UNK, M2C_UNK);    /* extern */

/** @sly2-annot addr=0x183900
 * Purpose: CID88 vt7: InitSo, InitGrabspot(+0x150), state 0, radius 300, flag x18C=1, zap response mask
 * Info: proposed name InitBarrel (T1); class BARREL slot7; TU grabspot
 * Notes: v3 class identity: InitCid88 -> InitBarrel
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void InitBarrel(void *arg0) {
    InitSo();
    InitGrabspot(arg0 + 0x150);
    M2C_FIELD(arg0, s32 *, 0x17C) = 0;
    M2C_FIELD(arg0, f32 *, 0x158) = 300.0f;
    M2C_FIELD(arg0, s32 *, 0x18C) = 1;
    func_001F7918(arg0, 0x03FFFFFF, 8);
    func_001F7918(arg0, 0x03188001, 0);
}
