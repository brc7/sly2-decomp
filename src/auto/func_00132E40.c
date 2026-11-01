#include "m2c_prelude.h"
void func_00129EC8(M2C_UNK *);                   /* extern */
extern M2C_UNK D_0027A10C;
extern M2C_UNK D_0027A1D8;
extern M2C_UNK D_002BAAE8;
extern f32 D_002DDED0;
extern M2C_UNK D_003DD490;
extern f32 *D_003E0E80;
extern M2C_UNK D_003E1144;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x132E40
 * Purpose: RefEnsureOption with a null BASIC CRef (default object option)
 * Info: proposed name EnsureLoSidebagBasic (T2); TU basic
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void func_00132E40(s32 arg0, s32 arg1) {
    struct {
        M2C_UNK m0;
        char pad0[0xC];
        M2C_UNK m10;
    } fr;
    func_001294C0((s32) (void *)&fr, 0);
    RefEnsureOption__FP5BASICiP4CRef((s32) &fr.m10, arg0, arg1, (s32) (void *)&fr);
    func_00129EC8(&fr.m10);
    func_00129EC8((void *)&fr);
}
