#include "m2c_prelude.h"
M2C_UNK snd_SendIOPCommandAndWait__FiiPc(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */

// 989snd: AndWait IOP command 0x5B with no data; returns IOP movie-audio position (used by CQueueOutputIop_Update as
// pos*blocksize). (989snd) Probably snd_Cmd5B. (A 989snd-style name would be snd_GetMovieNAX.)
void func_00233018(void) {
    snd_SendIOPCommandAndWait__FiiPc(0x5B, 0, 0);
}
