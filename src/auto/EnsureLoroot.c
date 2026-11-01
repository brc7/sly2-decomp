#include "m2c_prelude.h"
void **PvAllocSlotheapClear(M2C_UNK *);                    /* extern */
extern M2C_UNK D_003E0E1C;

void EnsureLoroot(void *arg0) {
    void **temp_v0;

    if (M2C_FIELD(arg0, void ***, 0x14) == NULL) {
        temp_v0 = PvAllocSlotheapClear(&D_003E0E1C);
        M2C_FIELD(arg0, void ***, 0x14) = temp_v0;
        *temp_v0 = arg0;
    }
}
