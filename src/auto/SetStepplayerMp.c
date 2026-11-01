#include "m2c_prelude.h"
void SetStepplayerMp(void *arg0, s32 arg1) {
    s32 *temp_a2;
    s32 temp_v1;

    temp_a2 = M2C_FIELD(arg0, s32 **, 0x12FC);
    if (temp_a2 != NULL) {
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x1300);
        *temp_a2 = (arg1 >= temp_v1) ? temp_v1 : arg1;
    }
}
