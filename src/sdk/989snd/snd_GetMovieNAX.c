#include "m2c_prelude.h"
M2C_UNK snd_SendIOPCommandAndWait__FiiPc(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */

// 989snd: AndWait IOP command 0x5B with no data; returns the IOP movie-audio play position (CQueueOutputIop_Update
// multiplies it by the channel count). Name inferred (989snd style, next to snd_UpdateMovieADPCM's 0x5A).
void snd_GetMovieNAX(void) {
    snd_SendIOPCommandAndWait__FiiPc(0x5B, 0, 0);
}
