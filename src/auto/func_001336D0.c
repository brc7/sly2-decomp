#include "m2c_prelude.h"
M2C_UNK StaticInit_basic(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1336D0
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$basic (T1); TU basic
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r2b_rest
 * @end-sly2-annot */
void func_001336D0(void) {
    StaticInit_basic(1, 0xFFFF);
}
