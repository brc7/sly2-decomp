#include "m2c_prelude.h"
M2C_UNK SetMurrayState(void *, M2C_UNK);             /* extern */
M2C_UNK func_001B8D30(void *, M2C_UNK);             /* extern */
M2C_UNK StartBhv();                            /* extern */

/** @sly2-annot addr=0x146E98
 * Purpose: BHV198 slot 8: MURRAY flag +0x1AC0=1, Murray state 0xC (0x1B6078), 0x1B8D30(0)
 * Info: proposed name StartBhv198 (T2); class BHV198 slot8; TU tu_144B28
 * Notes: superseded: OnBhv198Resume (r2b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv198Resume
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00146E98(void *arg0) {
    StartBhv();
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x1AC0) = 1;
    SetMurrayState(M2C_FIELD(arg0, void **, 0x10), 0xC);
    func_001B8D30(M2C_FIELD(arg0, void **, 0x10), 0);
}
