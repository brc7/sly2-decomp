struct ALO;
struct ASEGA { char pad[0x18]; ALO *palo; char pad2[0x10]; ASEGA *pasegaNext; };
struct ASEG { char pad[0x50]; ASEGA *pasegaFirst; };
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
    for (p = paseg->pasegaFirst; p; p = p->pasegaNext)
        if (p->palo == palo) return p;
    return 0;
}
