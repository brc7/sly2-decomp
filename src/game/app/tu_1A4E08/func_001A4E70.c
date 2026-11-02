#include "m2c_prelude.h"
/** @sly2-annot addr=0x1A4E70
 * Purpose: cursor (+0x1C4) < count
 * Info: proposed name FBootSequencePending (T2); TU tu_1A4E08
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/world-sw.md#tu_1a4e08-0x1a4e08-0x1a5270
 * @end-sly2-annot */
s32 func_001A4E70(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x1C4) < M2C_FIELD(arg0, s32 *, 0);
}
