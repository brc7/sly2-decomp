#include "m2c_prelude.h"
extern M2C_UNK (*D_002C60BC)() __asm__("get_eh_context");

/** @sly2-annot addr=0x23E398
 * Purpose: return (*get_eh_context)() [libgcc exception]
 * Info: proposed name __get_eh_context (T1); TU sdk_libs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void func_0023E398(void) {
    D_002C60BC();
}
