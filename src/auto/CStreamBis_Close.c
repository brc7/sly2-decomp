#include "m2c_prelude.h"
M2C_UNK InitGeom__FP4GEOM(void *);                  /* extern */
M2C_UNK sceCdBreak();                               /* extern */
M2C_UNK sceClose(s32);                              /* extern */

void CStreamBis_Close(void *arg0) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x3C);
    if (temp_a0 >= 0) {
        sceClose(temp_a0);
        M2C_FIELD(arg0, s32 *, 0x3C) = -1;
    }
    if (M2C_FIELD(arg0, s32 *, 0x50) >= 0) {
        if (M2C_FIELD(arg0, s32 *, 0x44) != 0) {
            sceCdBreak();
        }
        M2C_FIELD(arg0, s32 *, 0x50) = -1;
    }
    InitGeom__FP4GEOM(arg0);
}
