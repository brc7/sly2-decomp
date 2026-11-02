#include "m2c_prelude.h"
M2C_UNK StaticInit_cm(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x14FAA8
 * Purpose: compiler-generated global destructor stub (calls static init with (0,0xFFFF))
 * Info: proposed name _GLOBAL_$D$cm (T1); TU cm
 * Confidence: high [T1 0.95] - evidence: semantic review; sources: tu_map(structural)+r2_131F70
 * See: docs/engine/camera.md#1-cm-the-camera-singleton, docs/engine/camera.md#4-source-files,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_0014FAA8(void) {
    StaticInit_cm(0, 0xFFFF);
}
