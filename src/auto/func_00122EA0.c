#include "m2c_prelude.h"
void func_001291A8(s32, M2C_UNK *);              /* extern */
void func_00129EC8(M2C_UNK *);                   /* extern */
void func_0012A0B0(M2C_UNK *, M2C_UNK);          /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F9C;
extern s32 D_003E0FA0;
extern s32 D_003E0FA4;
extern M2C_UNK D_004520A8;

s32 func_00122EA0(s32 arg0, s32 arg1, s32 arg2) {
    struct {
        M2C_UNK m0;
        M2C_UNK m4;
    } fr;
    func_0012A0B0((void *)&fr, 0x1B);
    CalculateDmat4(M2C_FIELD(arg2, s32 *, 4), M2C_FIELD(arg2, s32 *, 0xC), fr.m4);
    func_001291A8(arg0, (void *)&fr);
    func_00129EC8((void *)&fr);
    return arg0;
}
