#include "m2c_prelude.h"
M2C_UNK StaticInit_water(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x229ED0
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$water (T1); TU water
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void func_00229ED0(void) {
    StaticInit_water(1, 0xFFFF);
}
