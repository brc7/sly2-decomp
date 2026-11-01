#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, M2C_UNK);              /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
M2C_UNK strcpy1(s32, M2C_UNK *);                    /* extern */
extern M2C_UNK D_002A7AA8;
extern M2C_UNK D_003DD490;

void func_001DDF60(s32 arg0) {
    strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, 0x40));
    strcpy1(arg0, &D_002A7AA8);
}
