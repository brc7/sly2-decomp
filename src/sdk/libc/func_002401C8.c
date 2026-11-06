#include "m2c_prelude.h"
extern s32 D_002C49D4 __asm__("_impure_ptr");

/** @sly2-annot addr=0x2401C8
 * Purpose: newlib __errno: returns &_impure_ptr->_errno (_errno at +0) [libc errno.o]
 * Info: proposed name __errno (T1); TU sdk_libs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
s32 func_002401C8(void) {
    return D_002C49D4;
}
