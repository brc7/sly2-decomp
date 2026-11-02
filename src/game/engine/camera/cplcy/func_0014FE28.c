#include "m2c_prelude.h"
M2C_UNK SetCmFov__FP2CMf(M2C_UNK *, f32);              /* extern */
extern M2C_UNK D_002DDF40;

/** @sly2-annot addr=0x14FE28
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU cplcy
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * @end-sly2-annot */
void func_0014FE28(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x20) = 0;
    M2C_FIELD(arg0, s32 *, 0x24) = 0;
    SetCmFov__FP2CMf(&D_002DDF40, M2C_FIELD(arg0, f32 *, 0x2C));
}
