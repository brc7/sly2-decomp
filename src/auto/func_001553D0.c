#include "m2c_prelude.h"
M2C_UNK StaticInit_difficulty(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1553D0
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$difficulty (T1); TU difficulty
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r3_1539A8
 * @end-sly2-annot */
void func_001553D0(void) {
    StaticInit_difficulty(1, 0xFFFF);
}
