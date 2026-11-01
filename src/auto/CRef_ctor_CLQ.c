#include "m2c_prelude.h"
s128 *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_004521B8;

/** @sly2-annot addr=0x129330
 * Purpose: Constructor: tag 0x1C (CLQ, 16 B box, pool D_004521B8)
 * Info: proposed name CRef_ctor_CLQ (T1); class CRef; TU ref
 * Notes: C++ signature hint (const CLQ *) | COLLISION: CRef_ctor already used (same name proposed for
 *   0x129248); renamed to CRef_ctor_CLQ
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
void *CRef_ctor_CLQ(void *arg0, s128 *arg1) {
    s128 *temp_v0;

    M2C_FIELD(arg0, s32 *, 0) = 0x1C;
    temp_v0 = PvAllocSlotheapClear(&D_004521B8);
    M2C_FIELD(temp_v0, s32 *, 0x10) = 0;
    M2C_FIELD(arg0, s128 **, 4) = temp_v0;
    M2C_FIELD(temp_v0, s32 *, 0x10) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x10) + 1);
    *M2C_FIELD(arg0, s128 **, 4) = *arg1;
    return arg0;
}
