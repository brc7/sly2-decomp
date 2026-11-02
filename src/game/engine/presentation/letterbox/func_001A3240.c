#include "m2c_prelude.h"
M2C_UNK StaticInit_letterbox(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1A3240
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$letterbox (T1); TU letterbox
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r4b_rest
 * @end-sly2-annot */
void func_001A3240(void) {
    StaticInit_letterbox(1, 0xFFFF);
}
