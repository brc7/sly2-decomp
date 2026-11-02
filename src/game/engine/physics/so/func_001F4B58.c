#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_003E11B8;

/** @sly2-annot addr=0x1F4B58
 * Purpose: Set tree flag bit0 and add tree to dirty list D_003E11B8
 * Info: proposed name MarkSoTreeBoundsDirty (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4B58(void *arg0) {
    s64 temp_a0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xEC);
    if (temp_v1 != NULL) {
        temp_a0 = M2C_FIELD(temp_v1, s64 *, 0x70);
        if (!(temp_a0 & 1)) {
            M2C_FIELD(temp_v1, s64 *, 0x70) = (s64) (temp_a0 | 1);
            AppendDlEntry__FP2DLPv(&D_003E11B8, M2C_FIELD(arg0, void **, 0xEC));
        }
    }
}
