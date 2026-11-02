#include "m2c_prelude.h"
M2C_UNK StaticInit_glob(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x16F128
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$glob (T1); TU glob
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r3_1539A8
 * @end-sly2-annot */
void func_0016F128(void) {
    StaticInit_glob(1, 0xFFFF);
}
