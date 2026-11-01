#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
void **func_001AE318(s32 *);                        /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002C3C20;
extern M2C_UNK D_002C44F0;
extern void *D_002DD2FC;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_003DD490;
extern void *D_003E1218;
extern void *D_003E126C;
extern M2C_UNK D_003E15A8;
extern s32 *D_003E15C0;

void func_001F5078(s32 arg0) {
    void *temp_v0;

    if (M2C_FIELD(arg0, s32 *, 0xEC) != 0) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0, void (**)(s32, M2C_UNK), 0x17C)(arg0 + M2C_FIELD(temp_v0, s16 *, 0x178), 4);
        if (M2C_FIELD(arg0, s32 *, 0xE8) != 0) {
            SlotUnsettle(arg0, M2C_FIELD(arg0, s32 *, 0x58));
            SlotUnsettle(arg0, M2C_FIELD(arg0, s32 *, 0x5C));
        }
    }
}
