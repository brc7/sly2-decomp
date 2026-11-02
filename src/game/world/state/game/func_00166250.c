#include "m2c_prelude.h"
M2C_UNK StaticInit_game(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x166250
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$game (T1); TU game
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r3b_rest
 * @end-sly2-annot */
void func_00166250(void) {
    StaticInit_game(1, 0xFFFF);
}
