#include "m2c_prelude.h"
M2C_UNK func_0014AE38(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK func_001FEEA0(M2C_UNK *);                   /* extern */
M2C_UNK RemoveStepguardSgf(void *, M2C_UNK);             /* extern */
extern M2C_UNK D_003DD040;

/** @sly2-annot addr=0x14A248
 * Purpose: BHV182 slot 9: state -1, clear flag 0x24, stop cue, guard +0x4A4 = 1
 * Info: proposed name CloseBhv182 (T2); class BHV182 slot9; TU tu_14A008
 * Notes: superseded: OnBhv182Pop (r2b_rest) | BHV vtable slot 9 role per r9 (CONFLICTS #15); was OnBhv182Pop
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014A248(void *arg0) {
    CloseBhv();
    func_0014AE38(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, void **, 0x10), 0x24);
    func_001FEEA0(&D_003DD040);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x4A4) = 1;
}
