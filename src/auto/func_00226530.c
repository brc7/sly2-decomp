#include "m2c_prelude.h"
M2C_UNK StaticInit_turret(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x226530
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$turret (T1); TU turret
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void func_00226530(void) {
    StaticInit_turret(1, 0xFFFF);
}
