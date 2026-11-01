#include "m2c_prelude.h"
/** @sly2-annot addr=0x221948
 * Purpose: returns *(0x118)
 * Info: proposed name GetTnTnfnFirstWord (T2); TU tn
 * Notes: agent qualifier: splice getter
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
s32 func_00221948(void *arg0) {
    return *M2C_FIELD(arg0, s32 **, 0x118);
}
