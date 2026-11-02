#include "m2c_prelude.h"
M2C_UNK StaticInit_sw(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x210E90
 * Purpose: compiler-generated global destructor stub (calls static init with (0,0xFFFF))
 * Info: proposed name _GLOBAL_$D$sw (T1); TU sw
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7_1F8DD8
 * See: docs/engine/world-sw.md#9-source-files, docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_00210E90(void) {
    StaticInit_sw(0, 0xFFFF);
}
