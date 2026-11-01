#include "m2c_prelude.h"
M2C_UNK RecalcSwOxfFilterForObject(M2C_UNK *, s32);              /* extern */
M2C_UNK CloseBhvhit();                            /* extern */
extern M2C_UNK D_003DD490;

void CloseBhvko(void *arg0) {
    CloseBhvhit();
    RecalcSwOxfFilterForObject(&D_003DD490, M2C_FIELD(arg0, s32 *, 0x10));
}
