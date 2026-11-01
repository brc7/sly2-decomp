#include "m2c_prelude.h"
M2C_UNK SetLoXfFromMat(s32, s32);                    /* extern */

/** @sly2-annot addr=0x1A6B88
 * Purpose: LO vt38(ploOther): SetLoXfFromMat(plo, ploOther->PmatWorld())
 * Info: proposed name MatchLoXf (T2); class LO slot38; TU lo
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6B88(s32 arg0, void *arg1) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg1, void **, 0xC);
    SetLoXfFromMat(arg0, M2C_FIELD(temp_v1, s32 (**)(void *), 0xFC)(arg1 + M2C_FIELD(temp_v1, s16 *, 0xF8)));
}
