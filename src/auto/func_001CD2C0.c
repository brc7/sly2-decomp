#include "m2c_prelude.h"
M2C_UNK ReleaseAct(void *);                      /* extern */

void func_001CD2C0(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0xA0) != 0) {
        ReleaseAct(arg0 + 0x94);
        ReleaseAct(arg0 + 0x98);
        ReleaseAct(arg0 + 0x9C);
        M2C_FIELD(arg0, s32 *, 0xA0) = 0;
    }
}
