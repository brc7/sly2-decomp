#include "m2c_prelude.h"
M2C_UNK ChargeGadgetMp(void *, M2C_UNK);             /* extern */
M2C_UNK SetJtGrabState(void *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_00195A00(void *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */

void StartStepplayerGadget(void *arg0, s32 arg1) {
    switch (arg1) {                                 /* irregular */
    case 14:
    case 21:
    case 23:
        M2C_FIELD(arg0, s32 *, 0x9BC) = arg1;
        SetJtGrabState(arg0 + 0x4C0, 0xB, 0, 0);
        return;
    case 12:
        ChargeGadgetMp(arg0, 0xC);
        func_00195A00(arg0 + 0x2C0, 0x11, 0, 0);
        return;
    }
}
