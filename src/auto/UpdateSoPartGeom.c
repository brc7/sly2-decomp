#include "m2c_prelude.h"
M2C_UNK func_001664A0(void *, void *, void *);      /* extern */
M2C_UNK func_001F4BF8(s32);                         /* extern */

void UpdateSoPartGeom(void *arg0) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0);
    if (M2C_FIELD(arg0, s32 *, 0xA8) != M2C_FIELD(temp_a0, s32 *, 0xA4)) {
        func_001664A0(temp_a0 + 0x40, arg0 + 0x78, arg0 + 0x90);
        M2C_FIELD(arg0, s32 *, 0xA8) = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0), s32 *, 0xA4);
        func_001F4BF8(M2C_FIELD(arg0, s32 *, 4));
    }
}
