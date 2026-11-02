#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_00119C30 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
s32 func_001AE318(void *, s32, s32);                /* extern */
void Decompress__18CBinaryInputStream(void *arg0);                     /* static */
extern M2C_UNK D_002BBAF0;
extern M2C_UNK D_002BBB10;
extern M2C_UNK D_002BBB20;
extern M2C_UNK D_002CA5A0;
extern M2C_UNK D_002CA5C0;
extern void *D_002DD2F8;
extern void *D_002DD2FC;

/** @sly2-annot addr=0x1184B8
 * Purpose: Deleting destructor: resets vptr, operator delete (0x1AE520) if flag&1
 * Info: proposed name CMemInputStream_ctor (T1); class CMemInputStream; TU bis
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CMemInputStream_ctor(s32 arg0, s32 arg1) {
    M2C_FIELD(arg0, M2C_UNK **, 0x30) = &D_002CA5A0;
    if (arg1 & 1) {
        __builtin_delete();
    }
}
