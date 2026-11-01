#include "m2c_prelude.h"
s32 PdetectorGetStepguard(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0xF64);
}
