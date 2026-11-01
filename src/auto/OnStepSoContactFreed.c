#include "m2c_prelude.h"
M2C_UNK func_001F5988();                            /* extern */

void OnStepSoContactFreed(void *arg0, s32 arg1) {
    void *temp_v0;

    func_001F5988();
    temp_v0 = M2C_FIELD(arg0, void **, 0x208);
    if ((temp_v0 != NULL) && (M2C_FIELD(temp_v0, s32 *, 0x48) == arg1)) {
        M2C_FIELD(arg0, void **, 0x208) = NULL;
    }
}
