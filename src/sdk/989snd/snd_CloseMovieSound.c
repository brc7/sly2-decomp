#include "m2c_prelude.h"
M2C_UNK snd_SendIOPCommandAndWait__FiiPc(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */

void snd_CloseMovieSound(void) {
    snd_SendIOPCommandAndWait__FiiPc(0x3C, 0, 0);
}
