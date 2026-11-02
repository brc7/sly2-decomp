#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(s32, M2C_UNK);                    /* extern */
M2C_UNK func_00184130();                            /* extern */
M2C_UNK StartMurrayLiftGrabspot(s32, s32);                    /* extern */

/** @sly2-annot addr=0x184330
 * Purpose: TriggerCid35 then, if the grabber is MURRAY, start his lift (0x1B7000)
 * Info: proposed name Cid35OnGrab (T2); class CID36 slot51; TU grabspot
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#9-grabbable-world-objects,
 *   docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00184330(s32 arg0, s32 arg1) {
    func_00184130();
    if (FIsBasicDerivedFrom__FP5BASIC3CID(arg1, 0x36) != 0) {
        StartMurrayLiftGrabspot(arg1, arg0);
    }
}
