#include "m2c_prelude.h"
M2C_UNK snd_SendIOPCommandAndWait__FiiPc(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */

void snd_ResetMovieSound(void) {
    snd_SendIOPCommandAndWait__FiiPc(0x3D, 0, 0);
}
