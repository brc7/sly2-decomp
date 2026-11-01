#include "m2c_prelude.h"
M2C_UNK InitSave__FP4SAVEii(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK InitGame(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002E65E0;
extern s32 D_004425D8;

void func_001AA978(void) {
    InitGame(&D_002DEB30);
    D_004425D8 = -1;
    InitSave__FP4SAVEii(&D_002E65E0, 0, 0);
}
