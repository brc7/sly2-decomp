#include "m2c_prelude.h"
M2C_UNK StaticInit_brx(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x13FF50
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$brx (T1); TU brx
 * Confidence: high [T1 0.95] - evidence: semantic review; sources: tu_map(structural)+r2_131F70
 * @end-sly2-annot */
void func_0013FF50(void) {
    StaticInit_brx(1, 0xFFFF);
}
