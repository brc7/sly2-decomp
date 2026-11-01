#include "dmas.h"
/** @sly2-annot addr=0x160918
 * Purpose: Appends a DMA REF chain that uploads the VU1 microprogram [pvStart,pvEnd)
 * Info: proposed name EnsureVu1Code (T1); TU frm
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#frm-0x1607a0-0x1613a8
 * @end-sly2-annot */
extern "C" void func_00160918(DMAS *pdmas, QW *pqwStart, QW *pqwEnd)
{
    pdmas->AddDmaRefs((uint)((char *)pqwEnd - (char *)pqwStart + 15) >> 4, pqwStart);
}
