#include "m2c_prelude.h"
M2C_UNK ReleaseAct(s32);                         /* extern */
M2C_UNK func_0014D8C0(M2C_UNK *);                   /* extern */
M2C_UNK func_0014FCE8(s32, s32, s32);               /* extern */
extern M2C_UNK D_002DDF40;

void func_001512E0(s32 arg0, s32 arg1, s32 arg2) {
    ReleaseAct(arg0 + 0x14);
    func_0014D8C0(&D_002DDF40);
    func_0014FCE8(arg0, arg1, arg2);
}
