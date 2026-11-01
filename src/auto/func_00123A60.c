#include "m2c_prelude.h"
s32 func_00210A98();                                /* extern */

/** @sly2-annot addr=0x123A60
 * Purpose: S32 = table D_003E1C48[cur] (0x210A98) or -1
 * Info: proposed name RefOpCurrentEpisode (T1); TU bif
 * Notes: v3: superseded RefOpGetLoadFilterMappedA | splice builtin 98 load-filter-world-episode~ ->
 *   current-episode~ (0.65): Body returns Int(NEpisodeCur()) = g_anEpisodeOfLevel[IlevelCur()] or -1. Not
 *   used by retail scripts
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r1_116918+top50_round(splice
 *   builtin 98)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void *func_00123A60(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = func_00210A98();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
