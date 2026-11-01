#include "m2c_prelude.h"
M2C_UNK PrependDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
s32 FFindDlEntry__FP2DLPv(M2C_UNK *, s32);                  /* extern */
M2C_UNK func_001D95B8(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003D9E70;

void func_001D98B0(s32 arg0) {
    if (FFindDlEntry__FP2DLPv(&D_003D9E70, arg0) != 0) {
        RemoveDlEntry__FP2DLPv(&D_003D9E70, arg0);
    }
    PrependDlEntry__FP2DLPv(&D_003D9E70, arg0);
    func_001D95B8(&D_003D9E70);
}
