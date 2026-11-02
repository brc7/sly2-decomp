#include "m2c_prelude.h"
M2C_UNK StaticInit_memory(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1AE7B0
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$memory (T1); TU memory
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r5_1AE130
 * @end-sly2-annot */
void func_001AE7B0(void) {
    StaticInit_memory(1, 0xFFFF);
}
