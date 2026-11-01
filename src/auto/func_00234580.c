#include "m2c_prelude.h"
M2C_UNK func_00234350(s32, s32, s32, s32);          /* extern */
extern s32 D_002C49D4;

/** @sly2-annot addr=0x234580
 * Purpose: newlib strtol: _strtol_r(_impure_ptr,...) [libc strtol.o]
 * Info: proposed name strtol (T1); TU sdk_libs
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void func_00234580(s32 arg0, s32 arg1, s32 arg2) {
    func_00234350(D_002C49D4, arg0, arg1, arg2);
}
