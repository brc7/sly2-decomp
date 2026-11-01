#include "m2c_prelude.h"
M2C_UNK BindLoOidTable(s32, M2C_UNK, M2C_UNK *);     /* extern */
M2C_UNK LoadSoFromBrx();                            /* extern */
extern M2C_UNK D_002C0258;

void func_001B1E00(s32 arg0) {
    LoadSoFromBrx();
    BindLoOidTable(arg0, 1, &D_002C0258);
}
