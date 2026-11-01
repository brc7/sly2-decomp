#include "m2c_prelude.h"
M2C_UNK func_0018D698(void *, void *, void *);      /* extern */
M2C_UNK UnlockProxyPlo(s32, void *);                 /* extern */
s32 func_00224610(void *);                          /* extern */

void func_00136298(void *arg0) {
    if (func_00224610(M2C_FIELD(arg0, void **, 0x186C)) != 0) {
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x186C), void **, 0x16C) = arg0;
        func_0018D698(arg0, M2C_FIELD(arg0, void **, 0x16B4), M2C_FIELD(arg0, void **, 0x186C));
    }
    UnlockProxyPlo(M2C_FIELD(M2C_FIELD(arg0, void **, 0x16B4), s32 *, 4), M2C_FIELD(arg0, void **, 0x186C));
    M2C_FIELD(arg0, void **, 0x186C) = NULL;
    M2C_FIELD(arg0, void **, 0x16B4) = NULL;
}
