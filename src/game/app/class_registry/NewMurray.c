#include "m2c_prelude.h"
s32 func_001AE4F8(M2C_UNK);                         /* extern */
M2C_UNK MURRAY_ctor(s32);                         /* extern */

/** @sly2-annot addr=0x111700
 * Purpose: pfnNew factory for MURRAY (cid 54): allocates 0x1B00 bytes (operator new 0x1AE4F8), calls ctor
 *   MURRAY_ctor
 * Info: proposed name NewMurray (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void NewMurray(void) {
    MURRAY_ctor(func_001AE4F8(0x1B00));
}
