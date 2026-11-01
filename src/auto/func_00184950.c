#include "m2c_prelude.h"
M2C_UNK __builtin_delete(void *);                   /* extern */
M2C_UNK func_00184CF0();                            /* extern */
extern M2C_UNK D_002D2918;

/** @sly2-annot addr=0x184950
 * Purpose: Destructor: reset vptr, Close (0x184CF0), delete if flag&1
 * Info: proposed name CStreamBis_ctor (T2); class CStreamBis; TU tu_1848A8
 * Notes: names_v2 B proposed _$_18CBinaryInputStream (0.889)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/core-library.md#tu_1848a8-0x1848a8-0x184f50
 * @end-sly2-annot */
void func_00184950(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, M2C_UNK **, 0x30) = &D_002D2918;
    func_00184CF0();
    if (arg1 & 1) {
        __builtin_delete(arg0);
    }
}
