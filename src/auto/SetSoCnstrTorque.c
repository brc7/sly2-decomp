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

void SetSoCnstrTorque(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_s1;

    if (M2C_FIELD(arg0, void **, 0xE8) != NULL) {
        temp_s1 = M2C_FIELD(arg0, void **, 0x5C);
        if ((M2C_FIELD(temp_s1, s32 *, 0x90) == 0) && (M2C_FIELD(temp_s1, s16 *, 0x98) != 2)) {
            func_002303A0(arg0, (s32) temp_s1, 1);
        }
        func_001F5510(arg0, (s32) temp_s1, arg1, M2C_FIELD(arg0, void **, 0xE8) + 0x90);
        func_001F55C8(arg0);
        if (M2C_FIELD(M2C_FIELD(arg0, void **, 0xE8), s32 *, 0xA0) != 3) {
            func_001F8668(arg0, 0);
        }
    }
}
