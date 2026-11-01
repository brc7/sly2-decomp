#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003E1408;

void func_00183730(s32 arg0) {
    OnLoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E1408, arg0);
}
