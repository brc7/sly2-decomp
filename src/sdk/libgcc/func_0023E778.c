#include "m2c_prelude.h"
/** @sly2-annot addr=0x23E778
 * Purpose: return table->lang.version (+6) [libgcc exception]
 * Info: proposed name __get_eh_table_version (T1); TU sdk_libs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
s16 func_0023E778(void *arg0) {
    return M2C_FIELD(arg0, s16 *, 6);
}
