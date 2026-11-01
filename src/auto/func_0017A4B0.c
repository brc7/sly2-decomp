#include "m2c_prelude.h"
M2C_UNK func_00174920();                            /* extern */

/** @sly2-annot addr=0x17A4B0
 * Purpose: BHVSLEEP Init: BHVANIM Init with flag 0x10, sub-state -1
 * Info: proposed name InitBhv175 (T2); class BHV175 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvsleep -> InitBhv175
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A4B0(void *arg0) {
    func_00174920();
    M2C_FIELD(arg0, s32 *, 0x3C) = 0x10;
    M2C_FIELD(arg0, s32 *, 0x50) = -1;
}
