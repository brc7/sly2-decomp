#include "gifs.h"

/** @sly2-annot addr=0x166BD8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gifs-0x166920-0x167310
 * @end-sly2-annot */
void GIFS::AddPrimPack(int prim, int creg, ulong regs)
{
    EndPrim(0);
    ulong tag = ((ulong)creg << 60) | (1L << 46);
    register QW *pqw __asm__("$3") = (QW *)m_pb;
    m_pb = (uchar *)(pqw + 1);
    pqw->aul[0] = ((ulong)prim << 47) | tag;
    pqw->aul[1] = regs;
    m_creg = creg;
    m_pqwPrim = pqw;
    m_cregAll = 0;
}
