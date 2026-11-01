#include "m2c_prelude.h"
s32 GetPoCtlState();                                /* extern */

s32 func_001C4098(void) {
    return ~GetPoCtlState() != 0;
}
