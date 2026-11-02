#include "m2c_prelude.h"
s32 PvNewFrame();                                /* extern */
M2C_UNK FramePtr_Assign(void *, s32);                 /* extern */
M2C_UNK func_00259A88(s32);                         /* extern */

/** @sly2-annot addr=0x1FF7E8
 * Purpose: Create a root CFrame (0x256DD0), install it (0x256EE0), Incref (0x259A88)
 * Info: proposed name InitSwSpliceFrame (T2); TU splicemap
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/engine/splice-scripting.md#6-source-files,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file
 * @end-sly2-annot */
void func_001FF7E8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x3A14) = 1;
    FramePtr_Assign(arg0, PvNewFrame());
    func_00259A88(M2C_FIELD(arg0, s32 *, 0));
}
