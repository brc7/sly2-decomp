#include "m2c_prelude.h"
/** @sly2-annot addr=0x1A6318
 * Purpose: Return vt31 PmatWorld()
 * Info: proposed name PmatLoWorld (T1); TU lo
 * Notes: names_v2 C proposed SetShadowFrustrumUp__FP6SHADOWP6VECTOR (0.438)
 * Confidence: high [T1 0.90] - evidence: static naming pass; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#7-xfm-and-transform-helpers, docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6318(void *arg0) {
    s16 temp_a1;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xC);
    temp_a1 = M2C_FIELD(temp_v1, s16 *, 0xF8);
    M2C_FIELD(temp_v1, M2C_UNK (**)(void *, s16), 0xFC)(arg0 + temp_a1, temp_a1);
}
