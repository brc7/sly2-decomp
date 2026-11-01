#include "m2c_prelude.h"
s32 IcgvFromPcgv__FP2CGP3CGV(void *arg0, s32 arg1) {
    return (s32) (arg1 - M2C_FIELD(arg0, s32 *, 4)) >> 5;
}
