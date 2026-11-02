#include "m2c_prelude.h"
s32 sceRead(s32, s32, s32);                         /* extern */

/** @sly2-annot addr=0x184C60
 * Purpose: Refill from host file: sceRead(fd x3C, buf, cb)
 * Info: proposed name CStreamBis_RefillFile (T2); class CStreamBis; TU tu_1848A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/core-library.md#tu_1848a8-0x1848a8-0x184f50
 * @end-sly2-annot */
void func_00184C60(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x38);
    M2C_FIELD(arg0, s32 *, 0x10) = temp_v0;
    M2C_FIELD(arg0, s32 *, 0x14) = sceRead(M2C_FIELD(arg0, s32 *, 0x3C), temp_v0, M2C_FIELD(arg0, s32 *, 0x34));
}
