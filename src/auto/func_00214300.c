#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, void *);           /* extern */
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, void *);           /* extern */
s32 FFindDlEntry__FP2DLPv(M2C_UNK *, void *);               /* extern */
extern M2C_UNK D_003E1060;

void func_00214300(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0xA8);
    if (temp_v0 != arg1) {
        if ((temp_v0 == 0) && (FFindDlEntry__FP2DLPv(&D_003E1060, arg0) != 0)) {
            RemoveDlEntry__FP2DLPv(&D_003E1060, arg0);
        }
        M2C_FIELD(arg0, s32 *, 0xA8) = arg1;
        if ((arg1 == 0) && (FFindDlEntry__FP2DLPv(&D_003E1060, arg0) == 0)) {
            AppendDlEntry__FP2DLPv(&D_003E1060, arg0);
        }
    }
}
