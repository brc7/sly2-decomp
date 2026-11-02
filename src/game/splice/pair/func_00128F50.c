#include "m2c_prelude.h"
M2C_UNK StaticInit_pair(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x128F50
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$pair (T1); TU pair
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r1_116918
 * @end-sly2-annot */
void func_00128F50(void) {
    StaticInit_pair(1, 0xFFFF);
}
