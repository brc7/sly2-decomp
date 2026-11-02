#include "m2c_prelude.h"
M2C_UNK func_0014D8C0(M2C_UNK *);                   /* extern */
M2C_UNK func_0014FCE8(void *, s32, void *);         /* extern */
extern M2C_UNK D_002DDF40;

/** @sly2-annot addr=0x12E7F0
 * Purpose: CutCm (0x14D8C0) then CPLCY::Deactivate
 * Info: proposed name DeactivateCpcamera (T2); class CPCAMERA; TU tu_12E600
 * Notes: verify r1r2: CplcyCid44 is Lilly CPCAMERA (CM+0x4A8)
 * Confidence: low (draft) [T2 0.40] - evidence: Sly 1 / Lilly reference; sources: r1_116918
 * See: docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48
 * @end-sly2-annot */
void func_0012E7F0(void *arg0, s32 arg1, void *arg2) {
    if (M2C_FIELD(arg0, s32 *, 0x10) == arg2) {
        if (M2C_FIELD(arg2, s32 *, 0x120) != 0) {
            func_0014D8C0(&D_002DDF40);
        }
        M2C_FIELD(arg0, s32 *, 0x10) = 0;
    }
    func_0014FCE8(arg0, arg1, arg2);
}
