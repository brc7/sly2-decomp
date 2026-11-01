#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnSoRemove();                            /* extern */
extern M2C_UNK D_003E13E4;

void func_001839D0(s32 arg0) {
    OnSoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E13E4, arg0);
}
