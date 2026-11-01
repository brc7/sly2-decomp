#include "m2c_prelude.h"
s32 GetPoCtlState(void *arg0) {
    s32 var_v0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x150);
    var_v0 = -1;
    if (temp_v1 != NULL) {
        var_v0 = M2C_FIELD(temp_v1, s32 *, 0x80);
    }
    return var_v0;
}
