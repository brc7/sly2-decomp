#include "dmas.h"
extern QW D_002DF260[];
void BlastAqwGifsBothFrames(QW *aqw);
/** @sly2-annot addr=0x170348
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void ClearFrameBuffers()
{
    DMAS dmas;
    QW aqw[2];
    dmas.AllocStatic(2, aqw);
    dmas.AddDmaRefs(0x25, D_002DF260);
    dmas.AddDmaEnd();
    dmas.Detach(0, 0);
    BlastAqwGifsBothFrames(aqw);
}
