#include "m2c_prelude.h"
extern s32 D_002F6380;
extern s32 D_002F6384;

/** @sly2-annot addr=0x1AE300
 * Purpose: Reset world heap top to world heap min (level unload)
 * Info: proposed name FreeSw__Fv (EXISTING); TU memory
 * Confidence: high [EXISTING 0.97] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void FreeSw__Fv(void) {
    D_002F6384 = D_002F6380;
}
