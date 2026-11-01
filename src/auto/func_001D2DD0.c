#include "m2c_prelude.h"
M2C_UNK func_001291A8(M2C_UNK *, M2C_UNK *, s32, s32); /* extern */
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
s32 func_001AE318(s128 *);                          /* extern */
extern s128 D_002A2DB0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A7834;
extern f32 D_002A7838;
extern f32 D_002A783C;
extern f32 D_002A7870;
extern f32 D_002A7874;
extern M2C_UNK D_002A7878;
extern M2C_UNK D_002C20D0;
extern M2C_UNK D_002C20E0;
extern M2C_UNK D_002C2100;
extern f32 D_002C2110;
extern f32 D_002C2114;
extern M2C_UNK D_002C2120;
extern M2C_UNK D_002C2130;
extern f32 D_002C2144;
extern f32 D_002C2148;
extern M2C_UNK D_002C21A0;
extern M2C_UNK D_002C21B0;
extern M2C_UNK D_002C21C0;
extern f32 D_002C21CC;
extern M2C_UNK D_002C21D0;
extern M2C_UNK D_002C21D8;
extern M2C_UNK D_002C21E0;
extern M2C_UNK D_002C21E8;
extern M2C_UNK D_002C21F0;
extern M2C_UNK D_002C21F8;
extern M2C_UNK D_002C2200;
extern M2C_UNK D_002C2208;
extern s128 D_002C2230;
extern M2C_UNK D_002C2248;
extern M2C_UNK D_002C2260;
extern s32 D_002C2D8C;
extern M2C_UNK D_002C44F0;
extern M2C_UNK D_002C94A0;
extern f32 D_002DDED0;
extern f32 D_002DDF10;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE2E0;
extern M2C_UNK D_002DEA28;
extern s32 D_002DFBC0;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_002E0D00;
extern M2C_UNK D_003B3980;
extern void *D_003D4A50;
extern s32 D_003D4A54;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA150;
extern M2C_UNK D_003DD490;
extern s32 D_003E0FD8;

/** @sly2-annot addr=0x1D2DD0
 * Purpose: Return the contact in an XP list with the largest +0x90 value (deepest)
 * Info: proposed name PxpBestRobottankWheel (T2); TU tu_1D1370
 * Notes: class identity from whole_game WE names/object strings: PxpBestRcCarWheel -> PxpBestRobottankWheel
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
s32 func_001D2DD0(s32 arg0, s32 arg1) {
    f32 temp_f0;
    f32 var_f1;
    void *var_v0;
    void *var_v1;

    var_v1 = M2C_FIELD(arg1, void **, 8);
    var_f1 = D_002A7834;
    var_v0 = NULL;
    if (var_v1 != NULL) {
        do {
            temp_f0 = M2C_FIELD(var_v1, f32 *, 0x90);
            if (var_f1 < temp_f0) {
                var_f1 = temp_f0;
                var_v0 = var_v1;
            }
            var_v1 = M2C_FIELD(var_v1, void **, 0xAC);
        } while (var_v1 != NULL);
    }
    return (s32) var_v0;
}
