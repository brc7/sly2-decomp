#include "m2c_prelude.h"
s32 func_0011A380(s32, void *);                     /* extern */

/** @sly2-annot addr=0x1E5778
 * Purpose: True if the JSG is no longer in its relay's dlJsg
 * Info: proposed name FJsgRemoved (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
s32 func_001E5778(void *arg0) {
    return func_0011A380(M2C_FIELD(arg0, s32 *, 0x14) + 0x5C, arg0) == 0;
}
