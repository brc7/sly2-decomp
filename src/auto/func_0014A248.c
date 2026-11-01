#include "m2c_prelude.h"
M2C_UNK func_0014AE38(void *, M2C_UNK);             /* extern */
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK func_001FEEA0(M2C_UNK *);                   /* extern */
M2C_UNK RemoveStepguardSgf(void *, M2C_UNK);             /* extern */
extern M2C_UNK D_003DD040;

void func_0014A248(void *arg0) {
    CloseBhv();
    func_0014AE38(arg0, -1);
    RemoveStepguardSgf(M2C_FIELD(arg0, void **, 0x10), 0x24);
    func_001FEEA0(&D_003DD040);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x4A4) = 1;
}
