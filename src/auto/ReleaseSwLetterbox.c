#include "m2c_prelude.h"
M2C_UNK func_001A30D0(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002E1E60;

void ReleaseSwLetterbox(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x3D80) - 1;
    M2C_FIELD(arg0, s32 *, 0x3D80) = temp_v0;
    if (temp_v0 == 0) {
        func_001A30D0(&D_002E1E60);
    }
}
