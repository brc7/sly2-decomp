#include "m2c_prelude.h"
M2C_UNK sceIpuRestartDMA(s32);                         /* extern */

// Default restart-DMA callback: sceIpuRestartDMA. (libmpeg defhandler.o) SDK name _defRestartDMA.
s32 _defRestartDMA(void *arg0) {
    sceIpuRestartDMA(M2C_FIELD(arg0, s32 *, 0x40) + 0x68);
    return 1;
}
