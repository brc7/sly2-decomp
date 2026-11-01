#include "m2c_prelude.h"
/** @sly2-annot addr=0x24B610
 * Purpose: return mp->sys->isEnd [libmpeg mpeg.o]
 * Info: proposed name sceMpegIsEnd (T1); TU sdk_libs
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
s32 func_0024B610(void *arg0) {
    return *M2C_FIELD(arg0, s32 **, 0x40);
}
