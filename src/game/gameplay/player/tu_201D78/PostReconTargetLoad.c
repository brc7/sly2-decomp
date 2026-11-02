#include "m2c_prelude.h"
M2C_UNK SubscribeLoObject(s32, void *);                 /* extern */
M2C_UNK PostLoLoad();                            /* extern */

/** @sly2-annot addr=0x203638
 * Purpose: CID 0x2E PostLoad: PostLoLoad + register with task +0xEC
 * Info: proposed name PostReconTargetLoad (T1); class CID46 slot15; TU tu_201D78
 * Notes: names_v2 C proposed AddBrkTouchClass__FP3BRK3CID (0.638)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void PostReconTargetLoad(void *arg0) {
    s32 temp_a0;

    PostLoLoad();
    temp_a0 = M2C_FIELD(arg0, s32 *, 0xEC);
    if (temp_a0 != 0) {
        SubscribeLoObject(temp_a0, arg0);
    }
}
