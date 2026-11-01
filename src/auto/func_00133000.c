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

void func_00133000(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct {
        M2C_UNK m0;
    } fr;
    RefEval__FP5CPairP6CFrame((s32) (void *)&fr, arg1, arg2);
    if (arg3 != 0) {
        __as__4CRefRC4CRef(arg3, (s32) (void *)&fr);
    }
    func_00129EC8((void *)&fr);
}
