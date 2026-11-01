#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_001574A8 (initial). */

s32 func_00116970(s32, s32, s32, M2C_UNK);          /* extern */
M2C_UNK func_00117EB0(s32, s32);                    /* extern */
s32 *func_001AE318(s32 *, s32, s32);                /* extern */
void DetachCopySw__4DMASPiPP2QWT2i(M2C_UNK *arg0, s32 arg1, u32 *arg2, s32 *arg3); /* static */
void func_001574A0(s32, s32 *);                     /* static */
extern s128 D_002A2DD0;
extern f32 D_002A5130;
extern f32 D_002A5134;
extern f32 D_002A5138;
extern f32 D_002A513C;
extern s32 D_002BDC50;
extern M2C_UNK D_002BDC60;
extern M2C_UNK D_002BDC70;
extern M2C_UNK D_002BDC80;
extern M2C_UNK D_002BDC90;
extern f32 D_002BDC98;
extern f32 D_002BDCA0;
extern f32 D_002BDCD0;
extern M2C_UNK D_002BDCF0;
extern f32 D_002BDD00;
extern f32 D_002BDD04;
extern f32 D_002BDD08;
extern f32 D_002BDD0C;
extern f32 D_002BDD10;
extern f32 D_002BDD14;
extern f32 D_002BDD18;
extern M2C_UNK D_002BDD20;
extern M2C_UNK D_002BDD88;
extern M2C_UNK D_002BDD8C;
extern M2C_UNK D_002C1F00;
extern M2C_UNK D_002C1F10;
extern M2C_UNK D_002CE2B8;
extern void *D_002DD2FC;
extern M2C_UNK D_002DD590;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE578;
extern s32 D_002DE5A4;
extern M2C_UNK D_002DE5B0;
extern s128 D_002DE5F0;
extern M2C_UNK D_002DEA00;
extern M2C_UNK D_002DEA28;
extern void *D_002DEA4C;
extern M2C_UNK D_002DEA50;
extern M2C_UNK D_002DEDC0;
extern M2C_UNK D_002DF010;
extern M2C_UNK D_002DF800;
extern M2C_UNK D_002DF940;
extern M2C_UNK D_002DFA80;
extern M2C_UNK D_003B3980;
extern s32 D_003D4A50;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E12B8;
extern void *D_003E148C;
extern M2C_UNK D_003E1498;

void func_001566C8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x10) - 1;
    M2C_FIELD(arg0, s32 *, 0x10) = temp_v0;
    if (temp_v0 <= 0) {
        func_00256EE0(arg0, 0);
        func_002580A8(arg0 + 4, 0);
        UnsubscribeLoAll(arg0);
        func_0011CD98((s32) &D_003E1498, arg0);
    }
}
