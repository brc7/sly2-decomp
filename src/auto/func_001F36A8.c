#include "m2c_prelude.h"
M2C_UNK func_00167B68(s32);                         /* extern */
M2C_UNK PostLoLoad();                            /* extern */
M2C_UNK func_0022FAA0(s32);                         /* extern */

void func_001F36A8(s32 arg0) {
    PostLoLoad();
    func_0022FAA0(arg0);
    func_00167B68(arg0 + 0x80);
}
