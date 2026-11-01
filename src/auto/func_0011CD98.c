#include "m2c_prelude.h"
/** @sly2-annot addr=0x11CD98
 * Purpose: Pushes a slot back on the free list, count--
 * Info: proposed name FreeSlotheapPv (T1); TU slotheap
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#slotheap-0x11cca8-0x11cf60
 * @end-sly2-annot */
void func_0011CD98(void *arg0, M2C_UNK *arg1) {
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) - 1);
    *arg1 = M2C_FIELD(arg0, M2C_UNK **, 0xC);
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = arg1;
}
