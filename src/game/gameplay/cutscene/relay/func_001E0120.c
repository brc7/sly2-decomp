#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E0120
 * Purpose: calarm=0
 * Info: proposed name ClearRelayAlarms (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0120(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x84) = 0;
}
