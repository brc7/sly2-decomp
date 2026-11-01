#include "m2c_prelude.h"
/** @sly2-annot addr=0x1A62E8
 * Purpose: Return vt31 PmatWorld()->pos (row 3, +0x30)
 * Info: proposed name PosLoWorld (T1); TU lo
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#7-xfm-and-transform-helpers, docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
s128 func_001A62E8(void *arg0) {
    s16 temp_a1;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xC);
    temp_a1 = M2C_FIELD(temp_v1, s16 *, 0xF8);
    return M2C_FIELD(M2C_FIELD(temp_v1, void *(**)(void *, s16), 0xFC)(arg0 + temp_a1, temp_a1), s128 *, 0x30);
}
