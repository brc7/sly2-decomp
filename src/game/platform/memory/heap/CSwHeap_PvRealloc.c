#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of junk_0x0011d5d8 (initial). */

/* Warning: missing "jr $ra" in last block of func_00117640 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK Decompress__18CBinaryInputStream();                            /* extern */
extern M2C_UNK D_0027A778;
extern M2C_UNK D_0029A790;
extern M2C_UNK D_0029A7C0;
extern M2C_UNK D_0029A8C0;
extern M2C_UNK D_002A28D0;
extern M2C_UNK D_002A2900;
extern M2C_UNK D_002CA550;
extern M2C_UNK D_002CA5C0;
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

/** @sly2-annot addr=0x116A58
 * Purpose: vtable slot 2: ignores old pointer, re-dispatches to slot-1 PvAlloc with alignment = this->+4
 *   unless it is -1
 * Info: proposed name CSwHeap_PvRealloc (T1); class CSwHeap; TU heap
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#heap-0x116918-0x116ca0
 * @end-sly2-annot */
s32 CSwHeap_PvRealloc(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_a2;
    void *temp_t0;

    temp_t0 = M2C_FIELD(arg0, void **, 0);
    temp_a2 = M2C_FIELD(arg0, s32 *, 4);
    return M2C_FIELD(temp_t0, s32 (**)(s32, s32, s32), 0xC)(arg0 + M2C_FIELD(temp_t0, s16 *, 8), arg2, (~temp_a2 == 0) ? arg3 : temp_a2);
}
