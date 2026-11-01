#include "m2c_prelude.h"
extern s32 D_002DD5B8;

/** @sly2-annot addr=0x12AE18
 * Purpose: Sets Splice-active flag D_002DD5B8 = 1
 * Info: proposed name StartupSplice (T1); TU spliceutils
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources:
 *   r1_116918+verdicts_tu_r1r2+verdicts_B
 * See: docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#spliceutils-0x12ae18-0x12b0e8,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_0012AE18(void) {
    D_002DD5B8 = 1;
}
