#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003E13B4;

/** @sly2-annot addr=0x21D358
 * Purpose: OnLoAdd then AppendDlEntry(&g_dlTarget, this)
 * Info: proposed name OnTargetAdd (T1); class TARGET slot23; TU target
 * Notes: names_v2 B proposed OnHshapeRemove__FP6HSHAPE (0.746) | slot 23 OnAdd resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources:
 *   arbitration_slots22_23+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#target-0x21d278-0x21d638
 * @end-sly2-annot */
void OnTargetAdd(s32 arg0) {
    OnLoAdd();
    AppendDlEntry__FP2DLPv(&D_003E13B4, arg0);
}
