#include "m2c_prelude.h"
M2C_UNK _$_18CBinaryAsyncStream(M2C_UNK *, M2C_UNK); /* extern */
M2C_UNK func_00166920(void *, s32, s32, M2C_UNK);   /* extern */
void **func_001AE318(void *, s32, s32);             /* extern */
M2C_UNK func_0024A9D8(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_0024B480();                            /* extern */
s32 func_0024B540(s32, s32, M2C_UNK);               /* extern */
M2C_UNK func_0024B590(s32);                         /* extern */
M2C_UNK func_0024B628(s32, M2C_UNK, s32 (*)(s32, s32, s32), s32); /* extern */
M2C_UNK func_0024F790(s32, M2C_UNK, s32, s32 (*)(s32, s32, s32)); /* extern */
extern M2C_UNK D_002A2D30;
extern s128 D_002A2DD0;
extern M2C_UNK D_002A3968;
extern M2C_UNK D_002A39A8;
extern s32 D_002BD4C0;
extern f32 D_002BF730;
extern s32 D_002C0268;
extern M2C_UNK D_002C0270;
extern f32 D_002C08DC;
extern f32 D_002C0900;
extern M2C_UNK D_002C0908;
extern M2C_UNK D_002C0948;
extern f32 D_002C0984;
extern f32 D_002C0988;
extern M2C_UNK D_002C0990;
extern f32 D_002C09A0;
extern f32 D_002C09AC;
extern M2C_UNK D_002C09B0;
extern f32 D_002C09B8;
extern M2C_UNK D_002C09C0;
extern f32 D_002C09C8;
extern M2C_UNK D_002D4220;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDF0C;
extern s32 D_002DE760;
extern s32 D_002DEDBC;
extern M2C_UNK D_002DFC00;
extern s32 D_002E1E54;
extern M2C_UNK D_002E65E0;
extern M2C_UNK D_002F66B0;
extern s32 D_002F68F8;
extern s32 D_002F7900;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern void *D_003E0714;
extern void *D_003E13B4;
extern M2C_UNK D_003E1C20;
extern M2C_UNK D_004435F0;

/** @sly2-annot addr=0x1B33C8
 * Purpose: audio stream callback -> CMpegAudio::FAccept(pmpeg+0x180)
 * Info: proposed name FMpegAcceptAudio (T1); TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void FMpegAcceptAudio__FP7sceMpegP16sceMpegCbDataStrP5CMpeg(s32 arg0, s32 arg1, s32 arg2) {
    FAccept__10CMpegAudioiPUc(arg2 + 0x180, M2C_FIELD(arg1, s32 *, 0xC), M2C_FIELD(arg1, s32 *, 8));
}
