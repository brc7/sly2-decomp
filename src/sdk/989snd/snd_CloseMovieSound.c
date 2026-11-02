#include "m2c_prelude.h"
M2C_UNK snd_SendIOPCommandAndWait__FiiPc(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */

/** @sly2-annot addr=0x232F60
 * Purpose: 989snd API
 * Info: proposed name snd_CloseMovieSound (T1); TU 989snd
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8+verdicts_A2
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#989snd-0x2319e8-0x233000
 * @end-sly2-annot */
void snd_CloseMovieSound(void) {
    snd_SendIOPCommandAndWait__FiiPc(0x3C, 0, 0);
}
