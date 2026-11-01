#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, void *, s32);          /* extern */
M2C_UNK func_0011A058(s32);                         /* extern */
M2C_UNK func_0011A060(s32);                         /* extern */
M2C_UNK func_0011A070(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003E159C;

/** @sly2-annot addr=0x1ED850
 * Purpose: Duplicate an SAA (size from its class descriptor via vt6), reset its lists and link it into the
 *   SAA list D_003E159C
 * Info: proposed name PsaaClone (T2); TU shdanim
 * Notes: names_v2 v1 proposed PvtsaaFromSaak__F4SAAK (0.012)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
s32 func_001ED850(void *arg0) {
    s32 temp_v0;
    void *temp_v0_2;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xC);
    temp_v0_2 = M2C_FIELD(temp_v1, void *(**)(void *), 0x34)(arg0 + M2C_FIELD(temp_v1, s16 *, 0x30));
    temp_v0 = M2C_FIELD(temp_v0_2, s32 (**)(), 0x10)();
    CopyAb__FPvT0Ui(temp_v0, arg0, M2C_FIELD(temp_v0_2, s32 *, 0xC));
    func_0011A060(temp_v0 + 0x34);
    func_0011A058(temp_v0 + 0x40);
    func_0011A070(&D_003E159C, temp_v0);
    return temp_v0;
}
