#include "m2c_prelude.h"
void *func_0012E238(s32);                           /* extern */

/** @sly2-annot addr=0x12E3F0
 * Purpose: Wrapper of InitActKind (no callers: callback)
 * Info: proposed name ActUnk_12E3F0 (T2); TU act
 * Notes: verifier: purpose partly wrong: Placeholder. Body = kind factory 0x12E238(*desc) (new one of 10
 *   classes by kind) then that object's vt slot 6 (vptr at +0x18) with (desc,a1,a2); returns the obj
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void *func_0012E3F0(s32 *arg0, s32 arg1, s32 arg2) {
    void *temp_v0;
    void *temp_v1;

    temp_v0 = func_0012E238(*arg0);
    temp_v1 = M2C_FIELD(temp_v0, void **, 0x18);
    M2C_FIELD(temp_v1, M2C_UNK (**)(void *, s32 *, s32, s32), 0x34)(temp_v0 + M2C_FIELD(temp_v1, s16 *, 0x30), arg0, arg1, arg2);
    return temp_v0;
}
