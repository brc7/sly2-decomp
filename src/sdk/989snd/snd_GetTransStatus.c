#include "m2c_prelude.h"
M2C_UNK snd_SendIOPCommandAndWait__FiiPc(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */

void snd_GetTransStatus(void) {
    snd_SendIOPCommandAndWait__FiiPc(0x40, 0, 0);
}
