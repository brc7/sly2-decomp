#include "m2c_prelude.h"
M2C_UNK RenderStepplayerSelf(void *);                      /* extern */
M2C_UNK func_00190D10();                            /* extern */

void RenderJtSelf(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x18C8) == 3) {
        func_00190D10();
    }
    RenderStepplayerSelf(arg0);
}
