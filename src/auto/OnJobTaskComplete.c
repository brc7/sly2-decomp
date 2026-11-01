#include "m2c_prelude.h"
s32 FJobLive();                                /* extern */
M2C_UNK ActivateJob(void *);                      /* extern */
M2C_UNK CompleteJob(void *);                      /* extern */
M2C_UNK SetSwTaskPending(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_003DD490;

void OnJobTaskComplete(void *arg0, void *arg1) {
    if (FJobLive() != 0) {
        if (arg1 == M2C_FIELD(arg0, s32 *, 0x64)) {
            ActivateJob(arg0);
        }
        if (arg1 == M2C_FIELD(arg0, s32 *, 0x68)) {
            CompleteJob(arg0);
        }
        if (M2C_FIELD(arg1, s32 *, 0xB8) != -1) {
            SetSwTaskPending(&D_003DD490, arg1);
        }
    }
}
