#include "m2c_prelude.h"
M2C_UNK func_0014FCA8();                            /* extern */
extern M2C_UNK D_002CD450;

/** @sly2-annot addr=0x151208
 * Purpose: CPHANG ctor (vtable D_002CD450)
 * Info: proposed name CPHANG_ctor (T1); class CPHANG; TU cplcy
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void *func_00151208(void *arg0) {
    func_0014FCA8();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CD450;
    return arg0;
}
