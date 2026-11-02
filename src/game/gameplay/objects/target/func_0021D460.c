#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003E1474;

/** @sly2-annot addr=0x21D460
 * Purpose: OnLoAdd + AppendDlEntry(&D_003E1474)
 * Info: proposed name OnCid21Add (T2); class CID21 slot23; TU target
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#target-0x21d278-0x21d638
 * @end-sly2-annot */
void func_0021D460(s32 arg0) {
    OnLoAdd();
    AppendDlEntry__FP2DLPv(&D_003E1474, arg0);
}
