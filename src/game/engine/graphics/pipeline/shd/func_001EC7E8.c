#include "m2c_prelude.h"
M2C_UNK memset(M2C_UNK *, M2C_UNK, M2C_UNK);        /* extern */
extern s32 D_003DA790;
extern s32 D_003DCFA0;
extern M2C_UNK D_003DCFA8;

/** @sly2-annot addr=0x1EC7E8
 * Purpose: Clear shader GS state (D_003DA790, D_003DCFA0, 0x40 bytes at D_003DCFA8)
 * Info: proposed name UnloadShaders (T2); TU shd
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001EC7E8(void) {
    D_003DA790 = 0;
    D_003DCFA0 = 0;
    memset(&D_003DCFA8, 0, 0x40);
}
