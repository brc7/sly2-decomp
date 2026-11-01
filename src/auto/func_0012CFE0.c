#include "m2c_prelude.h"
s32 func_0012D058();                                /* extern */
M2C_UNK func_00230018(s32, s32, void *);            /* extern */

/** @sly2-annot addr=0x12CFE0
 * Purpose: 0x12D058 then link into ALO act list (0x230018)
 * Info: proposed name AttachActToAlo (T2); TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0012CFE0(void *arg0) {
    if (func_0012D058() == 0) {
        func_00230018(M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 8), arg0);
    }
}
