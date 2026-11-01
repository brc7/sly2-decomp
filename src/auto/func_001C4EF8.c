#include "m2c_prelude.h"
M2C_UNK BindLoOidTable(void *, M2C_UNK, M2C_UNK *);  /* extern */
M2C_UNK func_001F31E0();                            /* extern */
void *func_001F7B98(void *);                        /* extern */
M2C_UNK func_002303A0(void *, s32, M2C_UNK);        /* extern */
extern M2C_UNK D_002C0D40;

void func_001C4EF8(void *arg0) {
    func_001F31E0();
    M2C_FIELD(func_001F7B98(arg0), s32 *, 0xF0) = 1;
    func_002303A0(arg0, M2C_FIELD(arg0, s32 *, 0x58), 0x15);
    func_002303A0(arg0, M2C_FIELD(arg0, s32 *, 0x5C), 0x15);
    BindLoOidTable(arg0, 1, &D_002C0D40);
}
