#include "m2c_prelude.h"
M2C_UNK StaticInit_rumble(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1D5788
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$rumble (T1); TU rumble
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r6_1D5150
 * @end-sly2-annot */
void func_001D5788(void) {
    StaticInit_rumble(1, 0xFFFF);
}
