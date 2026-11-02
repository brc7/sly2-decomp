#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of junk_001B5B48 (initial). */

M2C_UNK Read__18CBinaryInputStreamiPv(s32, M2C_UNK, void *);        /* extern */
M2C_UNK AllocStack__4DMASi(M2C_UNK *, s32, s32, M2C_UNK); /* extern */
M2C_UNK DetachCopySw__4DMASPiPP2QWT2(M2C_UNK *, M2C_UNK, M2C_UNK, s32 *); /* extern */
M2C_UNK __4GIFS(void *);                      /* extern */
s32 func_001AE318(void *, s32, s32);                /* extern */
M2C_UNK __4VIFS(M2C_UNK *);                   /* extern */
f32 powf(f32 *, f32 *, M2C_UNK, f32, f32, f32); /* extern */
extern M2C_UNK D_002BAAE8;
extern M2C_UNK D_002BBB60;
extern f32 D_002C3690;
extern f32 D_002C3694;
extern f32 D_002C3698;
extern M2C_UNK D_002C36A0;
extern M2C_UNK D_002C3728;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern s32 D_003DA790;
extern M2C_UNK D_003DA798;
extern s32 D_003DCF98;
extern s32 D_003DCF9C;
extern s32 D_003DCFF0;
extern s32 D_003DCFF4;
extern s32 D_003DCFF8;
extern s32 D_003DCFFC;
extern s32 D_003DD00C;
extern s32 D_003DD010;
extern s32 D_003DD014;
extern s32 D_003DD018;
extern s32 D_003E12C8;
extern void *D_003E159C;

/** @sly2-annot addr=0x1EE480
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void InitScroller__FP8SCROLLERP4SAAF(s32 arg0, s32 arg1) {
    func_001ED8E0(arg0, arg1);
    M2C_FIELD(arg0, f32 *, 0x50) = (f32) M2C_FIELD(arg1, f32 *, 0xC);
    M2C_FIELD(arg0, f32 *, 0x54) = (f32) M2C_FIELD(arg1, f32 *, 0x10);
    M2C_FIELD(arg0, f32 *, 0x58) = (f32) M2C_FIELD(arg1, f32 *, 0x14);
    M2C_FIELD(arg0, f32 *, 0x5C) = (f32) M2C_FIELD(arg1, f32 *, 0x18);
    M2C_FIELD(arg0, f32 *, 0x64) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x60) = 1.0f;
}
