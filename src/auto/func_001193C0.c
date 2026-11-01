#include "m2c_prelude.h"
/** @sly2-annot addr=0x1193C0
 * Purpose: Stores normal qword and d at +0xC
 * Info: proposed name SetPlane (T1); TU bsp
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#bsp-0x118668-0x1195e0
 * @end-sly2-annot */
void func_001193C0(void *arg0, s128 arg1, f32 fparg0) {
    M2C_FIELD(arg0, s128 *, 0) = arg1;
    M2C_FIELD(arg0, f32 *, 0xC) = fparg0;
}
