#include "m2c_prelude.h"
/** @sly2-annot addr=0x24B910
 * Purpose: mark current position as dynamic start [libmpeg mpeg.o]
 * Info: proposed name _alalcSetDynamic (T1); TU sdk_libs
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void func_0024B910(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xC) = (s32) M2C_FIELD(arg0, s32 *, 8);
}
