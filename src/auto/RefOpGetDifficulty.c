#include "m2c_prelude.h"
f32 GGetDifficulty(M2C_UNK *);                       /* extern */
extern M2C_UNK D_002DE578;

/** @sly2-annot addr=0x123B60
 * Purpose: F32 = 0x154C90(&D_002DE578): D_003E0F9C->pg value or average of 3 Splice values
 * Info: proposed name RefOpGetDifficulty (T1); TU bif
 * Notes: names_v2 C proposed RefOpCurrentTime__FiP4CRefP6CFrame (0.638) | v3: superseded RefOpGetFloat103 |
 *   splice builtin 103 get-float-103~ -> difficulty~ (0.70): Body Float(GGetDifficulty(&D_002DE578)). Scripts
 *   scale numbers with it: (+ 10 (* (get-float-103~) 5)), (clq-evaluate #clq(1.0 -0.3 0.0) (get-float-103~))
 *   - dyna
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; static naming pass; sources:
 *   r1_116918+top50_round(splice builtin 103)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void *RefOpGetDifficulty(void *arg0) {
    M2C_FIELD(arg0, f32 *, 4) = GGetDifficulty(&D_002DE578);
    M2C_FIELD(arg0, s32 *, 0) = 0x16;
    return arg0;
}
