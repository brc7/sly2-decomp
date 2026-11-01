#include "m2c_prelude.h"
M2C_UNK func_00168638(void *, M2C_UNK, s32, f32, f32); /* extern */
M2C_UNK func_001F3C10(void *);                      /* extern */
extern s32 D_002DDF54;

void RenderSoSelf(void *arg0) {
    f32 temp_f12;

    if (M2C_FIELD(arg0, u8 *, 0x2D) & D_002DDF54) {
        func_001F3C10(arg0);
        temp_f12 = M2C_FIELD(arg0, f32 *, 0x12C);
        func_00168638(arg0 + 0x80, 0, M2C_FIELD(arg0, s32 *, 0x134), temp_f12, temp_f12);
    }
}
