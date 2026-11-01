#include "m2c_prelude.h"
s32 func_001C4840(M2C_UNK *);                       /* extern */
M2C_UNK func_001DFD58(s32, s32);                    /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DFE80
 * Purpose: FireRelay(prelay, PpoCur())
 * Info: proposed name FireRelayPlayer (T2); TU relay
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001DFE80(s32 arg0) {
    func_001DFD58(arg0, func_001C4840(&D_003DD490));
}
