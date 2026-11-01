#include "m2c_prelude.h"
M2C_UNK func_0011A2E8(M2C_UNK *, void *);           /* extern */
M2C_UNK func_001F4BA8();                            /* extern */
extern M2C_UNK D_003E11C4;

/** @sly2-annot addr=0x1F4D50
 * Purpose: Inverse of MarkSoTreeDirty (clear bits 0/1, remove from both lists)
 * Info: proposed name ClearSoTreeDirty (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4D50(void *arg0) {
    s64 temp_a1;
    void *temp_v1;

    func_001F4BA8();
    temp_v1 = M2C_FIELD(arg0, void **, 0xEC);
    if (temp_v1 != NULL) {
        temp_a1 = M2C_FIELD(temp_v1, s64 *, 0x70);
        if (temp_a1 & 2) {
            M2C_FIELD(temp_v1, s64 *, 0x70) = (s64) (temp_a1 & ~2);
            func_0011A2E8(&D_003E11C4, M2C_FIELD(arg0, void **, 0xEC));
        }
    }
}
