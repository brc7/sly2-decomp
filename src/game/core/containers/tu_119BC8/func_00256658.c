#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_00256498 (initial). */

/* Warning: missing "jr $ra" in last block of func_002564C8 (initial). */

/* Warning: missing "jr $ra" in last block of func_00258008 (initial). */

M2C_UNK Read__18CBinaryInputStreamiPv(s32, M2C_UNK, s32);           /* extern */
M2C_UNK Decref__4CRef(void *);                      /* extern */
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

/** @sly2-annot addr=0x256658
 * Purpose: free array and destroy list nodes via vt [game linkonce (inline/template)]
 * Info: proposed name CDynArrayV_Clear (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00256658(s32 arg0) {
    void *temp_v0;
    void *var_s0;

    Free__9CDynArray(arg0 + 0x10);
    var_s0 = M2C_FIELD(arg0, void **, 0x28);
    if (var_s0 != NULL) {
        do {
            temp_v0 = M2C_FIELD(var_s0, void **, 8);
            M2C_FIELD(temp_v0, void (**)(void *), 0xC)(var_s0 + M2C_FIELD(temp_v0, s16 *, 8));
            var_s0 = M2C_FIELD(var_s0, void **, 4);
        } while (var_s0 != NULL);
    }
}
