#include "m2c_prelude.h"
M2C_UNK func_001574A0(M2C_UNK *);                   /* extern */
s32 func_001AE318(s32 *, s32, s32);                 /* extern */
extern s128 D_002A2DD0;
extern f32 D_002A77D0;
extern f32 D_002A77D4;
extern M2C_UNK D_002AC880;
extern f32 D_002C1EC0;
extern f32 D_002C1EC4;
extern f32 D_002C1EC8;
extern f32 D_002C1ECC;
extern s32 D_002C1ED0;
extern s128 D_002C1EE0;
extern f32 D_002C1EF0;
extern f32 D_002C1EF4;
extern f32 D_002C1F20;
extern M2C_UNK D_002C1F28;
extern s32 D_002C1F30;
extern s32 D_002C1F34;
extern M2C_UNK D_002D77D8;
extern M2C_UNK D_002D9978;
extern s32 *D_002DD2FC;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEA00;
extern M2C_UNK D_002DEA28;
extern s32 D_002F7900;
extern M2C_UNK D_003B3980;
extern s32 D_003D4A50;
extern M2C_UNK D_003DA0F0;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003DD4BC;
extern s32 D_003E115C;
extern s32 D_003E1398;

/** @sly2-annot addr=0x1CE170
 * Purpose: RIPG vt slot 49: copy RIPG bounds (+0x170..+0x1A0) into its geometry/bound record (+0xEC), set
 *   dirty flag D_003E115C, call SO bound update 0x1F4BA8
 * Info: proposed name UpdateRipgBounds (T1); class RIPG slot49; TU rip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void UpdateRipgBounds(s32 arg0) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xEC), s128 *, 0) = (s128) M2C_FIELD(arg0, s128 *, 0x170);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xEC), s128 *, 0x10) = (s128) M2C_FIELD(arg0, s128 *, 0x180);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xEC), s128 *, 0x20) = (s128) M2C_FIELD(arg0, s128 *, 0x190);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xEC), f32 *, 0x30) = (f32) M2C_FIELD(arg0, f32 *, 0x1A0);
    D_003E115C = 1;
    func_001F4BA8(arg0);
}
