#include "m2c_prelude.h"
void *func_0011CCC0(M2C_UNK *);                     /* extern */
extern M2C_UNK D_004521A0;

/** @sly2-annot addr=0x1292B0
 * Purpose: Constructor: tag 0x1B, MATRIX4 box (pool D_004521A0, cref at +0x40)
 * Info: proposed name CRef_ctor_MATRIX4 (T1); class CRef; TU ref
 * Notes: names_v2 v1 proposed Decref__4CRef (0.003) | C++ signature hint (const MATRIX4 *) | COLLISION:
 *   CRef_ctor already used (same name proposed for 0x129248); renamed to CRef_ctor_MATRIX4
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
void *func_001292B0(void *arg0, void *arg1) {
    void *temp_a1;
    void *temp_v0;

    M2C_FIELD(arg0, s32 *, 0) = 0x1B;
    temp_v0 = func_0011CCC0(&D_004521A0);
    M2C_FIELD(temp_v0, s32 *, 0x40) = 0;
    M2C_FIELD(arg0, void **, 4) = temp_v0;
    M2C_FIELD(temp_v0, s32 *, 0x40) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x40) + 1);
    temp_a1 = M2C_FIELD(arg0, void **, 4);
    M2C_FIELD(temp_a1, s128 *, 0) = (s128) M2C_FIELD(arg1, s128 *, 0);
    M2C_FIELD(temp_a1, s128 *, 0x10) = (s128) M2C_FIELD(arg1, s128 *, 0x10);
    M2C_FIELD(temp_a1, s128 *, 0x20) = (s128) M2C_FIELD(arg1, s128 *, 0x20);
    M2C_FIELD(temp_a1, s128 *, 0x30) = (s128) M2C_FIELD(arg1, s128 *, 0x30);
    return arg0;
}
