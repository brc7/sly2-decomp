#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_00259D80 (initial). */

/* Warning: missing "jr $ra" in last block of func_00259DD8 (initial). */

extern M2C_UNK D_002DD2AC;
extern u32 D_002DD2D4;
extern M2C_UNK D_002DD598;
extern s32 D_003E0EA4;
extern M2C_UNK D_00452160;
extern M2C_UNK D_00452170;

/** @sly2-annot addr=0x259A88
 * Purpose: move frame to root list D_002DD2AC, set flag 0x8000 [game linkonce (inline/template)]
 * Info: proposed name GcFrame_MakeRoot (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/splice-scripting.md#splicemap-0x1ff7e8-0x1ffcf8
 * @end-sly2-annot */
s32 func_00259A88(s32 arg0) {
    s32 temp_v0;

    GcFrame_Unlink(arg0);
    GcFrame_PushFront(arg0, (s32) &D_002DD2AC);
    temp_v0 = M2C_FIELD(arg0, s32 *, 4) | 0x8000;
    M2C_FIELD(arg0, s32 *, 4) = temp_v0;
    return temp_v0;
}
