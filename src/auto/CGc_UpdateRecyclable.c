#include "m2c_prelude.h"
extern s32 D_002DD280;
extern s32 D_002DD288;
extern s32 D_002DD290;
extern s32 D_002DD298;
extern s32 D_002DD2A0;
extern s32 D_002DD2A8;
extern s32 D_002DD2B0;
extern s32 D_002DD2B8;
extern s32 D_002DD2C0;
extern s32 D_002DD2C8;
extern s32 D_002DD2D0;
extern s32 D_002DD2D8;

void CGc_UpdateRecyclable(void) {
    D_002DD288 = D_002DD280;
    D_002DD2A8 = D_002DD2A0;
    D_002DD298 = D_002DD290;
    D_002DD2B8 = D_002DD2B0;
    D_002DD2C8 = D_002DD2C0;
    D_002DD2D8 = D_002DD2D0;
}
