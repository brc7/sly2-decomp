#include "m2c_prelude.h"
s32 FBhvguardHasTargetList(void *arg0) {
    s32 var_a0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x10);
    var_a0 = 0;
    if ((M2C_FIELD(temp_v1, s32 *, 0xFA8) != 0) || (M2C_FIELD(temp_v1, s32 *, 0xF94) != 0)) {
        var_a0 = 1;
    }
    return var_a0;
}
