#include "m2c_prelude.h"
M2C_UNK StaticInit_so(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1F8DB8
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$so (T1); TU so
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r6_1D5150
 * @end-sly2-annot */
void func_001F8DB8(void) {
    StaticInit_so(1, 0xFFFF);
}
