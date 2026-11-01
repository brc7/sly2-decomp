#include "m2c_prelude.h"
M2C_UNK func_00167B68(s32);                         /* extern */
M2C_UNK PostLoLoad();                            /* extern */
M2C_UNK UpdateSoXfWorldMatrices(s32);                         /* extern */

void PostSoLoad(s32 arg0) {
    PostLoLoad();
    UpdateSoXfWorldMatrices(arg0);
    func_00167B68(arg0 + 0x80);
}
