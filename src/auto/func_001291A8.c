#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00129EC8(s32 *, s32);                  /* extern */
M2C_UNK func_0012A0B0(s32 *, M2C_UNK);              /* extern */
s32 func_001AE318(s32 *);                           /* extern */
s32 *func_001291A8(s32 *arg0, s32 *arg1);           /* static */
extern M2C_UNK D_002A43B0;
extern M2C_UNK D_002BBB90;
extern void *D_002DD2AC;
extern void *D_002DD2CC;
extern s32 D_002DD598;
extern void *D_003E0E80;
extern M2C_UNK D_004520A8;
extern s32 D_004520B8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520E8;
extern s32 D_00452108;
extern s32 D_00452130;
extern M2C_UNK D_00452138;
extern M2C_UNK D_00452188;
extern M2C_UNK D_004521A0;
extern M2C_UNK D_004521B8;
extern M2C_UNK D_004521D0;
extern M2C_UNK D_004521E8;

s32 *func_001291A8(s32 *arg0, s32 *arg1) {
    M2C_FIELD(arg0, s32 *, 0) = M2C_FIELD(arg1, s32 *, 0);
    M2C_FIELD(arg0, s32 *, 4) = (s32) M2C_FIELD(arg1, s32 *, 4);
    Incref__4CRef((s32) arg0);
    return arg0;
}
