#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(s32, M2C_UNK);                    /* extern */
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, s32, s32);    /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x17F408
 * Purpose: Resolve entry oids with PloFindSwObject (kind 4) and test class 0x6D
 * Info: proposed name BindCid47Entries (T2); class CID47; TU tu_17F090
 * Notes: names_v2 v1 proposed FTakeStepguardDamage__FP9STEPGUARDP3ZPR (0.019)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F408(s32 arg0, s32 arg1, void *arg2) {
    s32 temp_a2;
    s32 temp_v0;
    s32 var_s2;
    void *var_s1;

    if (arg1 > 0) {
        var_s1 = arg2;
        var_s2 = arg1;
        do {
            temp_a2 = M2C_FIELD(var_s1, s32 *, 0);
            if (temp_a2 != -1) {
                temp_v0 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, temp_a2, arg0);
                if (temp_v0 != 0) {
                    if (FIsBasicDerivedFrom__FP5BASIC3CID(temp_v0, 0x6D) != 0) {
                        M2C_FIELD(var_s1, s32 *, 4) = temp_v0;
                    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(temp_v0, 0x6F) != 0) {
                        M2C_FIELD(var_s1, s32 *, 8) = temp_v0;
                    }
                }
            }
            var_s2 -= 1;
            var_s1 += 0x10;
        } while (var_s2 != 0);
    }
}
