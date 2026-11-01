#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of junk_0x0011d5d8 (initial). */

/* Warning: missing "jr $ra" in last block of func_00117640 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_001179F0();                            /* extern */
extern M2C_UNK D_0027A778;
extern M2C_UNK D_0029A790;
extern M2C_UNK D_0029A7C0;
extern M2C_UNK D_0029A8C0;
extern M2C_UNK D_002A28D0;
extern M2C_UNK D_002A2900;
extern M2C_UNK D_002CA550;
extern M2C_UNK D_002CA5C0;
extern M2C_UNK D_002CC1C0;
extern M2C_UNK D_002CC240;
extern M2C_UNK D_002CC2C0;
extern M2C_UNK D_002CC340;
extern M2C_UNK D_002CC3C0;
extern M2C_UNK D_002CC440;
extern M2C_UNK D_002CCE90;
extern M2C_UNK D_002CDD40;
extern M2C_UNK D_002CDDC0;
extern M2C_UNK D_002CFA10;
extern M2C_UNK D_002CFC20;
extern M2C_UNK D_002CFCA0;
extern M2C_UNK D_002CFD20;
extern M2C_UNK D_002CFDA0;
extern M2C_UNK D_002CFE20;
extern M2C_UNK D_002CFEA0;
extern M2C_UNK D_002CFF20;
extern M2C_UNK D_002CFFA0;
extern M2C_UNK D_002D0020;
extern M2C_UNK D_002D00A0;
extern M2C_UNK D_002D0120;
extern M2C_UNK D_002D01A0;
extern M2C_UNK D_002D0220;
extern M2C_UNK D_002D02A0;
extern M2C_UNK D_002D0320;
extern M2C_UNK D_002D03A0;
extern M2C_UNK D_002D0420;
extern M2C_UNK D_002D4AF8;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern M2C_UNK D_002DD500;
extern M2C_UNK D_002DD520;
extern M2C_UNK D_002DD548;
extern M2C_UNK D_002DD568;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

static inline void *m2c_ctor_0(void *p)
{
    M2C_FIELD(p, M2C_UNK **, 0xC) = &D_002CFCA0;
    M2C_FIELD(p, s32 *, 0) = 0;
    M2C_FIELD(p, s32 *, 4) = 0;
    return p;
}

void *NewBhvpathlink(s32 arg0) {
    void *temp_v0;

    return m2c_ctor_0(func_001AE4F8(0x50));
}
