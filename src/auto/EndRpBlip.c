#include "m2c_prelude.h"
M2C_UNK AddDmaCnt__4DMAS();                         /* extern */
M2C_UNK AddVifFlusha__4VIFS(s32);                   /* extern */
M2C_UNK EndDmaCnt__4DMAS(s32);                      /* extern */

void EndRpBlip(s32 arg0) {
    AddDmaCnt__4DMAS();
    AddVifFlusha__4VIFS(arg0);
    EndDmaCnt__4DMAS(arg0);
}
