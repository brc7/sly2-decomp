#include "m2c_prelude.h"
/** @sly2-annot addr=0x1F7BF0
 * Purpose: return +0xE8
 * Info: proposed name PsophysFromSo (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
s32 func_001F7BF0(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0xE8);
}
