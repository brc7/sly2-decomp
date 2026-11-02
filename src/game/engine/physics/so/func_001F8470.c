#include "m2c_prelude.h"
void *func_001F8300();                              /* extern */

/** @sly2-annot addr=0x1F8470
 * Purpose: record.g4 = f
 * Info: proposed name SetSoPhysParam4 (T2); TU so
 * Notes: names_v2 v1 proposed SetSoCameraStyle__FP2SO3CMK (0.043)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8470(f32 fparg0) {
    void *temp_v0;

    temp_v0 = func_001F8300();
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, f32 *, 4) = fparg0;
    }
}
