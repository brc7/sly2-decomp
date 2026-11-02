#include "m2c_prelude.h"
M2C_UNK func_0014FCA8();                            /* extern */
extern M2C_UNK D_002DB858;

/** @sly2-annot addr=0x221990
 * Purpose: base CPLCY ctor 0x14FCA8 then vptr = D_002DB858
 * Info: proposed name CPTN_ctor (T1); class CPTN; TU tn
 * Notes: agent qualifier: (ctor)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void *func_00221990(void *arg0) {
    func_0014FCA8();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002DB858;
    return arg0;
}
