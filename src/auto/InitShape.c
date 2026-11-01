#include "m2c_prelude.h"
M2C_UNK InitXfm();                            /* extern */

void InitShape(void *arg0) {
    InitXfm();
    M2C_FIELD(arg0, s32 *, 0x148) = -1;
    M2C_FIELD(arg0, s32 *, 0x178) = 1;
}
