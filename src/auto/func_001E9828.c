#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */

/** @sly2-annot addr=0x1E9828
 * Purpose: Copy the shape's dist array into the chain's
 * Info: proposed name SyncChainDists (T2); TU shape
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E9828(void *arg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x48);
    CopyAb__FPvT0Ui(M2C_FIELD(arg0, s32 *, 0x5C), M2C_FIELD(temp_v0, s32 *, 0xF0), M2C_FIELD(temp_v0, s32 *, 0xE8) * 4);
}
