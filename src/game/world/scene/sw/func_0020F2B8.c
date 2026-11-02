#include "m2c_prelude.h"
M2C_UNK UpdateSwIllum();                            /* extern */

/** @sly2-annot addr=0x20F2B8
 * Purpose: Store HSV qword (+0x3E10) and UpdateSwIllum
 * Info: proposed name SetSwIllumHsv (T2); TU sw
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F2B8(void *arg0, s128 arg1) {
    M2C_FIELD(arg0, s128 *, 0x3E10) = arg1;
    UpdateSwIllum();
}
