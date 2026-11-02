#include "m2c_prelude.h"
/** @sly2-annot addr=0x1CE9C0
 * Purpose: cRef++
 * Info: proposed name AddRipRef__FP3RIP (EXISTING); TU rip
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void AddRipRef__FP3RIP(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = (s32) (M2C_FIELD(arg0, s32 *, 4) + 1);
}
