#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_001A73B0 (initial). */

/* Warning: missing "jr $ra" in last block of func_001ABFF8 (initial). */

s32 func_00117898(M2C_UNK *, M2C_UNK, M2C_UNK *);   /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
s32 func_001AE318(s32 *, s32, s32 *, M2C_UNK);      /* extern */
M2C_UNK func_002355E0(s32, s32);                    /* extern */
M2C_UNK func_00243CB0();                            /* extern */
s32 func_00244450(M2C_UNK, void *, s32 *);          /* extern */
s32 func_002446F0(s32, s32, M2C_UNK *, M2C_UNK);    /* extern */
s32 func_00244A58(s32, s32, M2C_UNK *);             /* extern */
extern M2C_UNK D_002A38E8;
extern M2C_UNK D_002A6A58;
extern f32 D_002A6AD0;
extern M2C_UNK D_002A6BB0;
extern M2C_UNK D_002A6BC8;
extern M2C_UNK D_002A6BF8;
extern M2C_UNK D_002A6C00;
extern M2C_UNK D_002A6C08;
extern M2C_UNK D_002A6C28;
extern M2C_UNK D_002A6C50;
extern M2C_UNK D_002A6C60;
extern M2C_UNK D_002A6C78;
extern M2C_UNK D_002A6C98;
extern M2C_UNK D_002A6CA0;
extern M2C_UNK D_002A6CB8;
extern M2C_UNK D_002A6CC0;
extern M2C_UNK D_002BBB20;
extern s32 D_002BE538;
extern s32 D_002BFCE0;
extern f32 D_002BFD30;
extern f32 D_002BFD34;
extern M2C_UNK D_002C46C0;
extern M2C_UNK D_002C46D8;
extern s32 D_002DD644;
extern M2C_UNK D_002DDED0;
extern f32 D_002DDF0C;
extern s32 D_002DEB4C;
extern M2C_UNK D_002DFC00;
extern s32 D_002E2050;
extern s32 D_002E2054;
extern M2C_UNK D_002E2080;
extern M2C_UNK D_002E5270;
extern M2C_UNK D_002E6080;
extern M2C_UNK D_002E6500;
extern M2C_UNK D_002E6550;
extern M2C_UNK D_002E65A0;
extern M2C_UNK D_002E65E0;
extern M2C_UNK D_002F66B0;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003D9E70;
extern M2C_UNK D_003D9ED0;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern s32 D_004425D8;

/** @sly2-annot addr=0x1AC238
 * Purpose: existing: load memcard_icon.ico from disc and write to card
 * Info: proposed name load_sly_icon_from_disc (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.70] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/formats/disc-records.md#3-static-records-in-the-elf,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void load_sly_icon_from_disc(s32 arg0, s32 arg1) {
    load_file_from_disc(arg0, arg1, (s32) &D_002A38E8, (s32) &D_002A6CA0);
}
