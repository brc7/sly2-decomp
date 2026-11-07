#include "m2c_prelude.h"
// System SIF cmd 0x80000000 handler: store IOP cmd-buffer address. (libkernel sifcmd.o) SDK name _sceSifCmdChangeAddr.
void func_0023AAB8(void *arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 8) = (s32) M2C_FIELD(arg0, s32 *, 0x10);
}
