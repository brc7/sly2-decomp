#include "m2c_prelude.h"
M2C_UNK StaticInit_mark(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1AA958
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$mark (T1); TU mark
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r4b_rest
 * @end-sly2-annot */
void func_001AA958(void) {
    StaticInit_mark(1, 0xFFFF);
}
