#include "m2c_prelude.h"
M2C_UNK ReleaseAct(void *);                      /* extern */

/** @sly2-annot addr=0x1CD2C0
 * Purpose: Free the three hand acts if active
 * Info: proposed name ReleaseJtHandsActs (T2); TU jthands
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CD2C0(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0xA0) != 0) {
        ReleaseAct(arg0 + 0x94);
        ReleaseAct(arg0 + 0x98);
        ReleaseAct(arg0 + 0x9C);
        M2C_FIELD(arg0, s32 *, 0xA0) = 0;
    }
}
