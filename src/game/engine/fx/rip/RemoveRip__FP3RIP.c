#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(s32, void *);                 /* extern */
M2C_UNK func_001CE9D0(void *);                      /* extern */

/** @sly2-annot addr=0x1CE980
 * Purpose: unlink from RIPG dl + ReleaseRip
 * Info: proposed name RemoveRip__FP3RIP (EXISTING); TU rip
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void RemoveRip__FP3RIP(void *arg0) {
    RemoveDlEntry__FP2DLPv(M2C_FIELD(arg0, s32 *, 8) + 0x15C, arg0);
    func_001CE9D0(arg0);
}
