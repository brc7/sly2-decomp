#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FF828
 * Purpose: SW+0x3A14 = 0
 * Info: proposed name ClearSwSpliceStub (T2); TU splicemap
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/splice-scripting.md#splicemap-0x1ff7e8-0x1ffcf8
 * @end-sly2-annot */
void func_001FF828(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x3A14) = 0;
}
