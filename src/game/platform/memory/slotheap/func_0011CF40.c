#include "m2c_prelude.h"
M2C_UNK StaticInit_slotheap(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x11CF40
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$slotheap (T1); TU slotheap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r1_116918
 * @end-sly2-annot */
void func_0011CF40(void) {
    StaticInit_slotheap(1, 0xFFFF);
}
