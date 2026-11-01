#include "m2c_prelude.h"
M2C_UNK EndTask(s32);                         /* extern */
s32 FIsCurrentTask();                                /* extern */

void DeactivateTask(s32 arg0) {
    if (FIsCurrentTask() != 0) {
        EndTask(arg0);
    }
}
