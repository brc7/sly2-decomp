#include "m2c_prelude.h"
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, s32, void *); /* extern */
M2C_UNK func_00183F50();                            /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1842B0
 * Purpose: CID 0x24 vt15: PostCid35Load and resolve three oids (PloFindSwObject kind 4)
 * Info: proposed name PostCid36Load (T2); class CID36 slot15; TU grabspot
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001842B0(void *arg0) {
    func_00183F50();
    M2C_FIELD(arg0, s32 *, 0x188) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, M2C_FIELD(arg0, s32 *, 0x184), arg0);
    M2C_FIELD(arg0, s32 *, 0x190) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, M2C_FIELD(arg0, s32 *, 0x18C), arg0);
    M2C_FIELD(arg0, s32 *, 0x198) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, M2C_FIELD(arg0, s32 *, 0x194), arg0);
}
