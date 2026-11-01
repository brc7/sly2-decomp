#include "m2c_prelude.h"
M2C_UNK func_0014FCA8();                            /* extern */
extern M2C_UNK D_002CD528;

/** @sly2-annot addr=0x14FD68
 * Purpose: Constructor of the default policy at CM+0x3A0 (vtable D_002CD528)
 * Info: proposed name CMPART1_ctor (T1); class CMPART1; TU cplcy
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void *func_0014FD68(void *arg0) {
    func_0014FCA8();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CD528;
    return arg0;
}
