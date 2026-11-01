#include "m2c_prelude.h"
M2C_UNK BroadcastGameEvent(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_001A2C20(M2C_UNK);                     /* extern */
M2C_UNK func_001A2DA0(s32);                         /* extern */
M2C_UNK func_001A4E08(M2C_UNK *);                   /* extern */
s32 func_0024A860();                                /* extern */
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002E1E88;

void StartupGame(void) {
    func_001A2C20(0);
    func_001A2DA0(func_0024A860());
    BroadcastGameEvent(&D_002DEB30, 0, 0);
    func_001A4E08(&D_002E1E88);
}
