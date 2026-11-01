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

s32 PdcbAddLo(s32 arg0) {
    void *temp_v0;

    temp_v0 = func_00210100((s32) &D_003DD490);
    M2C_FIELD(temp_v0, void **, 8) = (void *) M2C_FIELD(arg0, void **, 0x34);
    M2C_FIELD(arg0, void **, 0x34) = temp_v0;
    return (s32) temp_v0;
}
