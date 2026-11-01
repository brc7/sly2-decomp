#include "m2c_prelude.h"
M2C_UNK PloAcquireProxy(s32, M2C_UNK);                /* extern */
s32 func_001CB2E0();                                /* extern */

/** @sly2-annot addr=0x1CB328
 * Purpose: if no current copy acquire one
 * Info: proposed name EnsureProxyCopy (T1); TU proxy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void EnsureProxyCopy(s32 arg0) {
    if (func_001CB2E0() == 0) {
        PloAcquireProxy(arg0, 1);
    }
}
