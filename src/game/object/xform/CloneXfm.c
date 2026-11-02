#include "m2c_prelude.h"
M2C_UNK CloneLo();                            /* extern */
M2C_UNK func_002313F0(void *);                      /* extern */

/** @sly2-annot addr=0x2311B0
 * Purpose: CloneLo 0x1A6730 then invalidate world
 * Info: proposed name CloneXfm (T1); class XFM slot41; TU xform
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions, docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void CloneXfm(void *arg0, void *arg1) {
    void *temp_v1;
    void *temp_v1_2;

    CloneLo();
    temp_v1 = M2C_FIELD(arg1, void **, 0xC);
    M2C_FIELD(temp_v1, M2C_UNK (**)(void *, void *, void *), 0x194)(arg1 + M2C_FIELD(temp_v1, s16 *, 0x190), arg1, arg1 + 0x50);
    temp_v1_2 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v1_2, M2C_UNK (**)(void *, void *, void *), 0x194)(arg0 + M2C_FIELD(temp_v1_2, s16 *, 0x190), arg1, arg0 + 0x50);
    func_002313F0(arg0);
}
