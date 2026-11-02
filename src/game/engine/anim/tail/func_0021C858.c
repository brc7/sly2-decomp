#include "m2c_prelude.h"
M2C_UNK func_0012CE98();                            /* extern */

/** @sly2-annot addr=0x21C858
 * Purpose: hook vt slot1: base 0x12CE98 (calls 0x2303A0 + 0x22C840 = XF act list update) then writes +0x1C
 *   back to object+0xA8
 * Info: proposed name RetractChainAct (T2); TU tail
 * Notes: agent qualifier: hook vt slot1
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#tail-0x21c820-0x21d278
 * @end-sly2-annot */
void func_0021C858(void *arg0) {
    func_0012CE98();
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), s32 *, 0xA8) = (s32) M2C_FIELD(arg0, s32 *, 0x1C);
}
