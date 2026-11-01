#include "m2c_prelude.h"
extern M2C_UNK D_002C44F0;

/** @sly2-annot addr=0x231550
 * Purpose: copies local matrix out; returns parent XF or default
 * Info: proposed name XFM_PxfLocal (T2); class XFM slot32; TU xform
 * Notes: names_v2 C proposed SetAloThrobInColor__FP3ALOP6VECTOR (0.253) | agent qualifier: / GetXf
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
M2C_UNK *func_00231550(void *arg0, void *arg1) {
    M2C_UNK *var_v0;

    if (arg1 != NULL) {
        M2C_FIELD(arg1, s128 *, 0) = (s128) M2C_FIELD(arg0, s128 *, 0x60);
        M2C_FIELD(arg1, s128 *, 0x10) = (s128) M2C_FIELD(arg0, s128 *, 0x70);
        M2C_FIELD(arg1, s128 *, 0x20) = (s128) M2C_FIELD(arg0, s128 *, 0x80);
        M2C_FIELD(arg1, s128 *, 0x30) = (s128) M2C_FIELD(arg0, s128 *, 0x90);
    }
    var_v0 = M2C_FIELD(arg0, M2C_UNK **, 0x50);
    if (var_v0 == NULL) {
        var_v0 = &D_002C44F0;
    }
    return var_v0;
}
