#include "m2c_prelude.h"
M2C_UNK StaticInit_exit_warp(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x228B28
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$exit_warp (T1); TU exit_warp
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void func_00228B28(void) {
    StaticInit_exit_warp(1, 0xFFFF);
}
