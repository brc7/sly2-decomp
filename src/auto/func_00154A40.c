#include "m2c_prelude.h"
M2C_UNK StaticInit_detector(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x154A40
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$detector (T1); TU detector
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r3_1539A8
 * @end-sly2-annot */
void func_00154A40(void) {
    StaticInit_detector(1, 0xFFFF);
}
