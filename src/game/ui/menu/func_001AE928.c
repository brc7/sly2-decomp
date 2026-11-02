#include "m2c_prelude.h"
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1AE928
 * Purpose: Find menu widget object (oid 0x370) into +0x20; state 0, selection -1
 * Info: proposed name InitPauseMenu (T2); TU menu
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/hud-ui.md#4-source-files, docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_001AE928(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x20) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x370, 0);
    M2C_FIELD(arg0, s32 *, 0x1EC) = -1;
    M2C_FIELD(arg0, s32 *, 0x1E8) = 0;
}
