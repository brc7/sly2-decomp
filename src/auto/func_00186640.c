#include "m2c_prelude.h"
M2C_UNK func_0012CE98();                            /* extern */

/** @sly2-annot addr=0x186640
 * Purpose: ACT::Retract then restore owner xA8 from x1C
 * Info: proposed name RetractActik (T2); class ACTIK; TU ik
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/animation.md#ik-0x186610-0x187030
 * @end-sly2-annot */
void func_00186640(void *arg0) {
    func_0012CE98();
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), s32 *, 0xA8) = (s32) M2C_FIELD(arg0, s32 *, 0x1C);
}
