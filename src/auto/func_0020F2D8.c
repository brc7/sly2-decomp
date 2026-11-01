#include "m2c_prelude.h"
M2C_UNK func_0020F1C8();                            /* extern */

/** @sly2-annot addr=0x20F2D8
 * Purpose: Store +0x3E24 and UpdateSwIllum
 * Info: proposed name SetSwIllumShadow (T1); TU sw
 * Notes: names_v2 v1 proposed FAddAshAseg__FP3ASHP4ASEG (0.009)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F2D8(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x3E24) = fparg0;
    func_0020F1C8();
}
