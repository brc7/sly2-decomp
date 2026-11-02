#include "vifs.h"
extern "C" void *func_002333B4(void *pv, int n, int cb);
extern "C" void func_00160918(VIFS *pvifs, void *pvStart, void *pvEnd);
extern char D_0025E680[];
extern char D_00262320[];
extern char D_00000000[];
/** @sly2-annot addr=0x16E1A8
 * Purpose: Pass 0 begin: upload main glob microprogram D_0025E680..D_00262320, zero the 11-qw batch
 *   descriptor at TOPS+0xA, MSCAL 0
 * Info: proposed name BeginRpGlob (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void func_0016E1A8(VIFS *pvifs)
{
    uint *pun;
    func_00160918(pvifs, D_0025E680, D_00262320);
    pvifs->AddDmaCnt();
    pvifs->UnpackHelper(UPK_V4_16, 11, 10, 0, &pun);
    func_002333B4(pun, 0, 0x58);
    pvifs->AddVifMscal(D_00000000);
    pvifs->EndDmaCnt();
}
