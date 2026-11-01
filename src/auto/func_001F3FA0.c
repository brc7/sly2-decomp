#include "m2c_prelude.h"
/** @sly2-annot addr=0x1F3FA0
 * Purpose: SO vt31: &pxfRoot(+0x60)->matWorld (+0x40)
 * Info: proposed name SO_PmatWorld (T1); class SO slot31; TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/formats/meshes-scene.md#3-transforms-cstatic,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
s32 func_001F3FA0(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x60) + 0x40;
}
