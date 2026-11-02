#include "m2c_prelude.h"
s128 *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_00452188;

/** @sly2-annot addr=0x1291E0
 * Purpose: Constructor: tag 0x1A, allocates refcounted VECTOR box from pool D_00452188 (0x11CCC0) and copies
 *   *pv
 * Info: proposed name CRef_ctor_VECTOR (T1); class CRef; TU ref
 * Notes: C++ signature hint (const VECTOR *) | COLLISION: CRef_ctor already used (same name proposed for
 *   0x129248); renamed to CRef_ctor_VECTOR
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
void *CRef_ctor_VECTOR(void *arg0, s128 *arg1) {
    s128 *temp_v0;

    M2C_FIELD(arg0, s32 *, 0) = 0x1A;
    temp_v0 = PvAllocSlotheapClear(&D_00452188);
    M2C_FIELD(temp_v0, s32 *, 0x10) = 0;
    M2C_FIELD(arg0, s128 **, 4) = temp_v0;
    M2C_FIELD(temp_v0, s32 *, 0x10) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x10) + 1);
    *M2C_FIELD(arg0, s128 **, 4) = *arg1;
    return arg0;
}
