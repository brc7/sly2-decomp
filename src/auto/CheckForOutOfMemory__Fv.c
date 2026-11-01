#include "m2c_prelude.h"
/** @sly2-annot addr=0x1AE1C8
 * Purpose: Empty out-of-memory hook (release build)
 * Info: proposed name CheckForOutOfMemory__Fv (EXISTING); TU memory
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void CheckForOutOfMemory__Fv(void) {

}
