#include "m2c_prelude.h"
M2C_UNK UpdateGrfjoytFromGrfusr__Fv();                            /* extern */
extern s32 D_002DFBC4;

void AddGrfusr__Fi(s32 arg0) {
    D_002DFBC4 |= arg0;
    UpdateGrfjoytFromGrfusr__Fv();
}
