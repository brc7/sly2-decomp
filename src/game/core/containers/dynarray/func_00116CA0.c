#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of junk_0x0011d5d8 (initial). */

/* Warning: missing "jr $ra" in last block of func_00117640 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
extern M2C_UNK D_0027A778;
extern M2C_UNK D_0029A790;
extern M2C_UNK D_0029A7C0;
extern M2C_UNK D_0029A8C0;
extern M2C_UNK D_002A28D0;
extern M2C_UNK D_002A2900;
extern M2C_UNK D_002CA550;
extern M2C_UNK D_002CC1C0;
extern M2C_UNK D_002CC240;
extern M2C_UNK D_002CC2C0;
extern M2C_UNK D_002CC340;
extern M2C_UNK D_002CC3C0;
extern M2C_UNK D_002CC440;
extern M2C_UNK D_002CCE90;
extern M2C_UNK D_002CDD40;
extern M2C_UNK D_002CDDC0;
extern M2C_UNK D_002CFA10;
extern M2C_UNK D_002CFC20;
extern M2C_UNK D_002CFCA0;
extern M2C_UNK D_002CFD20;
extern M2C_UNK D_002CFDA0;
extern M2C_UNK D_002CFE20;
extern M2C_UNK D_002CFEA0;
extern M2C_UNK D_002CFF20;
extern M2C_UNK D_002CFFA0;
extern M2C_UNK D_002D0020;
extern M2C_UNK D_002D00A0;
extern M2C_UNK D_002D0120;
extern M2C_UNK D_002D01A0;
extern M2C_UNK D_002D0220;
extern M2C_UNK D_002D02A0;
extern M2C_UNK D_002D0320;
extern M2C_UNK D_002D03A0;
extern M2C_UNK D_002D0420;
extern M2C_UNK D_002D4AF8;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern M2C_UNK D_002DD500;
extern M2C_UNK D_002DD520;
extern M2C_UNK D_002DD548;
extern M2C_UNK D_002DD568;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

/** @sly2-annot addr=0x116CA0
 * Purpose: memcmp(a,b,4) (tail-returns result)
 * Info: proposed name NCmpAb4 (T2); TU dynarray
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/core-library.md#1-memory, docs/engine/core-library.md#6-source-files,
 *   docs/engine/core-library.md#open-questions
 * @end-sly2-annot */
void func_00116CA0(s32 arg0, s32 arg1, s32 arg2) {
    memcmp(arg0, arg1, 4);
}
