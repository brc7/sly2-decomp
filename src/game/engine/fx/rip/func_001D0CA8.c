#include "m2c_prelude.h"
M2C_UNK InitRip();                            /* extern */
extern s128 D_002C1F40;

/** @sly2-annot addr=0x1D0CA8
 * Purpose: RIPT 1 Init: InitRip + params
 * Info: proposed name InitRipt1 (T2); TU rip
 * Notes: names_v2 v1 proposed InitRay__FP3RAYP6VECTORfP2SO (0.638)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D0CA8(void *arg0) {
    InitRip();
    M2C_FIELD(arg0, f32 *, 0x18) = 5.0f;
    M2C_FIELD(arg0, s128 *, 0xA0) = (s128) D_002C1F40;
}
