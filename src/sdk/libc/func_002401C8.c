#include "m2c_prelude.h"
extern s32 D_002C49D4 __asm__("_impure_ptr");

// Newlib __errno: returns &_impure_ptr->_errno (_errno at +0). (libc errno.o) SDK name __errno.
s32 func_002401C8(void) {
    return D_002C49D4;
}
