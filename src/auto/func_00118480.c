#include "m2c_prelude.h"
M2C_UNK func_001177A8();                            /* extern */
extern M2C_UNK D_002CA5A0;

/** @sly2-annot addr=0x118480
 * Purpose: Constructor of a CBinaryInputStream subclass: base ctor then vptr = 0x2CA5A0
 * Info: proposed name CMemInputStream_ctor_118480 (T2); class CMemInputStream; TU bis
 * Notes: COLLISION: CMemInputStream_ctor already used (same name proposed for 0x1184B8); renamed to
 *   CMemInputStream_ctor_118480
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void *func_00118480(void *arg0) {
    func_001177A8();
    M2C_FIELD(arg0, M2C_UNK **, 0x30) = &D_002CA5A0;
    return arg0;
}
