#include "m2c_prelude.h"
M2C_UNK func_001DE5D8(s32);                         /* extern */
M2C_UNK func_001DEDB0(s32);                         /* extern */
M2C_UNK func_001DF260(s32, M2C_UNK);                /* extern */
M2C_UNK PostSoLoad();                            /* extern */

void func_001DE568(s32 arg0) {
    PostSoLoad();
    func_001DE5D8(arg0);
    func_001DEDB0(arg0);
    func_001DF260(arg0, 0);
}
