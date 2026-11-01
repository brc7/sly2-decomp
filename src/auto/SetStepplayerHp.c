#include "m2c_prelude.h"
void SetStepplayerHp(void *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0xDFC);
    *M2C_FIELD(arg0, s32 **, 0xE00) = (arg1 >= temp_v1) ? temp_v1 : arg1;
}
