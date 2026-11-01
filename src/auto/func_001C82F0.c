#include "m2c_prelude.h"
M2C_UNK BindLoOidTable(void *, M2C_UNK, M2C_UNK *);  /* extern */
s32 func_001C8110();                                /* extern */
M2C_UNK func_001C8730(void *, M2C_UNK);             /* extern */
M2C_UNK func_001F36A8();                            /* extern */
s32 func_001F4C58(void *, M2C_UNK, M2C_UNK);        /* extern */
extern M2C_UNK D_002C0EC8;

void func_001C82F0(void *arg0) {
    func_001F36A8();
    BindLoOidTable(arg0, 0x18, &D_002C0EC8);
    M2C_FIELD(arg0, s32 *, 0x204) = func_001F4C58(arg0, 0x645, 1);
    func_001C8730(arg0, (func_001C8110() == 0) ? 0 : 5);
}
