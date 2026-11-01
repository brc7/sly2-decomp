#include "m2c_prelude.h"
M2C_UNK BindLoOidTable(s32, M2C_UNK, M2C_UNK *);     /* extern */
M2C_UNK LoadStepFromBrx();                            /* extern */
extern M2C_UNK D_002BC390;

void LoadBentleyFromBrx(s32 arg0) {
    LoadStepFromBrx();
    BindLoOidTable(arg0, 0x76, &D_002BC390);
}
