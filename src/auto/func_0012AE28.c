#include "m2c_prelude.h"
M2C_UNK ResetSlotheap(M2C_UNK *);                   /* extern */
M2C_UNK func_00257E88();                            /* extern */
M2C_UNK func_00257EC8();                            /* extern */
M2C_UNK func_00257F08();                            /* extern */
M2C_UNK func_00257F48();                            /* extern */
M2C_UNK func_00257F88();                            /* extern */
M2C_UNK func_00257FC8();                            /* extern */
extern s32 D_002DD5B8;
extern M2C_UNK D_00452188;
extern M2C_UNK D_004521A0;
extern M2C_UNK D_004521B8;
extern M2C_UNK D_004521D0;
extern M2C_UNK D_004521E8;

void func_0012AE28(void) {
    D_002DD5B8 = 0;
    func_00257E88();
    func_00257EC8();
    func_00257F08();
    func_00257F48();
    func_00257F88();
    func_00257FC8();
    ResetSlotheap(&D_00452188);
    ResetSlotheap(&D_004521A0);
    ResetSlotheap(&D_004521B8);
    ResetSlotheap(&D_004521D0);
    ResetSlotheap(&D_004521E8);
}
