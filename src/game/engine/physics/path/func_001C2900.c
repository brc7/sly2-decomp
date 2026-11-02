#include "m2c_prelude.h"
M2C_UNK StaticInit_path(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1C2900
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$path (T1); TU path
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r5b_rest
 * @end-sly2-annot */
void func_001C2900(void) {
    StaticInit_path(1, 0xFFFF);
}
