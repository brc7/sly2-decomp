#include "m2c_prelude.h"
M2C_UNK func_00258040(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x150DE8
 * Purpose: Slot 7: release the two sound refs (+0xC4/+0xC0)
 * Info: proposed name ResetCpzoom (T2); TU cplcy
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_00150DE8(s32 arg0) {
    func_00258040(arg0 + 0xC4, 0);
    func_00258040(arg0 + 0xC0, 0);
}
