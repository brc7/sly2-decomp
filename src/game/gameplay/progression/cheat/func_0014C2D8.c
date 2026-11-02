#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(M2C_UNK, void *, s32);      /* extern */
M2C_UNK func_00129EC8(s32);                         /* extern */
void *func_001AE318(s32 *, s32);                    /* extern */
extern M2C_UNK D_002A2E50;
extern M2C_UNK D_002A2E58;
extern s32 D_002A2E5C;
extern s32 D_002A2E60;
extern M2C_UNK D_002A2E68;
extern f32 D_002A4EE8;
extern M2C_UNK D_002A4F00;
extern u16 D_002A4FD0;
extern M2C_UNK D_002A4FD8;
extern M2C_UNK D_002A4FE0;
extern M2C_UNK D_002A4FE8;
extern f32 D_002A4FF0;
extern f32 D_002A4FF4;
extern f32 D_002BD460;
extern M2C_UNK D_002BD468;
extern f32 D_002BD470;
extern f32 D_002BD474;
extern f32 D_002BD478;
extern f32 D_002BD47C;
extern M2C_UNK D_002BD480;
extern M2C_UNK D_002BD488;
extern M2C_UNK D_002BD490;
extern M2C_UNK D_002BD498;
extern M2C_UNK D_002BD4A0;
extern M2C_UNK D_002BD4B0;
extern s32 D_002BD4C0;
extern M2C_UNK D_002BD4E0;
extern M2C_UNK D_002BD518;
extern M2C_UNK D_002BD550;
extern M2C_UNK D_002BD588;
extern M2C_UNK D_002BD5C0;
extern M2C_UNK D_002BD5F8;
extern M2C_UNK D_002BD630;
extern M2C_UNK D_002BD668;
extern M2C_UNK D_002BD6A0;
extern M2C_UNK D_002BD6D8;
extern M2C_UNK D_002BD710;
extern M2C_UNK D_002BD748;
extern M2C_UNK D_002BD780;
extern M2C_UNK D_002BD7B8;
extern M2C_UNK D_002BD7F0;
extern M2C_UNK D_002BD810;
extern f32 D_002BD81C;
extern f32 D_002BD820;
extern f32 D_002BD824;
extern void *D_002DD2F8;
extern s32 D_002DD648;
extern u8 D_002DD680;
extern s32 D_002DDE80;
extern s32 D_002DDE84;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDEE0;
extern f32 D_002DDF0C;
extern M2C_UNK D_002DDF30;
extern s32 D_002DDF34;
extern M2C_UNK D_002DDF40;
extern s32 D_002DFBC0;
extern M2C_UNK D_002DFC00;
extern s32 D_002E0CB4;
extern M2C_UNK D_002E0D00;
extern s32 D_003DA0B8;
extern M2C_UNK D_003DA4F0;
extern M2C_UNK D_003DD040;
extern s32 D_003DD0E4;
extern M2C_UNK D_003DD490;
extern s32 D_003E1450;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x14C2D8
 * Purpose: InitDl(pdl, 0x18) (DLE at CODE+0x18), then AddCode for the 15 records 0x2BD4E0..0x2BD7F0
 * Info: proposed name StartupCodes (T1); TU cheat
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/cheat
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void func_0014C2D8(s32 arg0) {
    func_0011A050(arg0, 0x18);
    func_0014BEA8(arg0, (s32) &D_002BD4E0);
    func_0014BEA8(arg0, (s32) &D_002BD518);
    func_0014BEA8(arg0, (s32) &D_002BD550);
    func_0014BEA8(arg0, (s32) &D_002BD588);
    func_0014BEA8(arg0, (s32) &D_002BD5C0);
    func_0014BEA8(arg0, (s32) &D_002BD5F8);
    func_0014BEA8(arg0, (s32) &D_002BD630);
    func_0014BEA8(arg0, (s32) &D_002BD668);
    func_0014BEA8(arg0, (s32) &D_002BD6A0);
    func_0014BEA8(arg0, (s32) &D_002BD6D8);
    func_0014BEA8(arg0, (s32) &D_002BD710);
    func_0014BEA8(arg0, (s32) &D_002BD748);
    func_0014BEA8(arg0, (s32) &D_002BD780);
    func_0014BEA8(arg0, (s32) &D_002BD7B8);
    func_0014BEA8(arg0, (s32) &D_002BD7F0);
}
