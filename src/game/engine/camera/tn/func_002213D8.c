#include "m2c_prelude.h"
M2C_UNK OnLoRemove();                            /* extern */
M2C_UNK func_00221178(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x2213D8
 * Purpose: OnLoRemove then SetTnTns(Out)
 * Info: proposed name OnTnRemove (T1); class TN slot22; TU tn
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: arbitration_slots22_23+verdicts_tu_r7r8
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_002213D8(s32 arg0) {
    OnLoRemove();
    func_00221178(arg0, 0);
}
