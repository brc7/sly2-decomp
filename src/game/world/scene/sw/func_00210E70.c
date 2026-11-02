#include "m2c_prelude.h"
M2C_UNK StaticInit_sw(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x210E70
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$sw (T1); TU sw
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7_1F8DD8
 * @end-sly2-annot */
void func_00210E70(void) {
    StaticInit_sw(1, 0xFFFF);
}
