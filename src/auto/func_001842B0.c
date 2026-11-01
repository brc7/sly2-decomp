#include "m2c_prelude.h"
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, s32, void *); /* extern */
M2C_UNK func_00183F50();                            /* extern */
extern M2C_UNK D_003DD490;

void func_001842B0(void *arg0) {
    func_00183F50();
    M2C_FIELD(arg0, s32 *, 0x188) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, M2C_FIELD(arg0, s32 *, 0x184), arg0);
    M2C_FIELD(arg0, s32 *, 0x190) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, M2C_FIELD(arg0, s32 *, 0x18C), arg0);
    M2C_FIELD(arg0, s32 *, 0x198) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, M2C_FIELD(arg0, s32 *, 0x194), arg0);
}
