#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003E1414;

void func_00183FB8(s32 arg0) {
    OnLoAdd();
    AppendDlEntry__FP2DLPv(&D_003E1414, arg0);
}
