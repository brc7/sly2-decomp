#include "m2c_prelude.h"
extern s32 D_002C5A58;

/** @sly2-annot addr=0x23A9C8
 * Purpose: clears flag D_002C5A58; called by sceSifSyncIop before resetting SIF regs [libkernel iopnotify.o?]
 * Info: proposed name _sceSifClearRebootFlag (T2); TU sdk_libs
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void func_0023A9C8(void) {
    D_002C5A58 = 0;
}
