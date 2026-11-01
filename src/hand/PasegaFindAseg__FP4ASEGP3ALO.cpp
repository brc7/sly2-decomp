#include "sly2/aseg.h"
// Walks paseg->dlAsega (ASEG+0x50) through ASEGA::dleAseg (+0x2C) looking for the ASEGA bound to palo (ASEGA::paloRoot, +0x18).
struct DLI { void *m_pdl; void **m_ppv; int m_ibDle; DLI *m_pdliNext; };
/** @sly2-annot addr=0x1304A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
ASEGA *PasegaFindAseg(ASEG *paseg, ALO *palo)
{
    DLI dli;
    ASEGA *p;
    for (p = (ASEGA *)paseg->dlAsega.head; p; p = (ASEGA *)p->dleAseg.next)
        if (p->paloRoot == palo) return p;
    return 0;
}
