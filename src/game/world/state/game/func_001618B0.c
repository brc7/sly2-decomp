#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0015F058 (initial). */

/* Warning: missing "jr $ra" in last block of func_0015F700 (initial). */

/* Warning: missing "jr $ra" in last block of func_0015FD58 (initial). */

M2C_UNK func_001291A8(s32 *, s32 *, s32, s32);      /* extern */
M2C_UNK func_00129EC8(s32 *);                       /* extern */
M2C_UNK func_002271C8(s32, M2C_UNK *);              /* extern */
M2C_UNK func_002355E0(s32, M2C_UNK);                /* extern */
M2C_UNK func_002417F0(s32, M2C_UNK, M2C_UNK);       /* extern */
extern M2C_UNK D_0025E680;
extern M2C_UNK D_00262320;
extern M2C_UNK D_002AA6A1;
extern M2C_UNK D_002BAAE8;
extern s32 D_002BE320;
extern u8 D_002BE328;
extern M2C_UNK D_002BE338;
extern M2C_UNK D_002C44F0;
extern void *D_002DD2FC;
extern f32 D_002DDED0;
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
extern s32 D_003E0FB0;
extern s32 D_003E12C8;
extern M2C_UNK D_003E1518;
extern M2C_UNK D_003E1530;
extern s32 D_004425D4;

/** @sly2-annot addr=0x1618B0
 * Purpose: Same as 0x161868 for the HUD widget g_sw+0x3BB8
 * Info: proposed name ShowSwWidget3BB8 (T2); TU game
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001618B0(s32 arg0) {
    void *temp_s0;
    void *temp_v0;

    temp_s0 = M2C_FIELD(arg0, void **, 0x3BB8);
    if (temp_s0 != NULL) {
        temp_v0 = M2C_FIELD(temp_s0, void **, 0xC);
        M2C_FIELD(temp_v0, void (**)(void *, M2C_UNK), 0x18C)(temp_s0 + M2C_FIELD(temp_v0, s16 *, 0x188), -1);
        func_00212A30((s32) temp_s0);
    }
}
