#include "m2c_prelude.h"
s32 PspareFromIcopy(void *arg0, s32 arg1) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xE0);
    return M2C_FIELD(temp_v1, s32 *, 0x68) + ((arg1 - M2C_FIELD(temp_v1, s32 *, 0xC)) * 0x50);
}
