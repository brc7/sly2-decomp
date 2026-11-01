#include "m2c_prelude.h"
/** @sly2-annot addr=0x1CDE70
 * Purpose: copy emitb rate into RIPG
 * Info: proposed name SetRipgEmitb__FP4RIPGP5EMITB (EXISTING); TU rip
 * Notes: verifier: purpose partly wrong: Existing name = Sly 1 SetRipgEmitb, but the copied field is
 *   emitrip.sExpand (emitb+0xD4 -> ripg+0x158), not a rate.
 * Confidence: high [EXISTING 0.90] - evidence: Sly 1 / Lilly reference; pre-existing symbol in
 *   symbol_addrs.txt; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void SetRipgEmitb__FP4RIPGP5EMITB(void *arg0, void *arg1) {
    M2C_FIELD(arg0, f32 *, 0x158) = (f32) M2C_FIELD(arg1, f32 *, 0xD4);
}
