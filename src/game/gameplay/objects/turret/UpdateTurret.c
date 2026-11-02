#include "m2c_prelude.h"
M2C_UNK UpdateSo();                            /* extern */
M2C_UNK UpdateTurretAim(s32);                         /* extern */
M2C_UNK func_00225E98(s32);                         /* extern */
M2C_UNK func_00226250(s32);                         /* extern */

/** @sly2-annot addr=0x225B60
 * Purpose: SO update then aim (0x225BA0), zoom (0x225E98), fire (0x226250)
 * Info: proposed name UpdateTurret (T1); class TURRET slot16; TU turret
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/minigames.md#4-mounted-turret-turret-0x45-turret-tu-0x2259d8-0x226550-conf-high-ccode,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void UpdateTurret(s32 arg0) {
    UpdateSo();
    UpdateTurretAim(arg0);
    func_00225E98(arg0);
    func_00226250(arg0);
}
