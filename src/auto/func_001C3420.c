#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003E13C0;

void func_001C3420(s32 arg0) {
    OnLoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E13C0, arg0);
}
