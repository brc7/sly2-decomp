#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, void *);           /* extern */
M2C_UNK func_001F4B58();                            /* extern */
extern M2C_UNK D_003E11C4;

/** @sly2-annot addr=0x1F4BF8
 * Purpose: After an SO moved/changed bounds: MarkSoTreeBoundsDirty, then flag its broadphase OXA (+0xEC, flag
 *   x70 bit1) dirty once and queue it on D_003E11C4 so UpdateSwAaox re-sorts its sweep endpoints
 * Info: proposed name InvalidateSoAaox (T2); TU so
 * Notes: v3: superseded MarkSoTreeDirty | top50_round (0.55): Body + callees AppendDlEntry. +0xEC is the OXA
 *   node (AddSwAaobrObject allocates it; RecalcSwOxfFilterForObject tests +0xEC). Per-object counterpart of
 *   Sly 1 bbmark InvalidateSwAaox (psw->fAaoxValid=0). Callers: SetSoVelo
 * Confidence: low (draft) [T2 0.55] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r6_1D5150+top50_round
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4BF8(void *arg0) {
    s64 temp_a0;
    void *temp_v1;

    func_001F4B58();
    temp_v1 = M2C_FIELD(arg0, void **, 0xEC);
    if (temp_v1 != NULL) {
        temp_a0 = M2C_FIELD(temp_v1, s64 *, 0x70);
        if (!(temp_a0 & 2)) {
            M2C_FIELD(temp_v1, s64 *, 0x70) = (s64) (temp_a0 | 2);
            AppendDlEntry__FP2DLPv(&D_003E11C4, M2C_FIELD(arg0, void **, 0xEC));
        }
    }
}
