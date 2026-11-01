#include "m2c_prelude.h"
M2C_UNK __builtin_delete(void *);                   /* extern */
M2C_UNK CStreamBis_Close();                            /* extern */
extern M2C_UNK D_002D2918;

void func_00184950(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, M2C_UNK **, 0x30) = &D_002D2918;
    CStreamBis_Close();
    if (arg1 & 1) {
        __builtin_delete(arg0);
    }
}
