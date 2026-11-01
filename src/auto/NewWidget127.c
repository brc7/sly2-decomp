#include "m2c_prelude.h"
void *func_001AE4F8(M2C_UNK);                       /* extern */
M2C_UNK func_00212158(void *);                      /* extern */
extern M2C_UNK D_002DABF0;

void *NewWidget127(void) {
    void *temp_v0;

    temp_v0 = func_001AE4F8(0x120);
    func_00212158(temp_v0);
    M2C_FIELD(temp_v0, M2C_UNK **, 0xC) = &D_002DABF0;
    return temp_v0;
}
