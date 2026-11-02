#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32 *, s32 *);                /* extern */
M2C_UNK Decref__4CRef(s32 *);                       /* extern */
M2C_UNK *func_001AE318(s32 *, s32, s32);            /* extern */
M2C_UNK sceDmaSync(s32, M2C_UNK, M2C_UNK);       /* extern */
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

/** @sly2-annot addr=0x13A830
 * Purpose: Pass 2 begin (kind 12): upload blip microprogram
 * Info: proposed name BeginRpBlip (T1); TU blip
 * Notes: superseded: EnsureBlipVu1Code (r2b_rest)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void BeginRpBlip(s32 arg0) {
    EnsureVu1Code__FP4VIFSPvT1(arg0, (s32) &D_0025A0C0, (s32) &D_0025AF90);
}
