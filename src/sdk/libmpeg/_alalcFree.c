#include "m2c_prelude.h"
/** @sly2-annot addr=0x24B920
 * Purpose: reset current to dynamic start [libmpeg mpeg.o]
 * Info: proposed name _alalcFree (T1); TU sdk_libs
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void _alalcFree(void *arg0) {
    M2C_FIELD(arg0, s32 *, 8) = (s32) M2C_FIELD(arg0, s32 *, 0xC);
}
