#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0015F058 (initial). */

/* Warning: missing "jr $ra" in last block of func_0015F700 (initial). */

/* Warning: missing "jr $ra" in last block of func_0015FD58 (initial). */

M2C_UNK func_001291A8(s32 *, s32 *, s32);           /* extern */
M2C_UNK Decref__4CRef(s32 *);                       /* extern */
M2C_UNK __4GIFS(M2C_UNK *);                   /* extern */
M2C_UNK AddVifGifs__4VIFSP4GIFS(s32, M2C_UNK *);              /* extern */
s32 func_002355E0(s32, M2C_UNK);                    /* extern */
M2C_UNK sceDmaSync(s32, M2C_UNK, M2C_UNK);       /* extern */
extern M2C_UNK D_0025E680;
extern M2C_UNK D_00262320;
extern M2C_UNK D_002AA6A1;
extern M2C_UNK D_002BAAE8;
extern s32 D_002BE320;
extern u8 D_002BE328;
extern M2C_UNK D_002BE338;
extern M2C_UNK D_002C44F0;
extern void *D_002DD2FC;
extern M2C_UNK D_002DDED0;
extern s32 D_002DDF54;
extern M2C_UNK D_002DE578;
extern s32 D_002DE594;
extern s32 D_002DE598;
extern s32 D_002DE760;
extern M2C_UNK D_002DE770;
extern s32 D_002DE9F0;
extern M2C_UNK D_002DEA00;
extern M2C_UNK D_002DEA28;
extern void *D_002DEA4C;
extern M2C_UNK D_002DEA50;
extern M2C_UNK D_002DEAF0;
extern M2C_UNK D_002DEB00;
extern u32 D_002DEB0C;
extern u32 D_002DEB10;
extern void *D_002DEB14;
extern s32 D_002DEB18;
extern s32 D_002DEB1C;
extern s32 D_002DEB20;
extern s32 D_002DEB24;
extern u64 D_002DEBE0;
extern u64 D_002DECD0;
extern s32 D_002DEDB0;
extern s32 D_002DEDB8;
extern s32 D_002DEDBC;
extern M2C_UNK D_002DF740;
extern s32 D_002E0C20;
extern s32 D_002E1E00;
extern s32 D_003D3980;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DD490;
extern s32 D_003E12C8;
extern M2C_UNK D_003E1518;
extern M2C_UNK D_003E1530;
extern s32 D_004425D4;

/** @sly2-annot addr=0x15F928
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
s32 __9CRichTextPcP5CFont(s32 arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 0xC) = arg2;
    M2C_FIELD(arg0, s32 *, 0x160) = 0xFF808080;
    M2C_FIELD(arg0, s32 *, 0x15C) = 0xFF808080;
    M2C_FIELD(arg0, s32 *, 0x158) = 0xFF808080;
    M2C_FIELD(arg0, s32 *, 0x154) = 0xFF808080;
    Reset__9CRichText(arg0);
    return arg0;
}
