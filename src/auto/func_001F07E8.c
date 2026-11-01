#include "m2c_prelude.h"
M2C_UNK OidFromSmIsms(s32, s32);                    /* extern */

/** @sly2-annot addr=0x1F07E8
 * Purpose: Same for ismsCur (+0x34)
 * Info: proposed name GetSmaCur (T1); TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void func_001F07E8(void *arg0) {
    OidFromSmIsms(M2C_FIELD(arg0, s32 *, 0x24), M2C_FIELD(arg0, s32 *, 0x34));
}
