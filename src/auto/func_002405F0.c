#include "m2c_prelude.h"
extern M2C_UNK D_002C66F0;

/** @sly2-annot addr=0x2405F0
 * Purpose: return &gs param block (D_002C66F0) [libgraph graph001.o]
 * Info: proposed name sceGsGetGParam (T1); TU sdk_libs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
M2C_UNK *func_002405F0(void) {
    return &D_002C66F0;
}
