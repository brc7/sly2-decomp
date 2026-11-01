#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003E13CC;

/** @sly2-annot addr=0x1CB9B0
 * Purpose: RAIL vt23: LO base (0x1A6178) then append to global rail list D_003E13CC
 * Info: proposed name OnRailAdd (T1); class RAIL slot23; TU rail
 * Notes: names_v2 B proposed OnHpntAdd__FP4HPNT (0.923) | slot 23 OnAdd resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources: arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void func_001CB9B0(s32 arg0) {
    OnLoAdd();
    AppendDlEntry__FP2DLPv(&D_003E13CC, arg0);
}
