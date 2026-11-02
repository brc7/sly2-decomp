#include "vifs.h"
extern int D_003DCFA0;
extern QW *D_003DCFA8[];
extern "C" void BeginRpGlob(VIFS *pvifs);
/** @sly2-annot addr=0x16E298
 * Purpose: Pass 3 begin (kind 1 sky/background): BeginRpGlob + DMA CALL each per-frame texture upload chain
 *   D_003DCFA8[0..D_003DCFA0)
 * Info: proposed name BeginRpBackground (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
extern "C" void BeginRpBackground(VIFS *pvifs)
{
    int i;
    BeginRpGlob(pvifs);
    for (i = 0; i < D_003DCFA0; i++)
        pvifs->AddDmaCall(D_003DCFA8[i]);
}
