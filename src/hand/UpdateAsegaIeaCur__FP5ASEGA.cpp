// Matched by Claude agent (2026-10-23). Both searches are real loops that re-read pasega->paseg / pasega->t (only the
// loads before the loop's first exit test get hoisted, which gives the target's paseg/cea copies). The `== 0` keeps
// fast-math from folding !(a <= b) into a < b.
// Types: include/sly2/aseg.h (ASEG::ceaFrame/aeaFrame at 0xA0/0xA4, ASEGA::tLocal/svt/iea at 0x1C/0x40/0x50).
#include "sly2/aseg.h"
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
