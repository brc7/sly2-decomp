#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, s32);                  /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern void *D_003DA4D0;
extern M2C_UNK D_003DD490;

void func_001DDE58(s32 arg0) {
    if (D_003DA4D0 != NULL) {
        strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, M2C_FIELD(D_003DA4D0, s32 *, 0x6C)));
    }
}
