#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_00119C30 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
extern u8 D_002A4150;
extern M2C_UNK D_002AA6A1;
extern M2C_UNK D_002BBAF0;
extern M2C_UNK D_002BBB10;
extern M2C_UNK D_002BBB20;
extern void *D_002DD2F8;
extern void *D_002DD2FC;

/** @sly2-annot addr=0x117F70
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void ReadVector4__18CBinaryInputStreamP7VECTOR4(s32 arg0, s32 arg1) {
    Read__18CBinaryInputStreamiPv(arg0, 0x10, arg1);
}
