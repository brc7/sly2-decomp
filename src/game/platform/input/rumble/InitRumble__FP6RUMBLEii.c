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

/** @sly2-annot addr=0x1D5150
 * Purpose: Init RUMBLE: state Idle (or Stop if already active), store port/slot
 * Info: proposed name InitRumble__FP6RUMBLEii (EXISTING); TU rumble
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/main-loop-startup.md#7-source-files,
 *   docs/engine/main-loop-startup.md#8-key-functions
 * @end-sly2-annot */
void InitRumble__FP6RUMBLEii(s32 arg0, s32 arg1, s32 arg2) {
    if (M2C_FIELD(arg0, s32 *, 8) == 0) {
        SetRumbleRums__FP6RUMBLE4RUMS(arg0, 1);
    } else {
        SetRumbleRums__FP6RUMBLE4RUMS(arg0, 3);
    }
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = arg2;
}
