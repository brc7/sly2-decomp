#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
void **func_001AE318(s32 *);                        /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002C3C20;
extern M2C_UNK D_002C44F0;
extern void *D_002DD2FC;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_003DD490;
extern void *D_003E1218;
extern void *D_003E126C;
extern M2C_UNK D_003E15A8;
extern s32 *D_003E15C0;

/** @sly2-annot addr=0x1F5720
 * Purpose: Set translation constraint from CNSTR: make pos XF dynamic if needed, store axis/mode in
 *   SOPHYS+0xB0, rebuild phys hook
 * Info: proposed name SetSoCnstrForce (T1); TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F5720(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_s1;

    if (M2C_FIELD(arg0, void **, 0xE8) != NULL) {
        temp_s1 = M2C_FIELD(arg0, void **, 0x58);
        if ((M2C_FIELD(temp_s1, s32 *, 0x90) == 0) && (M2C_FIELD(temp_s1, s16 *, 0x98) != 2)) {
            func_002303A0(arg0, (s32) temp_s1, 1);
        }
        func_001F5510(arg0, (s32) temp_s1, arg1, M2C_FIELD(arg0, void **, 0xE8) + 0xB0);
        func_001F55C8(arg0);
        if (M2C_FIELD(M2C_FIELD(arg0, void **, 0xE8), s32 *, 0xC0) != 3) {
            func_001F8668(arg0, 0);
        }
    }
}
