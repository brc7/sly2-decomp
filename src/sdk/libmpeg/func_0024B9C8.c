#include "m2c_prelude.h"
M2C_UNK func_00254A70(s32);                         /* extern */

/** @sly2-annot addr=0x24B9C8
 * Purpose: default restart-DMA callback: sceIpuRestartDMA [libmpeg defhandler.o]
 * Info: proposed name _defRestartDMA (T1); TU sdk_libs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
s32 func_0024B9C8(void *arg0) {
    func_00254A70(M2C_FIELD(arg0, s32 *, 0x40) + 0x68);
    return 1;
}
