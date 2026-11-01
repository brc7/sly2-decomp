#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A6150();                            /* extern */
extern M2C_UNK D_003E13B4;

/** @sly2-annot addr=0x21D320
 * Purpose: OnLoRemove then RemoveDlEntry(&g_dlTarget D_003E13B4, this)
 * Info: proposed name OnTargetRemove (T1); class TARGET slot22; TU target
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources:
 *   arbitration_slots22_23+verdicts_tu_r7r8+verdicts_B
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#target-0x21d278-0x21d638
 * @end-sly2-annot */
void func_0021D320(s32 arg0) {
    func_001A6150();
    func_0011A2E8(&D_003E13B4, arg0);
}
