#include "m2c_prelude.h"
s32 PchzFindSwLangstr__FP2SWi(M2C_UNK *, s32);                  /* extern */
s32 func_001B0078(M2C_UNK *);                       /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
extern M2C_UNK D_003D9ED0;
extern M2C_UNK D_003DD490;

void func_001AD928(s32 arg0) {
    strcpy(arg0, PchzFindSwLangstr__FP2SWi(&D_003DD490, func_001B0078(&D_003D9ED0)));
}
