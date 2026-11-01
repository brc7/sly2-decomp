#include "vifs.h"
#include "bis.h"

/** @sly2-annot addr=0x226CA8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifBaseOffset(int iqwBase, int diqwOffset)
{
    AddDmaInt(this, iqwBase | 0x03000000);
    AddDmaInt(this, diqwOffset | 0x02000000);
}

/** @sly2-annot addr=0x226CE0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifMscal(void *pv)
{
    int i = (int)pv < 0 ? (int)pv + 7 : (int)pv;
    uchar *p = m_pb;
    m_pb = p + 4;
    *(int *)p = (i >> 3) | 0x14000000;
}

/** @sly2-annot addr=0x226D10
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifFlush()
{
    AddDmaInt(this, 0x11000000);
}

/** @sly2-annot addr=0x226D28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifFlusha()
{
    AddDmaInt(this, 0x13000000);
}

/** @sly2-annot addr=0x226D40
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifStcycl(int wl, int cl)
{
    AddDmaInt(this, cl | (wl << 8) | 0x1000000);
}

/** @sly2-annot addr=0x226C10
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/rendering.md#vifs-0x226c10-0x227350,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
VIFS::VIFS()
{
    m_fPad = 1;
    m_unMask = 0;
    m_fCheckCnt = 0;
}

/** @sly2-annot addr=0x226C58
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::Align(int iun)
{
    int i = ((uint)m_pb & 0xC) >> 2;
    uchar *pb = m_pb;
    while (i != iun)
    {
        *(int *)pb = 0;
        pb += 4;
        i = (i + 1) % 4;
    }
    m_pb = pb;
}

/** @sly2-annot addr=0x226D68
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifStrow(QW *pqw)
{
    { uchar *p = m_pb; m_pb = p + 4; *(int *)p = 0x30000000; }
    { uchar *p = m_pb; m_pb = p + 4; *(int *)p = pqw->aui[0]; }
    { uchar *p = m_pb; m_pb = p + 4; *(int *)p = pqw->aui[1]; }
    { uchar *p = m_pb; m_pb = p + 4; *(int *)p = pqw->aui[2]; }
    { uchar *p = m_pb; m_pb = p + 4; *(int *)p = pqw->aui[3]; }
}

/** @sly2-annot addr=0x226DD0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifStmask(uint unMask)
{
    if (unMask)
    {
        AddDmaInt(this, 0x20000000);
        AddDmaInt(this, unMask);
    }
    m_unMask = unMask;
}

extern int s_mpupkcb[];

/** @sly2-annot addr=0x226E10
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
int VIFS::CbUnpackSetup(UPK upk, int c, int iqw)
{
    int vifcode;
    if (upk >= 16)
    {
        vifcode = 0x60008000;
        upk = (UPK)(upk & 0xF);
    }
    else
    {
        vifcode = 0x6000C000;
    }
    vifcode = vifcode | (upk << 24) | (c << 16) | iqw;
    if (m_unMask)
    {
        vifcode |= 0x10000000;
    }
    AddDmaInt(this, vifcode);
    return s_mpupkcb[upk] * c;
}

/** @sly2-annot addr=0x226E88
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988,
 *   docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::UnpackHelper(UPK upk, int c, int iqw, int *pcb, uint **ppun)
{
    if (c == 0)
    {
        if (ppun)
            *ppun = NULL;
        if (pcb)
            *pcb = 0;
        return;
    }

    int cb = CbUnpackSetup(upk, c, iqw);
    if (ppun)
        *ppun = (uint *)m_pb;
    m_pb += ((cb + 3) / 4) * 4;
    if (pcb)
        *pcb = cb;
}

/** @sly2-annot addr=0x226F28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8,
 *   docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifUnpack(UPK upk, int c, void *pvSrc, int iqw)
{
    int cb;
    uint *pun;
    UnpackHelper(upk, c, iqw, &cb, &pun);

    uint *punSrc = (uint *)pvSrc;
    uint *punDst = pun;
    int cun = (cb + 3) >> 2;
    int cunOdd = cun & 3;

    for (int i = cunOdd; i != 0; i--)
    {
        *punDst++ = *punSrc++;
    }

    int cunQuad = cun - cunOdd;

    for (int i = 0; i < cunQuad; i += 4)
    {
        punDst[0] = punSrc[0];
        punDst[1] = punSrc[1];
        punDst[2] = punSrc[2];
        punDst[3] = punSrc[3];
        punSrc += 4;
        punDst += 4;
    }
}

/** @sly2-annot addr=0x226FD0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifUnpack(UPK upk, int c, CBinaryInputStream *pbis, int iqw)
{
    int cb;
    uint *pun;
    UnpackHelper(upk, c, iqw, &cb, &pun);
    pbis->Read(cb, pun);
}

/** @sly2-annot addr=0x227010
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifUnpackRefs(UPK upk, int c, void *pvSrc, int iqw, void ***pppv)
{
    if (c == 0)
        return;

    Align(3);
    int cb = CbUnpackSetup(upk, c, iqw);
    AddDmaRefs((cb + 15) / 16, (QW *)pvSrc);
    if (pppv)
        *pppv = (void **)((uchar *)m_pqwCnt - 12);
}

/** @sly2-annot addr=0x2270C8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifDirect(int cqw, QW *aqw, int fInt)
{
    Align(3);
    uchar *p = m_pb;
    m_pb = p + 4;
    *(int *)p = cqw | 0x50000000 | (fInt << 31);
    AddDmaBulk(cqw, aqw);
}

/** @sly2-annot addr=0x227148
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifDirectRefs(int cqw, QW *aqw, int fInt)
{
    Align(3);
    uchar *p = m_pb;
    m_pb = p + 4;
    *(int *)p = cqw | 0x50000000 | (fInt << 31);
    AddDmaRefs(cqw, aqw);
}

/** @sly2-annot addr=0x2271C8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifGifs(GIFS *pgifs)
{
    DMAS *pdmas = (DMAS *)pgifs;
    AddVifDirect((uint)(pdmas->m_pb - pdmas->m_ab) >> 4, (QW *)pdmas->m_ab, 0);
}
