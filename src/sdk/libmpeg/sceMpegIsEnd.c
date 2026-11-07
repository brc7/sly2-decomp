#include "m2c_prelude.h"
// Returns mp->sys->isEnd. (libmpeg mpeg.o)
s32 sceMpegIsEnd(void *arg0) {
    return *M2C_FIELD(arg0, s32 **, 0x40);
}
