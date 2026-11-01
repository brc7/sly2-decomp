#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(void *, M2C_UNK);                 /* extern */
M2C_UNK func_001490A0(void *, M2C_UNK);             /* extern */
extern f32 D_002DDED0;

void func_00149010(void *arg0) {
    void *temp_a0;

    if (FIsBasicDerivedFrom__FP5BASIC3CID(M2C_FIELD(arg0, void **, 0x2F8), 0x34) != 0) {
        temp_a0 = M2C_FIELD(arg0, void **, 0x2F8);
        if (M2C_FIELD(temp_a0, s32 *, 0x2DC) != 0x11) {
            if (M2C_FIELD(temp_a0, s32 *, 0x2C4) == 3) {
                if ((D_002DDED0 - M2C_FIELD(temp_a0, f32 *, 0x2E0)) < 0.15f) {
                    goto block_4;
                }
                goto block_6;
            }
            goto block_5;
        }
block_4:
        M2C_FIELD(arg0, void **, 0x2F8) = NULL;
        return;
    }
block_5:
block_6:
    func_001490A0(arg0, 1);
}
