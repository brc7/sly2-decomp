#include "m2c_prelude.h"
M2C_UNK func_0022C840(s32, s32);                    /* extern */
M2C_UNK func_002303A0(s32, s32, s32);               /* extern */

/** @sly2-annot addr=0x12CE98
 * Purpose: Common slot 1 of the ACT vtables: detaches from the ALO (0x2303A0, 0x22C840)
 * Info: proposed name RetractAct (T2); class ACT; TU act
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/animation.md#tail-0x21c820-0x21d278
 * @end-sly2-annot */
void func_0012CE98(void *arg0) {
    func_002303A0(M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0x14));
    func_0022C840(M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 8));
}
