#include "m2c_prelude.h"
/** @sly2-annot addr=0x221958
 * Purpose: returns current TNFN pointer (0x118); base getter for TN splice properties
 * Info: proposed name PtnfnFromTn (T1); TU tn
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
s32 func_00221958(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x118);
}
