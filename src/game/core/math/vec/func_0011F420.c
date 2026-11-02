#include "m2c_prelude.h"
M2C_UNK StaticInit_vec(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x11F420
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$vec (T1); TU vec
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r1_116918
 * @end-sly2-annot */
void func_0011F420(void) {
    StaticInit_vec(1, 0xFFFF);
}
