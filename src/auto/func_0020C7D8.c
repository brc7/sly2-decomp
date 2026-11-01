#include "m2c_prelude.h"
/** @sly2-annot addr=0x20C7D8
 * Purpose: fInit (+0xC0) = 0
 * Info: proposed name InvalidateDirAnims (T2); TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020C7D8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xC0) = 0;
}
