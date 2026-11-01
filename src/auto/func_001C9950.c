#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C9950
 * Purpose: idProxy>>16
 * Info: proposed name IsrcFromIdProxy (T1); TU proxy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
s32 func_001C9950(s32 arg0) {
    return arg0 >> 0x10;
}
