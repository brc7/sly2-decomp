#include "m2c_prelude.h"
/** @sly2-annot addr=0x1CF5C8
 * Purpose: Returns 0x110 (largest RIP subclass size) for the rip pool
 * Info: proposed name CbMaxRip (T1); TU rip
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
s32 func_001CF5C8(void) {
    return 0x110;
}
