#include "m2c_prelude.h"
f32 SMaxShape(void *arg0) {
    return M2C_FIELD(((M2C_FIELD(arg0, s32 *, 0xE8) * 4) + M2C_FIELD(arg0, s32 *, 0xF0)), f32 *, -4);
}
