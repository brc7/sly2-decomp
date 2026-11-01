#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_001A50E8 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
extern f32 D_002A6A40;
extern M2C_UNK D_002C44F0;
extern f32 D_002DDED0;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003DD4A0;
extern M2C_UNK D_003DD4BC;
extern s32 D_003E139C;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_00452148;

void SetLoXfFromMat(s32 arg0, s32 arg1) {
    void *temp_v0;
    void *temp_v0_2;

    temp_v0 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v0, void (**)(s32, s128), 0xEC)(arg0 + M2C_FIELD(temp_v0, s16 *, 0xE8), M2C_FIELD(arg1, s128 *, 0x30));
    temp_v0_2 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v0_2, void (**)(s32, s32), 0xF4)(arg0 + M2C_FIELD(temp_v0_2, s16 *, 0xF0), arg1);
}
