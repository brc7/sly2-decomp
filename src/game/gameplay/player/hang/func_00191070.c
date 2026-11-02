#include "m2c_prelude.h"
M2C_UNK StaticInit_hang(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x191070
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$hang (T1); TU hang
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r4b_rest
 * @end-sly2-annot */
void func_00191070(void) {
    StaticInit_hang(1, 0xFFFF);
}
