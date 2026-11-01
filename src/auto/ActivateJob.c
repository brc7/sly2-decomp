#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(s32, M2C_UNK);                    /* extern */
M2C_UNK func_00143380(s32);                         /* extern */
M2C_UNK BroadcastGameEvent(M2C_UNK *, M2C_UNK, s32);     /* extern */
M2C_UNK SetCurrentJob(s32);                         /* extern */
s32 FIsCurrentJob();                                /* extern */
s32 PpoHero(M2C_UNK *);                       /* extern */
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003DD490;

void ActivateJob(s32 arg0) {
    s32 temp_v0;

    if (FIsCurrentJob() == 0) {
        BroadcastGameEvent(&D_002DEB30, 0x1C, arg0);
        temp_v0 = PpoHero(&D_003DD490);
        if ((temp_v0 != 0) && (FIsBasicDerivedFrom__FP5BASIC3CID(temp_v0, 0x34) != 0)) {
            func_00143380(temp_v0);
        }
        SetCurrentJob(arg0);
    }
}
