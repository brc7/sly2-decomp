#include "m2c_prelude.h"
s32 func_00210B68();                                /* extern */

/** @sly2-annot addr=0x123BA0
 * Purpose: S32 = 0x210B68() which always returns 0
 * Info: proposed name RefOpIsDemo (T2); TU bif
 * Notes: v3: superseded RefOpGetStub104 | splice builtin 104 zero-104~ -> demo?~ (0.25): Body Int(FSwStub0())
 *   - FSwStub0 0x210B68 always returns 0 in retail. Scripts gate extra HUD hint tickers on (= (zero-104~) 1)
 *   (texts 749-754). A build flag that
 * Confidence: low (draft) [T2 0.25] - evidence: semantic review; sources: r1_116918+top50_round(splice
 *   builtin 104)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void *func_00123BA0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = func_00210B68();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
