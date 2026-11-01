#include "m2c_prelude.h"
M2C_UNK func_001B0D90(void *);                      /* extern */
M2C_UNK func_001B1A38(void *, s32, void *);         /* extern */
M2C_UNK func_001B1BF0(void *);                      /* extern */
M2C_UNK PostSoLoad();                            /* extern */

void func_001B0D08(void *arg0) {
    PostSoLoad();
    func_001B1BF0(arg0);
    func_001B1A38(arg0, M2C_FIELD(arg0, s32 *, 0x18C), arg0 + 0x1CC);
    func_001B1A38(arg0, M2C_FIELD(arg0, s32 *, 0x1B0), arg0 + 0x1D0);
    func_001B0D90(arg0);
}
