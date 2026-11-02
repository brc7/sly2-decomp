#include "m2c_prelude.h"
M2C_UNK StaticInit_screen(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x1DE280
 * Purpose: compiler-generated global destructor stub (calls static init with (0,0xFFFF))
 * Info: proposed name _GLOBAL_$D$screen (T1); TU screen
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r6b_rest
 * See: docs/engine/cutscenes.md#open-questions, docs/engine/hud-ui.md#4-source-files,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_001DE280(void) {
    StaticInit_screen(0, 0xFFFF);
}
