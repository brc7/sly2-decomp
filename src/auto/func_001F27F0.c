#include "m2c_prelude.h"
M2C_UNK RecalcSwOxfFilterForObject(M2C_UNK *, void *);           /* extern */
void *PsoFindPhysRoot();                              /* extern */
extern M2C_UNK D_003DD490;

void func_001F27F0(void *arg0) {
    void *temp_v0;
    void *temp_v1;

    if (M2C_FIELD(arg0, s32 *, 0xEC) == 0) {
        temp_v0 = PsoFindPhysRoot();
        if (M2C_FIELD(temp_v0, s32 *, 0xEC) == 0) {
            temp_v1 = M2C_FIELD(temp_v0, void **, 0xC);
            M2C_FIELD(temp_v0, s32 *, 0xEC) = M2C_FIELD(temp_v1, s32 (**)(void *), 0x1AC)(temp_v0 + M2C_FIELD(temp_v1, s16 *, 0x1A8));
            RecalcSwOxfFilterForObject(&D_003DD490, temp_v0);
        }
        M2C_FIELD(arg0, s32 *, 0xEC) = M2C_FIELD(temp_v0, s32 *, 0xEC);
    }
}
