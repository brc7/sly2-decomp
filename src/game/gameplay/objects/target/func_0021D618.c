#include "m2c_prelude.h"
M2C_UNK StaticInit_target(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x21D618
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$target (T1); TU target
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void func_0021D618(void) {
    StaticInit_target(1, 0xFFFF);
}
