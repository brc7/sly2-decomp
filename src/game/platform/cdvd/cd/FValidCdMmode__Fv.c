#include "m2c_prelude.h"
s32 sceCdGetDiskType_242AB8();                                /* extern */
extern s32 D_002BD4C4;

/** @sly2-annot addr=0x14B9C0
 * Purpose: sceCdGetDiskType cached into global, returns ==0x14 (PS2DVD); identical to Sly1 decomp source
 * Info: proposed name FValidCdMmode (T1); TU cd
 * Confidence: high [T1 1.00] - evidence: static naming pass; sources: names_v2:A+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#cd-0x14b7f8-0x14bea8
 * @end-sly2-annot */
s32 FValidCdMmode__Fv(void) {
    s32 temp_v0;

    temp_v0 = sceCdGetDiskType_242AB8();
    if (D_002BD4C4 != temp_v0) {
        D_002BD4C4 = temp_v0;
    }
    return D_002BD4C4 == 0x14;
}
