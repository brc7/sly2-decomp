#include "m2c_prelude.h"
/** @sly2-annot addr=0x210978
 * Purpose: If n==1 store at SW+0x3EA8; return psw
 * Info: proposed name SetSwField3EA8 (T2); TU sw
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void *func_00210978(void *arg0, s32 arg1) {
    if (arg1 == 1) {
        M2C_FIELD(arg0, s32 *, 0x3EA8) = arg1;
    }
    return arg0;
}
