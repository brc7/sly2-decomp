#include "m2c_prelude.h"
M2C_UNK func_0012E460(s32);                         /* extern */
M2C_UNK func_0014D8C0(M2C_UNK *);                   /* extern */
M2C_UNK func_0014FCE8(s32, s32, s32);               /* extern */
extern M2C_UNK D_002DDF40;

/** @sly2-annot addr=0x1512E0
 * Purpose: 0x12E460(+0x14), CutCm, CPLCY::Deactivate
 * Info: proposed name DeactivateCphang (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_001512E0(s32 arg0, s32 arg1, s32 arg2) {
    func_0012E460(arg0 + 0x14);
    func_0014D8C0(&D_002DDF40);
    func_0014FCE8(arg0, arg1, arg2);
}
