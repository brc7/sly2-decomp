#include "m2c_prelude.h"
M2C_UNK StaticInit_crt0_splicewrap(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1147B8
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$crt0_splicewrap (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r0_thunks
 * @end-sly2-annot */
void func_001147B8(void) {
    StaticInit_crt0_splicewrap(1, 0xFFFF);
}
