#include "m2c_prelude.h"
M2C_UNK func_00231100();                            /* extern */

/** @sly2-annot addr=0x1E5D80
 * Purpose: SHAPE::Init (vt7): InitXfm; cached segment(+0x148)=-1; +0x178=1
 * Info: proposed name InitShape (T1); class SHAPE slot7; TU shape
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#4-source-files, docs/engine/physics-collision.md#4-geometry-and-queries,
 *   docs/engine/physics-collision.md#6-source-files
 * @end-sly2-annot */
void func_001E5D80(void *arg0) {
    func_00231100();
    M2C_FIELD(arg0, s32 *, 0x148) = -1;
    M2C_FIELD(arg0, s32 *, 0x178) = 1;
}
