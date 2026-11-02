#include "m2c_prelude.h"
M2C_UNK StaticInit_hnd(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x185110
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$hnd (T1); TU hnd
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r4b_rest
 * @end-sly2-annot */
void func_00185110(void) {
    StaticInit_hnd(1, 0xFFFF);
}
