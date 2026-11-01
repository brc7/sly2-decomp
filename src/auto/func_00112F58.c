#include "m2c_prelude.h"
s32 func_001AE4F8(M2C_UNK);                         /* extern */
M2C_UNK func_0020C058(s32);                         /* extern */

/** @sly2-annot addr=0x112F58
 * Purpose: pfnNew factory for LEAP (cid 145): allocates 0x70 bytes (operator new 0x1AE4F8), calls ctor
 *   LEAP_ctor
 * Info: proposed name NewLeap (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void func_00112F58(void) {
    func_0020C058(func_001AE4F8(0x70));
}
