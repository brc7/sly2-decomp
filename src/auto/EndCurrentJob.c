#include "m2c_prelude.h"
M2C_UNK EndJob(s32);                         /* extern */
s32 FIsCurrentJob();                                /* extern */

void EndCurrentJob(s32 arg0) {
    if (FIsCurrentJob() != 0) {
        EndJob(arg0);
    }
}
