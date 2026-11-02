#include "m2c_prelude.h"
M2C_UNK func_002301D0(s32, s32);                    /* extern */

/** @sly2-annot addr=0x131A40
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name SetAsegaPriority__FP5ASEGAi (EXISTING); TU asega
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void SetAsegaPriority__FP5ASEGAi(void *arg0, s32 arg1) {
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x34);
    if (var_s0 != NULL) {
        do {
            M2C_FIELD(var_s0, s32 *, 0xC) = arg1;
            func_002301D0(M2C_FIELD(var_s0, s32 *, 4), M2C_FIELD(var_s0, s32 *, 8));
            var_s0 = M2C_FIELD(var_s0, void **, 0x28);
        } while (var_s0 != NULL);
    }
}
