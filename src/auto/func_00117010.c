#include "m2c_prelude.h"
M2C_UNK StaticInit_dynarray(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x117010
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$dynarray (T1); TU dynarray
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r1_116918
 * @end-sly2-annot */
void func_00117010(void) {
    StaticInit_dynarray(1, 0xFFFF);
}
