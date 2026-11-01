#include "m2c_prelude.h"
M2C_UNK func_001291A8();                            /* extern */

/** @sly2-annot addr=0x129DA8
 * Purpose: Returns F32 copy (int promoted); used by most numeric builtins
 * Info: proposed name CRef_RefCoerceF32 (T1); class CRef; TU ref
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
void *func_00129DA8(void *arg0, void *arg1) {
    if (M2C_FIELD(arg1, s32 *, 0) == 0x16) {
        func_001291A8();
    } else {
        M2C_FIELD(arg0, f32 *, 4) = (f32) M2C_FIELD(arg1, s32 *, 4);
        M2C_FIELD(arg0, s32 *, 0) = 0x16;
    }
    return arg0;
}
