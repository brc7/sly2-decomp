#include "m2c_prelude.h"
s32 FFindDlEntry__FP2DLPv(s32, void *);                     /* extern */

s32 func_001E5778(void *arg0) {
    return FFindDlEntry__FP2DLPv(M2C_FIELD(arg0, s32 *, 0x14) + 0x5C, arg0) == 0;
}
