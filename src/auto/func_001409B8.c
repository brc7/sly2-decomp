#include "m2c_prelude.h"
s32 FSwHandsOff(M2C_UNK *);                       /* extern */
extern M2C_UNK D_003DD490;

s32 func_001409B8(void) {
    return (FSwHandsOff(&D_003DD490) != 0) ? 0 : -1;
}
