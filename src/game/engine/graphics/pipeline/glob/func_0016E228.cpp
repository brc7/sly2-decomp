#include "vifs.h"
extern "C" void *func_002333B4(void *pv, int n, int cb);
extern char D_00003AB0[];
/** @sly2-annot addr=0x16E228
 * Purpose: Pass 0 end (shared by passes 8-10): zero batch descriptor, MSCAL 0x3AB0 (flush entry), FLUSHA
 * Info: proposed name EndRpGlob (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void func_0016E228(VIFS *pvifs)
{
    uint *pun;
    pvifs->AddDmaCnt();
    pvifs->UnpackHelper(UPK_V4_16, 11, 10, 0, &pun);
    func_002333B4(pun, 0, 0x58);
    pvifs->AddVifMscal(D_00003AB0);
    pvifs->AddVifFlusha();
    pvifs->EndDmaCnt();
}
