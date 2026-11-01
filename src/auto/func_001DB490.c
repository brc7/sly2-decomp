#include "m2c_prelude.h"
s32 PpoHero(M2C_UNK *);                       /* extern */
M2C_UNK func_001DB520(s32, M2C_UNK);                /* extern */
extern s32 D_002DD5BC;
extern s32 D_002E1E40;
extern s32 D_002F7900;
extern M2C_UNK D_003DD490;

void func_001DB490(s32 arg0) {
    s32 temp_v0;

    temp_v0 = PpoHero(&D_003DD490);
    if (temp_v0 != 0) {
        if (temp_v0 == D_002E1E40) {
            func_001DB520(arg0, 0);
            return;
        }
        if (temp_v0 == D_002DD5BC) {
            func_001DB520(arg0, 1);
            return;
        }
        if (temp_v0 == D_002F7900) {
            func_001DB520(arg0, 2);
        }
    }
}
