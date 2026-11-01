#include "m2c_prelude.h"
s32 func_001AE4F8(M2C_UNK);                         /* extern */
M2C_UNK func_00211C58(s32);                         /* extern */

/** @sly2-annot addr=0x112CD0
 * Purpose: pfnNew factory for WIDGET130 (cid 130): allocates 0x130 bytes (operator new 0x1AE4F8), calls ctor
 *   WIDGET130_ctor
 * Info: proposed name NewWidget130 (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void func_00112CD0(void) {
    func_00211C58(func_001AE4F8(0x130));
}
