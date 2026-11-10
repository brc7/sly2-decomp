#include "m2c_prelude.h"
M2C_UNK _strtol_r(s32, s32, s32, s32);          /* extern */
extern s32 D_002C49D4 __asm__("_impure_ptr");

// Newlib strtol: _strtol_r(_impure_ptr,...). (libc strtol.o) SDK name strtol.
void strtol(s32 arg0, s32 arg1, s32 arg2) {
    _strtol_r(D_002C49D4, arg0, arg1, arg2);
}
