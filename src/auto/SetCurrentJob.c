#include "m2c_prelude.h"
M2C_UNK EndCurrentJob(s32);                         /* extern */
M2C_UNK BeginJob(s32);                         /* extern */
s32 FIsCurrentJob();                                /* extern */
extern s32 D_003E0F9C;

void SetCurrentJob(s32 arg0) {
    if (FIsCurrentJob() == 0) {
        if (D_003E0F9C != 0) {
            EndCurrentJob(D_003E0F9C);
        }
        BeginJob(arg0);
    }
}
