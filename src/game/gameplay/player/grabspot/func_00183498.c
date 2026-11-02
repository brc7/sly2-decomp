#include "m2c_prelude.h"
/** @sly2-annot addr=0x183498
 * Purpose: Range [-pi r, pi r] (circumference of radius x10C): a round pole
 * Info: proposed name CID34_FGetGrabRange (T2); class CID34; TU grabspot
 * Notes: superseded: CID34_FGetWrapRange (r4b_rest) | GRABSPOT secondary slot 1 named FGetGrabRange
 *   everywhere (verifier r4b: all overrides return a [min,max] range)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r4b_rest+CONFLICTS#15
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
s32 func_00183498(void *arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0) = (f32) (M2C_FIELD(arg0, f32 *, 0x10C) * -3.1415927f);
    M2C_FIELD(arg1, f32 *, 4) = (f32) (M2C_FIELD(arg0, f32 *, 0x10C) * 3.1415927f);
    return 1;
}
