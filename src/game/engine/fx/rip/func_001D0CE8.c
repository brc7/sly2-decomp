#include "m2c_prelude.h"
extern M2C_UNK D_002A2D00;
extern s128 D_002A2DA0;
extern s128 D_002A2DB0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A7810;
extern s128 D_002BBB50;
extern M2C_UNK D_002BBB60;
extern s128 D_002C1F50;
extern f32 D_002C1F60;
extern f32 D_002C1F68;
extern f32 D_002C1F6C;
extern s32 D_002C1F70;
extern M2C_UNK D_002C1F78;
extern M2C_UNK D_002C1F80;
extern M2C_UNK D_002C1FB0;
extern M2C_UNK D_002C1FC0;
extern M2C_UNK D_002C1FD0;
extern M2C_UNK D_002C1FE0;
extern M2C_UNK D_002C1FF0;
extern f32 D_002C2000;
extern M2C_UNK D_002C2010;
extern M2C_UNK D_002C2020;
extern M2C_UNK D_002C2040;
extern M2C_UNK D_002C2050;
extern M2C_UNK D_002C2060;
extern M2C_UNK D_002C2070;
extern M2C_UNK D_002C2080;
extern M2C_UNK D_002C2090;
extern M2C_UNK D_002C2098;
extern f32 D_002C20A4;
extern M2C_UNK D_002C20A8;
extern M2C_UNK D_002C20C0;
extern M2C_UNK D_002C94A0;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E1144;
extern M2C_UNK D_003E14E4;

/*
Decompilation failure in function func_001CE6D0:

Found jr instruction at chunk.s line 21, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

/** @sly2-annot addr=0x1D0CE8
 * Purpose: RIPT 1 PostEmit
 * Info: proposed name PostRipt1Emit (T2); TU rip
 * Notes: names_v2 v1 proposed InitRose__FP4ROSEP6VECTORfP2SO (0.438)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
s32 func_001D0CE8(s32 arg0, s32 arg1) {
    void *temp_v0;

    func_001CE9B8();
    temp_v0 = M2C_FIELD(arg1, void **, 8);
    M2C_FIELD(arg0, s32 *, 0xF4) = (s32) M2C_FIELD(temp_v0, s32 *, 0xC);
    return (s32) temp_v0;
}
