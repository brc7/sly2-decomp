#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003E13D8;

/** @sly2-annot addr=0x1FF470
 * Purpose: vt22: remove from SW update array (0x1A6150) and from DL D_003E13D8
 * Info: proposed name OnCid39Remove (T2); class CID39 slot22; TU tu_1FF3B8
 * Notes: verifier WRONG: OnCid27Remove -> OnCid39Remove | verifier: purpose partly wrong: Slot 22 (OnRemove)
 *   of CID 0x27 = CID39, not table CID27 (0x1B): the same hex/decimal slip. The body is OnLoRemove 0x1A6150
 *   plus RemoveDlEntry(&D_003E13D8); 0x1A | slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object
 *   leaves/enters the active world, incl. proximity activation)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void func_001FF470(s32 arg0) {
    OnLoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E13D8, arg0);
}
