#include "m2c_prelude.h"
M2C_UNK AppendDlEntry__FP2DLPv(M2C_UNK *, void *);           /* extern */
M2C_UNK RemoveDlEntry__FP2DLPv(s32, void *);                 /* extern */
extern M2C_UNK D_002DEB00;

void func_00139098(void *arg0) {
    RemoveDlEntry__FP2DLPv(M2C_FIELD(arg0, s32 *, 0x108C) + 0x448, arg0);
    AppendDlEntry__FP2DLPv(&D_002DEB00, arg0);
}
