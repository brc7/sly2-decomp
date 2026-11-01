#include "m2c_prelude.h"
s32 FBhvanimDone(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x30) == 1;
}
