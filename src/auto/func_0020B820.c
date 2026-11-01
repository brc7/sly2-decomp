#include "m2c_prelude.h"
M2C_UNK StaticInit_stepguard(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x20B820
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$stepguard (T1); TU stepguard
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7b_guard
 * @end-sly2-annot */
void func_0020B820(void) {
    StaticInit_stepguard(1, 0xFFFF);
}
