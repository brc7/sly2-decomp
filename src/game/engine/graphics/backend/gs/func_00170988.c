#include "m2c_prelude.h"
M2C_UNK StaticInit_gs(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x170988
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$gs (T1); TU gs
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r3_1539A8
 * @end-sly2-annot */
void func_00170988(void) {
    StaticInit_gs(1, 0xFFFF);
}
