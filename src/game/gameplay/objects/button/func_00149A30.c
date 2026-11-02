#include "m2c_prelude.h"
M2C_UNK StaticInit_button(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x149A30
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$button (T1); TU button
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r2b_rest
 * @end-sly2-annot */
void func_00149A30(void) {
    StaticInit_button(1, 0xFFFF);
}
