#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, void *, s32);          /* extern */
M2C_UNK ClearSle(s32);                         /* extern */
M2C_UNK ClearSl(s32);                         /* extern */
M2C_UNK func_0011A070(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003E159C;

s32 func_001ED850(void *arg0) {
    s32 temp_v0;
    void *temp_v0_2;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xC);
    temp_v0_2 = M2C_FIELD(temp_v1, void *(**)(void *), 0x34)(arg0 + M2C_FIELD(temp_v1, s16 *, 0x30));
    temp_v0 = M2C_FIELD(temp_v0_2, s32 (**)(), 0x10)();
    CopyAb__FPvT0Ui(temp_v0, arg0, M2C_FIELD(temp_v0_2, s32 *, 0xC));
    ClearSl(temp_v0 + 0x34);
    ClearSle(temp_v0 + 0x40);
    func_0011A070(&D_003E159C, temp_v0);
    return temp_v0;
}
