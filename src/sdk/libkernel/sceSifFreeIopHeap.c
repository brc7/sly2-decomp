#include "m2c_prelude.h"
M2C_UNK func_00238CC8();                            /* extern */

/** @sly2-annot addr=0x238D40
 * Purpose: wrapper -> sceSifFreeSysMemory [libkernel iopheap.o]
 * Info: proposed name sceSifFreeIopHeap (T1); TU sdk_libs
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void sceSifFreeIopHeap(void) {
    func_00238CC8();
}
