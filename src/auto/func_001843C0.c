#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitXfm();                            /* extern */

/** @sly2-annot addr=0x1843C0
 * Purpose: CID27 vt7: InitXfm + InitGrabspot(+0x130)
 * Info: proposed name InitCid27 (T2); class CID27 slot7; TU grabspot
 * Notes: names_v2 C proposed OnHbskRemove__FP4HBSK (0.638)
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001843C0(s32 arg0) {
    InitXfm();
    InitGrabspot(arg0 + 0x130);
}
