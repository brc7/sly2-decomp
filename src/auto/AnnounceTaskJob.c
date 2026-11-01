#include "m2c_prelude.h"
M2C_UNK BroadcastGameEvent(M2C_UNK *, M2C_UNK, s32);     /* extern */
extern M2C_UNK D_002DEB30;

void AnnounceTaskJob(void *arg0) {
    BroadcastGameEvent(&D_002DEB30, 0x1E, M2C_FIELD(arg0, s32 *, 0x7C));
}
