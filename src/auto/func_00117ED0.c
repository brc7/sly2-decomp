#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_00119C30 (initial). */

/* Warning: missing "jr $ra" in last block of func_0011B040 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00117898();                            /* extern */
s32 func_001AE318(s32 *, s32, s32);                 /* extern */
extern u8 D_002A4150;
extern M2C_UNK D_002AA6A1;
extern M2C_UNK D_002BBAF0;
extern M2C_UNK D_002BBB10;
extern M2C_UNK D_002BBB20;
extern void *D_002DD2F8;
extern void *D_002DD2FC;

/** @sly2-annot addr=0x117ED0
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void func_00117ED0(s32 arg0, s32 arg1) {
    func_00117EB0();
    func_00117EB0(arg0, arg1 + 0x10);
    func_00117EB0(arg0, arg1 + 0x20);
}
