#include "m2c_prelude.h"
extern s32 D_003E1C50;

void *PrecWorldEntry(s32 arg0) {
    s32 var_v0;
    void *temp_a0;

    if (arg0 == -1) {
        return NULL;
    }
    temp_a0 = D_003E1C50 + (arg0 << 6);
    var_v0 = 0;
    if (temp_a0 != NULL) {
        var_v0 = M2C_FIELD(temp_a0, s32 *, 4) != M2C_FIELD(temp_a0, s32 *, 0x3C);
    }
    return (var_v0 == 0) ? NULL : temp_a0;
}
