#include "m2c_prelude.h"
/** @sly2-annot addr=0x221940
 * Purpose: 0x118 = 0
 * Info: proposed name ClearTnTnfn (T2); TU tn
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00221940(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x118) = 0;
}
