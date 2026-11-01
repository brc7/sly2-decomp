#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, void *);           /* extern */
M2C_UNK func_001F4B58();                            /* extern */
extern M2C_UNK D_003E11C4;

void func_001F4BF8(void *arg0) {
    s64 temp_a0;
    void *temp_v1;

    func_001F4B58();
    temp_v1 = M2C_FIELD(arg0, void **, 0xEC);
    if (temp_v1 != NULL) {
        temp_a0 = M2C_FIELD(temp_v1, s64 *, 0x70);
        if (!(temp_a0 & 2)) {
            M2C_FIELD(temp_v1, s64 *, 0x70) = (s64) (temp_a0 | 2);
            AppendDlEntry__FP2DLPv(&D_003E11C4, M2C_FIELD(arg0, void **, 0xEC));
        }
    }
}
