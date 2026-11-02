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

/** @sly2-annot addr=0x15F148
 * Purpose: Ensures the font texture is uploaded this frame: tracks a per-frame bit in g_pfrmOpen+0x2C when
 *   drawing into g_gifs, emits DMA REFs of the upload chain; returns whether a TEXFLUSH is needed
 * Info: proposed name FEnsureLoaded (T1); TU font
 * Notes: names_v2 B proposed FEnsureLoaded__8CFontBrxP4GIFS (0.895)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
s32 FEnsureLoaded(s32 arg0, s32 arg1) {
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s2;

    if (arg1 == &D_002DEA00) {
        temp_v0 = 1 << M2C_FIELD(arg0, s32 *, 0);
        temp_v1 = M2C_FIELD(D_002DEA4C, s32 *, 0x2C);
        var_s2 = (temp_v1 & temp_v0) == 0;
        M2C_FIELD(D_002DEA4C, s32 *, 0x2C) = (s32) (temp_v1 | temp_v0);
    } else {
        var_s2 = 1;
    }
    if (var_s2 != 0) {
        EndDmaCnt__4DMAS(arg1);
        AddDmaRefs__4DMASiP2QW(arg1, M2C_FIELD(arg0, s32 *, 0x58), M2C_FIELD(arg0, s32 *, 0x5C));
        AddDmaRefs__4DMASiP2QW(arg1, M2C_FIELD(arg0, s32 *, 0xC0), M2C_FIELD(arg0, s32 *, 0xC4));
        temp_a1 = M2C_FIELD(arg0, s32 *, 0xC8);
        if (temp_a1 != 0) {
            AddDmaRefs__4DMASiP2QW(arg1, temp_a1, M2C_FIELD(arg0, s32 *, 0xCC));
            AddDmaRefs__4DMASiP2QW(arg1, M2C_FIELD(arg0, s32 *, 0x130), M2C_FIELD(arg0, s32 *, 0x134));
        }
        AddDmaCnt__4DMAS(arg1);
    }
    return var_s2;
}
