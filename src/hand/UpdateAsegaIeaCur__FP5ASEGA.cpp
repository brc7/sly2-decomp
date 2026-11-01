// Matched by Claude agent (2026-10-23). Both searches are real loops that re-read pasega->paseg / pasega->t (only the
// loads before the loop's first exit test get hoisted, which gives the target's paseg/cea copies). The `== 0` keeps
// fast-math from folding !(a <= b) into a < b.
// Types: include/sly2/aseg.h (ASEG::ceaFrame/aeaFrame at 0xA0/0xA4, ASEGA::tLocal/svt/iea at 0x1C/0x40/0x50).
#include "sly2/aseg.h"
/** @sly2-annot addr=0x130EB0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name UpdateAsegaIeaCur__FP5ASEGA (EXISTING); TU asega
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void UpdateAsegaIeaCur(ASEGA *pasega)
{
    ASEG *paseg = pasega->paseg;
    int cea = paseg->ceaFrame;
    int iea;
    if (cea == 0)
        return;
    if (pasega->svt >= 0.0f) {
        for (iea = 0; iea < pasega->paseg->ceaFrame; iea++)
            if (pasega->tLocal <= pasega->paseg->aeaFrame[iea].tLocal)
                break;
        pasega->iea = iea;
    } else {
        iea = cea;
        do
            iea--;
        while (iea >= 0 && (pasega->paseg->aeaFrame[iea].tLocal <= pasega->tLocal) == 0);
        pasega->iea = iea + 1;
    }
}
