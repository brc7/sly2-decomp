#include "m2c_prelude.h"
M2C_UNK StaticInit_proxy(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1CB8E8
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$proxy (T1); TU proxy
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r5_1AE130
 * @end-sly2-annot */
void func_001CB8E8(void) {
    StaticInit_proxy(1, 0xFFFF);
}
