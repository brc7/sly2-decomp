#include "m2c_prelude.h"
M2C_UNK StaticInit_pipe(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1C3D50
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$pipe (T1); TU pipe
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r5_1AE130
 * @end-sly2-annot */
void func_001C3D50(void) {
    StaticInit_pipe(1, 0xFFFF);
}
