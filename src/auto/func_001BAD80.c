#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnSoRemove();                            /* extern */
extern M2C_UNK D_003E1450;

void func_001BAD80(s32 arg0) {
    OnSoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E1450, arg0);
}
