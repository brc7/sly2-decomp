#include "m2c_prelude.h"
M2C_UNK UpdateSo();                            /* extern */
M2C_UNK UpdateTurretAim(s32);                         /* extern */
M2C_UNK func_00225E98(s32);                         /* extern */
M2C_UNK func_00226250(s32);                         /* extern */

void UpdateTurret(s32 arg0) {
    UpdateSo();
    UpdateTurretAim(arg0);
    func_00225E98(arg0);
    func_00226250(arg0);
}
