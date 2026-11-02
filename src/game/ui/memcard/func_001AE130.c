#include "m2c_prelude.h"
M2C_UNK StaticInit_memcard(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1AE130
 * Purpose: compiler-generated global destructor stub (calls static init with (0,0xFFFF))
 * Info: proposed name _GLOBAL_$D$memcard (T1); TU memcard
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r5_1AE130
 * See: docs/engine/core-library.md#1-memory, docs/engine/core-library.md#6-source-files,
 *   docs/engine/save-memcard.md#4-source-files
 * @end-sly2-annot */
void func_001AE130(void) {
    StaticInit_memcard(0, 0xFFFF);
}
