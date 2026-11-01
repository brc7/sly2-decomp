#include "m2c_prelude.h"
/** @sly2-annot addr=0x236CD0
 * Purpose: free RPC packet (clear pid, rec_id&~1) [libkernel sifrpc.o]
 * Info: proposed name _sceRpcFreePacket (T1); TU sdk_libs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void func_00236CD0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) & 0xFFFFFFFE);
}
