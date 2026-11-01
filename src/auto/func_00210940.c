#include "m2c_prelude.h"
M2C_UNK func_001179D8(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
M2C_UNK func_00166920(M2C_UNK *);                   /* extern */
s32 func_001AE318(s32 *);                           /* extern */
extern M2C_UNK D_0025AFA0;
extern M2C_UNK D_0025B870;
extern M2C_UNK D_002A8A78;
extern M2C_UNK D_002A8AA0;
extern M2C_UNK D_002A8AA8;
extern M2C_UNK D_002A8AB0;
extern M2C_UNK D_002A8AB8;
extern M2C_UNK D_002A8AC0;
extern M2C_UNK D_002A8AC8;
extern M2C_UNK D_002A8AD0;
extern M2C_UNK D_002A8AD8;
extern M2C_UNK D_002A8AE8;
extern s32 D_002BE538;
extern M2C_UNK D_002C3288;
extern M2C_UNK D_002C3EB8;
extern M2C_UNK D_002C3EC0;
extern M2C_UNK D_002C9670;
extern M2C_UNK D_002DAF30;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern s32 D_002DD318;
extern f32 D_002DDED0;
extern f32 D_002DDF0C;
extern f32 D_002DDF10;
extern f32 D_002DE3B8;
extern f32 D_002DE514;
extern M2C_UNK D_002DEA28;
extern s64 D_002DEBF0;
extern s64 D_002DECE0;
extern s32 D_002DEDB0;
extern M2C_UNK D_002DF010;
extern M2C_UNK D_002DF800;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_002E6550;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003D9EDC;
extern s32 D_003D9EF4;
extern M2C_UNK D_003DA4F0;
extern M2C_UNK D_003DD490;
extern void *D_003E1060;
extern void *D_003E106C;
extern M2C_UNK D_003E1C20;
extern s32 D_003E1C38;
extern s32 D_003E1C40;
extern s32 D_003E1C44;
extern s32 D_003E1C48;
extern M2C_UNK D_003E1C60;
extern M2C_UNK D_003E1C70;
extern M2C_UNK D_003E1C80;
extern M2C_UNK D_004421A0;
extern M2C_UNK D_004421B0;
extern M2C_UNK D_00442210;
extern M2C_UNK D_00442240;
extern M2C_UNK D_004424B0;
extern M2C_UNK D_004424D0;
extern M2C_UNK D_004424F0;
extern M2C_UNK D_00442520;
extern M2C_UNK D_00442560;
extern M2C_UNK D_004425A0;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;
extern M2C_UNK D_100;
extern M2C_UNK D_C0;

/** @sly2-annot addr=0x210940
 * Purpose: SW+0x4638 = PloFindSwObject(psw,5,oid 0x620,0)
 * Info: proposed name BindSwObject620 (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210940(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    M2C_FIELD(arg0, s32 *, 0x4638) = func_0015E938(arg0, 5, 0x620, 0, arg4, arg5, arg6);
}
