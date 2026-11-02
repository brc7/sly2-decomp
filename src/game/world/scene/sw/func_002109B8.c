#include "m2c_prelude.h"
extern s32 D_003E1C4C;

/** @sly2-annot addr=0x2109B8
 * Purpose: g_alevellist + ilevel*8
 * Info: proposed name PlevelFromIlevel (T2); TU sw
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
s32 func_002109B8(s32 arg0) {
    return D_003E1C4C + (arg0 * 8);
}
