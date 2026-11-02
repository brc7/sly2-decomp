#include "m2c_prelude.h"
M2C_UNK func_001DFEB8(s32);                         /* extern */

/** @sly2-annot addr=0x1E53D8
 * Purpose: Stop the relay this script fired and is waiting on (+0x50) and clear it
 * Info: proposed name ClearJsgRelayWait (T1); TU relay
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E53D8(void *arg0) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x50);
    if (temp_a0 != 0) {
        func_001DFEB8(temp_a0);
        M2C_FIELD(arg0, s32 *, 0x50) = 0;
    }
}
