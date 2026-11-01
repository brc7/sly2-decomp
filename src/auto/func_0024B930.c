#include "m2c_prelude.h"
M2C_UNK func_0024FA38(s32, M2C_UNK *);              /* extern */
extern M2C_UNK D_002AC418;

/** @sly2-annot addr=0x24B930
 * Purpose: aligned bump alloc; error "work area size is too small" [libmpeg mpeg.o]
 * Info: proposed name _alalcAlloc (T1); TU sdk_libs
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
s32 func_0024B930(s32 arg0, void *arg1, s32 arg2, u32 arg3) {
    s32 temp_v0;
    u32 temp_a0;

    temp_v0 = ((u32) ((M2C_FIELD(arg1, u32 *, 8) + arg3) - 1) / arg3) * arg3;
    temp_a0 = temp_v0 + arg2;
    if ((u32) (M2C_FIELD(arg1, s32 *, 0) + M2C_FIELD(arg1, s32 *, 4)) < temp_a0) {
        func_0024FA38(arg0, &D_002AC418);
        return 0;
    }
    M2C_FIELD(arg1, u32 *, 8) = temp_a0;
    return temp_v0;
}
