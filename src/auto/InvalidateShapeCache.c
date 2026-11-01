#include "m2c_prelude.h"
void InvalidateShapeCache(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x178) = 0;
    M2C_FIELD(arg0, s32 *, 0x148) = -1;
}
