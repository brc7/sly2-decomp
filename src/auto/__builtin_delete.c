#include "m2c_prelude.h"
/** @sly2-annot addr=0x1AE520
 * Purpose: operator delete no-op
 * Info: proposed name __builtin_delete (EXISTING); TU memory
 * Confidence: high [EXISTING 0.97] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668,
 *   docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void __builtin_delete(void) {

}
