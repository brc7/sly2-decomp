#include "m2c_prelude.h"
M2C_UNK StaticInit_memcard(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1AE110
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$memcard (T1); TU memcard
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r4_17F090
 * @end-sly2-annot */
void func_001AE110(void) {
    StaticInit_memcard(1, 0xFFFF);
}
