#include "m2c_prelude.h"
/** @sly2-annot addr=0x1F8238
 * Purpose: +0xB4 = 0
 * Info: proposed name ClearSoB4 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8238(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xB4) = 0;
}
