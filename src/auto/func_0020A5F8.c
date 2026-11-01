#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_003E145C;

void *func_0020A5F8(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0xC) = arg1;
    M2C_FIELD(arg0, f32 *, 0x14) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x18) = 2.0f;
    M2C_FIELD(arg0, s32 *, 0x10) = -1;
    M2C_FIELD(arg0, s32 *, 8) = -1;
    AppendDlEntry__FP2DLPv(&D_003E145C, arg0);
    return arg0;
}
