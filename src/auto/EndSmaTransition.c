#include "m2c_prelude.h"
extern M2C_UNK D_002C3738;
extern M2C_UNK D_002C3748;
extern M2C_UNK D_002C3968;
extern M2C_UNK D_002D9768;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDFA0;
extern M2C_UNK D_002DE578;
extern M2C_UNK D_002DEB30;
extern s32 D_002E1E40;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E126C;
extern M2C_UNK D_003E1278;

void EndSmaTransition(s32 arg0) {
    s32 temp_a1;
    s32 temp_a3;

    M2C_FIELD(arg0, s32 *, 0x30) = 0;
    temp_a3 = M2C_FIELD(arg0, s32 *, 0x38);
    temp_a1 = M2C_FIELD(arg0, s32 *, 0x34);
    M2C_FIELD(arg0, s32 *, 0x38) = -1;
    M2C_FIELD(arg0, s32 *, 0x34) = temp_a3;
    NotifySmaSpliceOnEnterState(arg0, temp_a1, temp_a3);
}
