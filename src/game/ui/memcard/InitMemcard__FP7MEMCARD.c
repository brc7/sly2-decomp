#include "m2c_prelude.h"
s32 SemaCreate__Fii(M2C_UNK, M2C_UNK);              /* extern */

/** @sly2-annot addr=0x1AAED8
 * Purpose: MEMCARD init: SemaCreate(1,1), thread id -1, prio 0x20
 * Info: proposed name InitMemcard (T1); TU memcard
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void InitMemcard__FP7MEMCARD(void *arg0) {
    s32 temp_v0;

    temp_v0 = SemaCreate__Fii(1, 1);
    M2C_FIELD(arg0, s32 *, 0) = temp_v0;
    M2C_FIELD(arg0, s32 *, 4) = -1;
    M2C_FIELD(arg0, s32 *, 0x10) = 0x20;
}
