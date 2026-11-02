#include "m2c_prelude.h"
M2C_UNK InitRip();                            /* extern */
extern s128 D_002C1F90;
extern s128 D_002C1FA0;

/** @sly2-annot addr=0x1D0108
 * Purpose: RIPT 4 Init: InitRip + extra fields
 * Info: proposed name InitGlint (T2); TU rip
 * Notes: names_v2 v1 proposed InitDebris__FP6DEBRISP6VECTORfP2SO (0.001)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D0108(void *arg0) {
    InitRip();
    M2C_FIELD(arg0, f32 *, 0x18) = 0.5f;
    M2C_FIELD(arg0, s128 *, 0x30) = (s128) D_002C1F90;
    M2C_FIELD(arg0, s128 *, 0x20) = (s128) D_002C1FA0;
}
