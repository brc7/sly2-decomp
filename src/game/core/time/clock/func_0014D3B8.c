#include "m2c_prelude.h"
M2C_UNK StaticInit_clock(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x14D3B8
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$clock (T1); TU clock
 * Confidence: high [T1 0.95] - evidence: semantic review; sources: tu_map(structural)+semantic_tu_docs/clock
 * @end-sly2-annot */
void func_0014D3B8(void) {
    StaticInit_clock(1, 0xFFFF);
}
