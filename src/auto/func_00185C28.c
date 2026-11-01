#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK AddLoHierarchy();                            /* extern */
extern M2C_UNK D_003E142C;

void func_00185C28(s32 arg0) {
    AddLoHierarchy();
    AppendDlEntry__FP2DLPv(&D_003E142C, arg0);
}
