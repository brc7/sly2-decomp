#include "m2c_prelude.h"
M2C_UNK func_0024FA38(s32, M2C_UNK *) __asm__("_sceMpegError");              /* extern */
extern M2C_UNK D_002AC418;

// Aligned bump alloc; error "work area size is too small". (libmpeg mpeg.o) SDK name _alalcAlloc.
s32 _alalcAlloc(s32 arg0, void *arg1, s32 arg2, u32 arg3) {
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
