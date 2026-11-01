#include "m2c_prelude.h"
void *func_001AE4F8(M2C_UNK);                       /* extern */
M2C_UNK func_00256360(void *);                      /* extern */
extern M2C_UNK D_002C91C8;

void *NewJeanbison(void) {
    void *temp_v0;

    temp_v0 = func_001AE4F8(0x1010);
    func_00256360(temp_v0);
    M2C_FIELD(temp_v0, M2C_UNK **, 0xC) = &D_002C91C8;
    return temp_v0;
}
