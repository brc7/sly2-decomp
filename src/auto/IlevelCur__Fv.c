#include "m2c_prelude.h"
extern s32 D_003E1110;

/** @sly2-annot addr=0x210A88
 * Purpose: Return the current level index D_003E1110
 * Info: proposed name IlevelCur (T1); TU sw
 * Notes: top50_round (0.80): Body. Siblings PlevelFromIlevel/IlevelFromRec,
 *   NEpisodeCur=g_anEpisodeOfLevel[IlevelCur()], NWorldCur=g_anWorldOfLevel[IlevelCur()]. Splice builtin 97
 *   returns it and scripts switch on it (case (load-filter~) ...) with le
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r7_1F8DD8+top50_round
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
s32 IlevelCur__Fv(void) {
    return D_003E1110;
}
