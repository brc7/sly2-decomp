#include "m2c_prelude.h"
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, s32, M2C_UNK); /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DA770
 * Purpose: slot 12: resolve widget oid +0x20 to object +0x24 (PloFindSwObject kind 5)
 * Info: proposed name PostScreenLoad (T2); class SCREEN; TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001DA770(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x24) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, M2C_FIELD(arg0, s32 *, 0x20), 0);
}
