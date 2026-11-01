#include "m2c_prelude.h"
M2C_UNK SetLoParent_1A58D0(void *, s32, M2C_UNK);        /* extern */
s32 PsoFromLo(s32);                             /* extern */

/** @sly2-annot addr=0x159968
 * Purpose: Re-parents the emitter to the LO found via +0xE4 (0x1A58D0(p, 0x1A5BF0(oid), 0))
 * Info: TU emitter
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r3_1539A8
 * @end-sly2-annot */
void func_00159968(void *arg0) {
    SetLoParent_1A58D0(arg0, PsoFromLo(*M2C_FIELD(M2C_FIELD(arg0, void **, 0xE4), s32 **, 8)), 0);
}
