#include "m2c_prelude.h"
M2C_UNK AbortSmaTransition();                            /* extern */
M2C_UNK ChooseSmaTransition(void *, M2C_UNK);             /* extern */

void SetSmaMask(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x48) = arg1;
    if ((M2C_FIELD(arg0, s32 *, 0x30) != 0) && (M2C_FIELD(M2C_FIELD(arg0, void **, 0x40), s32 *, 0x10) & arg1)) {
        AbortSmaTransition();
        ChooseSmaTransition(arg0, 0);
    }
}
