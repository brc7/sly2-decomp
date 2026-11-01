#include "m2c_prelude.h"
M2C_UNK UpdateSwIllum();                            /* extern */

/** @sly2-annot addr=0x20F2F8
 * Purpose: Store midtone +0x3E20 (ctor default 90) and UpdateSwIllum
 * Info: proposed name SetSwIllum (T1); TU sw
 * Notes: names_v2 v1 proposed FAddAshOid__FP3ASH3OID (0.009)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8+verify_side
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void SetSwIllum(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x3E20) = fparg0;
    UpdateSwIllum();
}
