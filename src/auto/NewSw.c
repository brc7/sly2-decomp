#include "m2c_prelude.h"
s32 func_001AE4F8(M2C_UNK);                         /* extern */
M2C_UNK SW_ctor(s32);                         /* extern */

/** @sly2-annot addr=0x110AB0
 * Purpose: pfnNew factory for SW (cid 1): allocates 0x4790 bytes (operator new 0x1AE4F8), calls ctor SW_ctor
 * Info: proposed name NewSw (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/engine/object-model.md#1-class-descriptors-and-vtables,
 *   docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void NewSw(void) {
    SW_ctor(func_001AE4F8(0x4790));
}
