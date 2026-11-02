#include "m2c_prelude.h"
M2C_UNK StaticInit_sound(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1FF398
 * Purpose: compiler-generated global destructor stub (calls static init with (0,0xFFFF))
 * Info: proposed name _GLOBAL_$D$sound (T1); TU sound
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r7_1F8DD8
 * See: docs/engine/sound.md#5-source-files, docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_001FF398(void) {
    StaticInit_sound(0, 0xFFFF);
}
