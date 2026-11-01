#include "m2c_prelude.h"
s32 FMurrayCanAttack(void *arg0) {
    if ((M2C_FIELD(arg0, s32 *, 0x190C) != 0) || (M2C_FIELD(arg0, s32 *, 0x1810) != -1)) {
        return M2C_FIELD(arg0, s32 *, 0x1820);
    }
    return 1;
}
