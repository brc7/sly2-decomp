#include "m2c_prelude.h"
M2C_UNK StaticInit_wipe(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x22B700
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$wipe (T1); TU wipe
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void func_0022B700(void) {
    StaticInit_wipe(1, 0xFFFF);
}
