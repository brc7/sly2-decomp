#include "m2c_prelude.h"
M2C_UNK Incref__4CRef(void *);                      /* extern */
M2C_UNK Decref__4CRef(void *);                      /* extern */

/** @sly2-annot addr=0x129658
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name __as__4CRefRC4CRef (EXISTING); TU ref
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
void *__as__4CRefRC4CRef(void *arg0, void *arg1) {
    Incref__4CRef(arg1);
    Decref__4CRef(arg0);
    M2C_FIELD(arg0, s32 *, 0) = (s32) M2C_FIELD(arg1, s32 *, 0);
    M2C_FIELD(arg0, s32 *, 4) = (s32) M2C_FIELD(arg1, s32 *, 4);
    return arg0;
}
