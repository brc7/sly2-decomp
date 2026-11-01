#include "m2c_prelude.h"
extern s32 D_002DEB40;

void SetSwJobCur(void *arg0, void *arg1) {
    M2C_FIELD(arg0, void **, 0x3B0C) = arg1;
    if (arg1 != NULL) {
        D_002DEB40 = M2C_FIELD(arg1, s32 *, 0x18);
        return;
    }
    D_002DEB40 = -1;
}
