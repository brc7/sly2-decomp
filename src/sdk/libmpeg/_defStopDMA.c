#include "m2c_prelude.h"
M2C_UNK sceIpuStopDMA(s32);                         /* extern */

// Default stop-DMA callback: sceIpuStopDMA(sys+0x68). (libmpeg defhandler.o) SDK name _defStopDMA.
s32 _defStopDMA(void *arg0) {
    sceIpuStopDMA(M2C_FIELD(arg0, s32 *, 0x40) + 0x68);
    return 1;
}
