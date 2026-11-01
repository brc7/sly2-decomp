#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, M2C_UNK *);              /* extern */
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
void func_0012A0B0(M2C_UNK *, M2C_UNK);          /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F9C;
extern s32 D_003E0FA0;
extern s32 D_003E0FA4;
extern M2C_UNK D_004520A8;

s32 RefOpCancelCallback(s32 arg0, s32 arg1, s32 arg2) {
    CancelSwSpliceCallback((s32) &D_003DD490, M2C_FIELD(arg2, s32 *, 4));
    func_0012A0B0((M2C_UNK *) arg0, 0x24);
    return arg0;
}
