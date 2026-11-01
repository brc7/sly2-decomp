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

/** @sly2-annot addr=0x211148
 * Purpose: STRINGGEN ids 0x2DD/0x2DE: text of recon selector selected id (ctx 0 -> 0x202730, P+0xBC) or
 *   pending caption (ctx 1 -> 0x2026F8, P+0xC0)
 * Info: proposed name BuildReconSelectionText (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211148(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a1;
    s32 var_v0;

    temp_v0 = func_001C4840((s32) &D_003DD490);
    if ((temp_v0 != 0) && (func_00132B10(temp_v0, 0x34) != 0)) {
        temp_a0 = M2C_FIELD(temp_v0, s32 *, 0xF38);
        var_a1 = 0;
        if (temp_a0 != 0) {
            if (arg1 != 0) {
                if (arg1 == 1) {
                    var_v0 = func_002026F8(temp_a0);
                    goto block_7;
                }
            } else {
                var_v0 = func_00202730(temp_a0);
block_7:
                var_a1 = var_v0;
            }
        }
        if (var_a1 != 0) {
            strcpy(arg0, var_a1);
        }
    }
}
