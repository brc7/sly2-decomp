#include "m2c_prelude.h"
M2C_UNK func_001D4940(void *, M2C_UNK);             /* extern */
M2C_UNK func_001D4C88();                            /* extern */
M2C_UNK SeekSma(void *, s32);                 /* extern */

void func_001D14C0(void *arg0) {
    void *temp_a0;

    M2C_FIELD(arg0, s32 *, 0x560) = (s32) M2C_FIELD(arg0, s32 *, 0x55C);
    func_001D4C88();
    temp_a0 = M2C_FIELD(arg0, void **, 0x570);
    if (temp_a0 != NULL) {
        SeekSma(temp_a0, *M2C_FIELD(M2C_FIELD(temp_a0, void **, 0x24), s32 **, 0x54));
    }
    func_001D4940(arg0, 0);
}
