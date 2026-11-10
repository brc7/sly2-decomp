#include "m2c_prelude.h"
// Marks the current position as dynamic start. (libmpeg mpeg.o) SDK name _alalcSetDynamic.
void _alalcSetDynamic(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xC) = (s32) M2C_FIELD(arg0, s32 *, 8);
}
