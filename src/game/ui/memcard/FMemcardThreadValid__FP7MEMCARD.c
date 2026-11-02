#include "m2c_prelude.h"
/** @sly2-annot addr=0x1AB088
 * Purpose: Thread id (+4) != -1
 * Info: proposed name FMemcardThreadValid__FP7MEMCARD (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
s32 FMemcardThreadValid__FP7MEMCARD(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 4) != 0;
}
