#include "m2c_prelude.h"
f32 func_00154C90(M2C_UNK *);                       /* extern */
extern M2C_UNK D_002DE578;

void *RefOpGetDifficulty(void *arg0) {
    M2C_FIELD(arg0, f32 *, 4) = func_00154C90(&D_002DE578);
    M2C_FIELD(arg0, s32 *, 0) = 0x16;
    return arg0;
}
