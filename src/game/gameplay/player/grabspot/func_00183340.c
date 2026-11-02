#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitXfm();                            /* extern */

/** @sly2-annot addr=0x183340
 * Purpose: CID34 vt7: InitXfm + InitGrabspot(+0xE0)
 * Info: proposed name InitCid34 (T2); class CID34 slot7; TU grabspot
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183340(s32 arg0) {
    InitXfm();
    InitGrabspot(arg0 + 0xE0);
}
