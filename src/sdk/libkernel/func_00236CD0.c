#include "m2c_prelude.h"
// Frees an RPC packet (clear pid, rec_id&~1). (libkernel sifrpc.o) SDK name _sceRpcFreePacket.
void func_00236CD0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) & 0xFFFFFFFE);
}
