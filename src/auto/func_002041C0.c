#include "m2c_prelude.h"
s128 func_001A6F00(void *, M2C_UNK, s128);          /* extern */
M2C_UNK func_001F40A0();                            /* extern */

/** @sly2-annot addr=0x2041C0
 * Purpose: STEP vt29: TranslateSoToPos then refresh world base position +0x230 from local +0x240
 * Info: proposed name TranslateStepToPos (T1); class STEP slot29; TU step
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_002041C0(void *arg0) {
    func_001F40A0();
    M2C_FIELD(arg0, s128 *, 0x230) = func_001A6F00(arg0, 0, M2C_FIELD(arg0, s128 *, 0x240));
}
