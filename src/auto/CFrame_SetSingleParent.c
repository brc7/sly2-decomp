#include "m2c_prelude.h"
M2C_UNK func_00256EE0(void *);                      /* extern */

void CFrame_SetSingleParent(void *arg0) {
    M2C_FIELD(arg0, s32 *, 8) = 1;
    func_00256EE0(arg0 + 0xC);
}
