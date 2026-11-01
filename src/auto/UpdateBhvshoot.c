#include "m2c_prelude.h"
s32 FBhvanimDone(s32);                             /* extern */
M2C_UNK SetBhvshootState(void *, M2C_UNK);             /* extern */
M2C_UNK UpdateBhv();                            /* extern */
s32 FBhvBusyAbove(void *);                          /* extern */

void UpdateBhvshoot(void *arg0) {
    s32 temp_s1;

    UpdateBhv();
loop_1:
    if (FBhvBusyAbove(arg0) == 0) {
        temp_s1 = M2C_FIELD(arg0, s32 *, 0x30);
        if ((temp_s1 == 0) && (FBhvanimDone(M2C_FIELD(arg0, s32 *, 0x18)) != 0)) {
            SetBhvshootState(arg0, -1);
        }
        if (temp_s1 != M2C_FIELD(arg0, s32 *, 0x30)) {
            goto loop_1;
        }
    }
}
