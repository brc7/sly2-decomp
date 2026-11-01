#include "m2c_prelude.h"
M2C_UNK func_0014C2D8(M2C_UNK *);                   /* extern */
M2C_UNK UpdateGrfjoytFromGrfusr__Fv();                            /* extern */
M2C_UNK InitJoy__FP3JOYii(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK scePadInit(M2C_UNK);                        /* extern */
extern M2C_UNK D_002DDEC0;
extern M2C_UNK D_002DFC00;

void StartupJoy__Fv(void) {
    UpdateGrfjoytFromGrfusr__Fv();
    scePadInit(0);
    InitJoy__FP3JOYii(&D_002DFC00, 0, 0);
    func_0014C2D8(&D_002DDEC0);
}
