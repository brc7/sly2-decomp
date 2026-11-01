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

void func_00132E40(s32 arg0, s32 arg1) {
    struct {
        M2C_UNK m0;
        char pad0[0xC];
        M2C_UNK m10;
    } fr;
    CRef_ctor_BASIC((s32) (void *)&fr, 0);
    RefEnsureOption__FP5BASICiP4CRef((s32) &fr.m10, arg0, arg1, (s32) (void *)&fr);
    func_00129EC8(&fr.m10);
    func_00129EC8((void *)&fr);
}
