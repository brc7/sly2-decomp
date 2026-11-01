#include "m2c_prelude.h"
M2C_UNK StaticInit_step(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x205E50
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$step (T1); TU step
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7b_guard
 * @end-sly2-annot */
void func_00205E50(void) {
    StaticInit_step(1, 0xFFFF);
}
