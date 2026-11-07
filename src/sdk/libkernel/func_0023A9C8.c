#include "m2c_prelude.h"
extern s32 D_002C5A58;

// Clears flag D_002C5A58; called by sceSifSyncIop before resetting SIF regs. (libkernel iopnotify.o?) Probably
// _sceSifClearRebootFlag.
void func_0023A9C8(void) {
    D_002C5A58 = 0;
}
