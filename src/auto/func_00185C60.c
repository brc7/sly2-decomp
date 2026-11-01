#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK RemoveLoHierarchy();                            /* extern */
extern M2C_UNK D_003E142C;

void func_00185C60(s32 arg0) {
    RemoveLoHierarchy();
    RemoveDlEntry__FP2DLPv(&D_003E142C, arg0);
}
