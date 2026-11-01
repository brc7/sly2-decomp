#include "m2c_prelude.h"
M2C_UNK StaticInit_ms(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1B4708
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$ms (T1); TU ms
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r5_1AE130
 * @end-sly2-annot */
void func_001B4708(void) {
    StaticInit_ms(1, 0xFFFF);
}
