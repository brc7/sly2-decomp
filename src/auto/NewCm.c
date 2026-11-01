#include "m2c_prelude.h"
M2C_UNK func_0014D3D8(s32);                         /* extern */
s32 func_001AE4F8(M2C_UNK);                         /* extern */

/** @sly2-annot addr=0x112DA8
 * Purpose: pfnNew factory for CM (cid 135): allocates 0x630 bytes (operator new 0x1AE4F8), calls ctor CM_ctor
 * Info: proposed name NewCm (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void NewCm(void) {
    func_0014D3D8(func_001AE4F8(0x630));
}
