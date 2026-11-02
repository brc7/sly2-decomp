#include "m2c_prelude.h"
/** @sly2-annot addr=0x23AAB8
 * Purpose: system SIF cmd 0x80000000 handler: store IOP cmd-buffer address [libkernel sifcmd.o]
 * Info: proposed name _sceSifCmdChangeAddr (T1); TU sdk_libs
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void func_0023AAB8(void *arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 8) = (s32) M2C_FIELD(arg0, s32 *, 0x10);
}
