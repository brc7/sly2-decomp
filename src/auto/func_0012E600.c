#include "m2c_prelude.h"
M2C_UNK InitXfm();                            /* extern */
extern s128 D_002A2DA0;
extern s128 D_002A2DC0;

void func_0012E600(void *arg0) {
    InitXfm();
    M2C_FIELD(arg0, s128 *, 0x100) = (s128) D_002A2DA0;
    M2C_FIELD(arg0, s128 *, 0x110) = (s128) D_002A2DC0;
}
