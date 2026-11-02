#include "m2c_prelude.h"
M2C_UNK StaticInit_sensor(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1DFAB8
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$sensor (T1); TU sensor
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r6b_rest
 * @end-sly2-annot */
void func_001DFAB8(void) {
    StaticInit_sensor(1, 0xFFFF);
}
