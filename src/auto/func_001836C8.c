#include "m2c_prelude.h"
M2C_UNK func_00182C70(s32);                         /* extern */
M2C_UNK func_00231100();                            /* extern */

/** @sly2-annot addr=0x1836C8
 * Purpose: CID38 vt7: InitXfm + InitGrabspot(+0xE0)
 * Info: proposed name InitCid38 (T2); class CID38 slot7; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001836C8(s32 arg0) {
    func_00231100();
    func_00182C70(arg0 + 0xE0);
}
