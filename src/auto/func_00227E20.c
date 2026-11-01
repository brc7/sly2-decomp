#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A6178();                            /* extern */
extern M2C_UNK D_003E1480;

/** @sly2-annot addr=0x227E20
 * Purpose: OnLoAdd + AppendDlEntry(&D_003E1480)
 * Info: proposed name OnCid23Add (T2); class CID23 slot23; TU vol
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23+verdicts_tu_r7r8
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#vol-0x227bb8-0x227fe8
 * @end-sly2-annot */
void func_00227E20(s32 arg0) {
    func_001A6178();
    func_0011A220(&D_003E1480, arg0);
}
