#include "m2c_prelude.h"
M2C_UNK ResolveZapReactions(void *, void *);              /* extern */
M2C_UNK ResolveAnimBinding(void *, void *);              /* extern */

/** @sly2-annot addr=0x2080F0
 * Purpose: Resolve all anim bindings (+0x4C0 count, +0x4D0 entries) and the 3 zap tables
 * Info: proposed name ResolveStepguardAnimBindings (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void ResolveStepguardAnimBindings(void *arg0) {
    s32 var_s2;
    void *var_s1;

    var_s2 = 0;
    if (M2C_FIELD(arg0, s32 *, 0x4C0) > 0) {
        var_s1 = arg0 + 0x4D0;
        do {
            ResolveAnimBinding(var_s1, arg0);
            var_s2 += 1;
            var_s1 += 0x20;
        } while (var_s2 < M2C_FIELD(arg0, s32 *, 0x4C0));
    }
    ResolveZapReactions(arg0, arg0 + 0xC10);
    ResolveZapReactions(arg0, arg0 + 0x8D0);
    ResolveZapReactions(arg0, arg0 + 0xA70);
}
