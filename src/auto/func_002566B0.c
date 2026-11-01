#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_00256498 (initial). */

/* Warning: missing "jr $ra" in last block of func_002564C8 (initial). */

/* Warning: missing "jr $ra" in last block of func_00258008 (initial). */

void func_00117968(s32, M2C_UNK, s32);           /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
s32 func_001AE318(s32 *, s32 *);                    /* extern */
extern M2C_UNK D_002CB1F0;
extern M2C_UNK D_002D9978;
extern M2C_UNK D_002DA568;
extern M2C_UNK D_002DA9D0;
extern void *D_002DD280;
extern u32 D_002DD284;
extern s32 D_002DD288;
extern s32 D_002DD28C;
extern void *D_002DD290;
extern u32 D_002DD294;
extern s32 D_002DD298;
extern s32 D_002DD29C;
extern void *D_002DD2A0;
extern u32 D_002DD2A4;
extern s32 D_002DD2A8;
extern s32 D_002DD2AC;
extern void *D_002DD2B0;
extern u32 D_002DD2B4;
extern s32 D_002DD2B8;
extern s32 D_002DD2BC;
extern void *D_002DD2C0;
extern u32 D_002DD2C4;
extern s32 D_002DD2C8;
extern s32 D_002DD2CC;
extern void *D_002DD2D0;
extern u32 D_002DD2D4;
extern void *D_002DD2D8;
extern void *D_002DD2DC;
extern s32 D_002DD598;
extern s32 D_003E0EA4;
extern M2C_UNK D_00452098;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520C0;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520E8;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452110;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452138;
extern M2C_UNK D_00452148;
extern u32 D_00452160;
extern M2C_UNK D_00452170;

/** @sly2-annot addr=0x2566B0
 * Purpose: virtual: read 4 bytes from binary stream (0x117968) [game linkonce (inline/template)]
 * Info: proposed name LoadS32FromBis_vt (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002566B0(s32 arg0, s32 arg1, s32 arg2) {
    func_00117968(arg2, 4, arg1);
}
