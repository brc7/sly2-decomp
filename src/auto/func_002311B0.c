#include "m2c_prelude.h"
M2C_UNK CloneLo();                            /* extern */
M2C_UNK func_002313F0(void *);                      /* extern */

void func_002311B0(void *arg0, void *arg1) {
    void *temp_v1;
    void *temp_v1_2;

    CloneLo();
    temp_v1 = M2C_FIELD(arg1, void **, 0xC);
    M2C_FIELD(temp_v1, M2C_UNK (**)(void *, void *, void *), 0x194)(arg1 + M2C_FIELD(temp_v1, s16 *, 0x190), arg1, arg1 + 0x50);
    temp_v1_2 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v1_2, M2C_UNK (**)(void *, void *, void *), 0x194)(arg0 + M2C_FIELD(temp_v1_2, s16 *, 0x190), arg1, arg0 + 0x50);
    func_002313F0(arg0);
}
