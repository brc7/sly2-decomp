#include "m2c_prelude.h"
/** @sly2-annot addr=0x1D57A8
 * Purpose: *p = 0 (empty stack)
 * Info: proposed name InitIntStack (T2); TU tu_1D57A8
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/main-loop-startup.md#7-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
s32 *func_001D57A8(s32 *arg0) {
    *arg0 = 0;
    return arg0;
}
