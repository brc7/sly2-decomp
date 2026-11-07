#include "m2c_prelude.h"
// Resets the current position to dynamic start. (libmpeg mpeg.o)
void _alalcFree(void *arg0) {
    M2C_FIELD(arg0, s32 *, 8) = (s32) M2C_FIELD(arg0, s32 *, 0xC);
}
