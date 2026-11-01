#include "m2c_prelude.h"
/** @sly2-annot addr=0x231220
 * Purpose: unknown
 * Info: class XFM slot50; TU xform
 * Notes: names_v2 C proposed SetAloRotationDamping__FP3ALOf (0.253) | descriptive only: other-class vt slot2
 *   (0x2DB84C) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.15] - evidence: static naming pass; sources: r8_21BB88
 * @end-sly2-annot */
void func_00231220(void *arg0, void *arg1, void **arg2) {
    s32 temp_a0;
    void *temp_a1;
    void *temp_t0;
    void *var_a3;

    temp_t0 = *arg2;
    if (temp_t0 != NULL) {
        temp_a1 = M2C_FIELD(arg1, void **, 0x10);
        temp_a0 = (s32) (temp_t0 - M2C_FIELD(temp_a1, s32 *, 0x54)) >> 8;
        var_a3 = NULL;
        if ((temp_a0 >= 0) && (temp_a0 < M2C_FIELD(temp_a1, s32 *, 0x50))) {
            var_a3 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x54) + (temp_a0 << 8);
        }
        if (M2C_FIELD(var_a3, void **, 0x88) == (temp_t0 + 0x80)) {
            M2C_FIELD(var_a3, void **, 0x88) = (void *) (var_a3 + 0x80);
        }
        if (M2C_FIELD(var_a3, s32 *, 0x8C) == (temp_t0 + 0x84)) {
            M2C_FIELD(var_a3, void **, 0x88) = (void *) (var_a3 + 0x84);
        }
        *arg2 = var_a3;
    }
}
