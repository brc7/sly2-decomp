#include "m2c_prelude.h"
M2C_UNK PloNew__F3CIDP2SO3OIDi(M2C_UNK, s32, M2C_UNK, M2C_UNK); /* extern */
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, M2C_UNK, s32); /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1F7B98
 * Purpose: If SO not found in g_sw list 0x187 (0x15E938) add it (0x13E3C8)
 * Info: proposed name EnsureSoInSwList187 (T2); TU so
 * Notes: names_v2 v1 proposed UProjectVector__FP6VECTORN20 (0.438)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7B98(s32 arg0) {
    if (PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 2, 0x187, arg0) == 0) {
        PloNew__F3CIDP2SO3OIDi(0x12, arg0, 0x187, -1);
    }
}
