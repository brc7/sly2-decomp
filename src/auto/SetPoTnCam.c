#include "m2c_prelude.h"
M2C_UNK SetLoParent_1A58D0(void *, M2C_UNK, M2C_UNK);    /* extern */
s32 PpoCur(M2C_UNK *);                       /* extern */
M2C_UNK func_00221268(void *, M2C_UNK);             /* extern */
extern M2C_UNK D_003DD490;

void SetPoTnCam(void *arg0, void *arg1) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0x16C);
    if (arg1 != temp_a0) {
        if (temp_a0 != NULL) {
            func_00221268(temp_a0, 0);
        }
        M2C_FIELD(arg0, void **, 0x16C) = arg1;
        if (arg1 != NULL) {
            M2C_FIELD(arg1, s32 *, 0x128) = -1;
            SetLoParent_1A58D0(M2C_FIELD(arg0, void **, 0x16C), 0, 0);
            if (PpoCur(&D_003DD490) == arg0) {
                func_00221268(M2C_FIELD(arg0, void **, 0x16C), 1);
            }
        }
    }
}
