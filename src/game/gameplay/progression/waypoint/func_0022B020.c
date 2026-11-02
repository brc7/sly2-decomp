#include "m2c_prelude.h"
M2C_UNK StaticInit_waypoint(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x22B020
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$waypoint (T1); TU waypoint
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void func_0022B020(void) {
    StaticInit_waypoint(1, 0xFFFF);
}
