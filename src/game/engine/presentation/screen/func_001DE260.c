#include "m2c_prelude.h"
M2C_UNK StaticInit_screen(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1DE260
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$screen (T1); TU screen
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r6b_rest
 * @end-sly2-annot */
void func_001DE260(void) {
    StaticInit_screen(1, 0xFFFF);
}
