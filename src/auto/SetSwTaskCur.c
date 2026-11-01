#include "m2c_prelude.h"
extern s32 D_002DEB44;

void SetSwTaskCur(void *arg0, void *arg1) {
    M2C_FIELD(arg0, void **, 0x3B10) = arg1;
    if (arg1 != NULL) {
        D_002DEB44 = M2C_FIELD(arg1, s32 *, 0x18);
        return;
    }
    D_002DEB44 = -1;
}
