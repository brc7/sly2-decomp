#include "m2c_prelude.h"
M2C_UNK func_0019F590(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1A16A8
 * Purpose: Callback thunk: ReturnJtToSafePlace(owner, NULL)
 * Info: proposed name ReturnJtToSafePlaceCb (T1); TU hurt
 * Notes: runtime3: JTHURT PMF callback pfn in S26 (swim)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A16A8(s32 *arg0) {
    func_0019F590(*arg0, 0);
}
