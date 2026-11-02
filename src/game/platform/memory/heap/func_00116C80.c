#include "m2c_prelude.h"
M2C_UNK StaticInit_heap(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x116C80
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$heap (T1); TU heap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r1_116918
 * @end-sly2-annot */
void func_00116C80(void) {
    StaticInit_heap(1, 0xFFFF);
}
