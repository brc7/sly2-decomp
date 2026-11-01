#include "m2c_prelude.h"
M2C_UNK GoToTask(s32);                         /* extern */
extern s32 D_003E0FA4;

void func_00162DF8(void *arg0) {
    if (D_003E0FA4 != 0) {
        GoToTask(D_003E0FA4);
        return;
    }
    GoToTask(M2C_FIELD(arg0, s32 *, 0x64));
}
