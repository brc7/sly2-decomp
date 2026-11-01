#include "m2c_prelude.h"
M2C_UNK StaticInit_throw(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x220D20
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$throw (T1); TU throw
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void func_00220D20(void) {
    StaticInit_throw(1, 0xFFFF);
}
