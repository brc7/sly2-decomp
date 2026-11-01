#include "m2c_prelude.h"
M2C_UNK StaticInit_splicemap(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1FFCD8
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$splicemap (T1); TU splicemap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7_1F8DD8
 * @end-sly2-annot */
void func_001FFCD8(void) {
    StaticInit_splicemap(1, 0xFFFF);
}
