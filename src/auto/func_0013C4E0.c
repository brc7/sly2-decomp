#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32 *, s32 *);                /* extern */
M2C_UNK func_00129EC8(s32 *);                       /* extern */
M2C_UNK *func_001AE318(s32 *, s32, s32);            /* extern */
M2C_UNK func_002417F0(s32, M2C_UNK, M2C_UNK);       /* extern */
extern M2C_UNK D_0025A0C0;
extern M2C_UNK D_0025AF90;
extern M2C_UNK D_002A2D00;
extern M2C_UNK D_002A2D30;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern s128 D_002A2DE0;
extern f32 D_002A4AF4;
extern f32 D_002A4AF8;
extern f32 D_002A4AFC;
extern f32 D_002A4B00;
extern f32 D_002A4B08;
extern f32 D_002A4B18;
extern f32 D_002A4B1C;
extern f32 D_002A4B20;
extern M2C_UNK D_002A4BB8;
extern M2C_UNK D_002A4BC0;
extern s32 D_002BC9C0;
extern f32 D_002BC9C8;
extern s32 D_002BC9D0;
extern s32 D_002C3C5C;
extern M2C_UNK D_002CB5D8;
extern s128 *D_002DD2FC;
extern M2C_UNK D_002DD5C0;
extern M2C_UNK D_002DD600;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern s32 D_002DE59C;
extern M2C_UNK D_002DEA28;
extern M2C_UNK D_002DEAF0;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA0C0;
extern M2C_UNK D_003DD2CC;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern s32 D_003E0F9C;
extern void *D_003E13B4;
extern M2C_UNK D_390;
extern M2C_UNK D_870;

/** @sly2-annot addr=0x13C4E0
 * Purpose: If dropped: reset to 0 and, if its container is still in world, put it back there (state 1)
 * Info: proposed name ReturnDroppedBooty (T2); TU booty
 * Notes: names_v2 v1 proposed FGetAlarmSensorList__FP5ALARMPv (0.001)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void func_0013C4E0(s32 arg0) {
    s32 temp_s1;
    s32 temp_s2;

    if (M2C_FIELD(arg0, s32 *, 0x184) == 5) {
        temp_s1 = M2C_FIELD(arg0, s32 *, 0x15C);
        M2C_FIELD(arg0, s32 *, 0x15C) = 0;
        temp_s2 = M2C_FIELD(arg0, s32 *, 0x158);
        func_0013C560(arg0, 0);
        if ((temp_s1 != 0) && (func_001A61A0(temp_s1) != 0)) {
            M2C_FIELD(arg0, s32 *, 0x15C) = temp_s1;
            M2C_FIELD(arg0, s32 *, 0x158) = temp_s2;
            func_0013C560(arg0, 1);
        }
    }
}
