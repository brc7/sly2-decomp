#include "m2c_prelude.h"
/** @sly2-annot addr=0x1F8730
 * Purpose: cref(+0x10)++
 * Info: proposed name IncrefXfh (T2); class CID148 slot1; TU so
 * Notes: verify r4r6: HANDLE::Decref -> XFH::Decref
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8730(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + 1);
}
