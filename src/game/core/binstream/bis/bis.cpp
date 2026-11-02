#include "bis.h"

/** @sly2-annot addr=0x1177A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name __18CBinaryInputStream (EXISTING); TU bis
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd, docs/engine/core-library.md#6-source-files,
 *   docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
CBinaryInputStream::CBinaryInputStream() : m_grfbis(0)
{
    m_pbRaw = NULL;
    m_pb = NULL;
    m_cbRaw = 0;
    m_cb = 0;
    m_grfDecomp = 0;
    m_cbPending = 0;
    m_cbConsumed = 0;
    m_unk20 = 0;
    m_pbDecomp = NULL;
    m_unk2C = 0;
    m_unk28 = 0;
}

/** @sly2-annot addr=0x1177F0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::Close()
{
    m_pbRaw = NULL;
    m_pb = NULL;
    m_cbRaw = 0;
    m_cb = 0;
    m_grfDecomp = 0;
    m_cbPending = 0;
}

/** @sly2-annot addr=0x117810
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name Refill__18CBinaryInputStream (EXISTING); TU bis
 * Notes: names_v2 v1 hint not used: Pump__18CBinaryInputStream
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: static naming pass; pre-existing symbol
 *   in symbol_addrs.txt
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::Refill()
{
    if (m_cb != 0)
        return;

    if (m_grfbis & 2)
    {
        Decompress();
    }
    else
    {
        Fill();
        m_cb = m_cbRaw;
        m_pb = m_pbRaw;
        m_pbRaw += m_cbRaw;
        m_cbRaw = 0;
    }
    m_cbConsumed += m_cb;
}

/** @sly2-annot addr=0x117898
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
int CBinaryInputStream::CbRead(int cb, void *pv)
{
    int cbLeft = cb;
    byte *pb = (byte *)pv;

    if (m_cb < 0)
        return 0;

    while (cbLeft > 0)
    {
        Refill();
        if (m_cb == 0)
        {
            m_cb = -1;
            break;
        }

        int cbCopy = (cbLeft < m_cb) ? cbLeft : m_cb;

        if (pb)
        {
            CopyAb(pb, m_pb, cbCopy);
            pb += cbCopy;
        }

        m_pb += cbCopy;
        m_cb -= cbCopy;
        cbLeft -= cbCopy;
    }

    return cb - cbLeft;
}

/** @sly2-annot addr=0x117968
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container,
 *   docs/reference/functions/core-library.md#bis-0x1177a8-0x118668,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void CBinaryInputStream::Read(int cb, void *pv)
{
    CbRead(cb, pv);
}

/** @sly2-annot addr=0x117988
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name Align__18CBinaryInputStreami (EXISTING); TU bis
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: name high [EXISTING 0.45]; purpose not reviewed - evidence: pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/formats/world-file.md#3-head-layout-loadswfrombrx-func_0013da18
 * @end-sly2-annot */
void CBinaryInputStream::Align(int n)
{
    int pbAligned = ((int)m_pb + (n - 1)) & -n;
    m_cb -= (pbAligned - (int)m_pb);
    m_pb = (byte *)pbAligned;
}

/** @sly2-annot addr=0x117BD0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
uchar CBinaryInputStream::U8Read()
{
    if (m_cb >= 1)
    {
        uchar b = *m_pb;
        m_pb++;
        m_cb--;
        return b;
    }

    uchar b;
    Read(sizeof(uchar), &b);
    return b;
}

/** @sly2-annot addr=0x117C20
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
ushort CBinaryInputStream::U16Read()
{
    if (m_cb >= 2)
    {
        ushort u = m_pb[0];
        ushort v = m_pb[1];
        m_pb += 2;
        m_cb -= 2;
        return (v << 8) | u;
    }

    ushort v;
    Read(sizeof(ushort), &v);
    return v;
}

/** @sly2-annot addr=0x117C80
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
uint CBinaryInputStream::U32Read()
{
    if (m_cb >= 4)
    {
        uint v = m_pb[0] + (m_pb[1] << 8) + (m_pb[2] << 16) + (m_pb[3] << 24);
        m_pb += 4;
        m_cb -= 4;
        return v;
    }

    uint v;
    Read(sizeof(uint), &v);
    return v;
}

/** @sly2-annot addr=0x117D00
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
char CBinaryInputStream::S8Read()
{
    if (m_cb >= 1)
    {
        char b = *m_pb;
        m_pb += 1;
        m_cb -= 1;
        return b;
    }

    char b;
    Read(sizeof(char), &b);
    return b;
}

/** @sly2-annot addr=0x117D50
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
short CBinaryInputStream::S16Read()
{
    if (m_cb >= 2)
    {
        short v = m_pb[1] << 8;
        v |= m_pb[0];
        m_pb += 2;
        m_cb -= 2;
        return v;
    }

    short v;
    Read(sizeof(short), &v);
    return v;
}

/** @sly2-annot addr=0x117DB8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
int CBinaryInputStream::S32Read()
{
    if (m_cb >= 4)
    {
        int v = m_pb[0] + (m_pb[1] << 8) + (m_pb[2] << 16) + (((char *)m_pb)[3] << 24);
        m_pb += 4;
        m_cb -= 4;
        return v;
    }

    int v;
    Read(sizeof(int), &v);
    return v;
}

/** @sly2-annot addr=0x117E38
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
float CBinaryInputStream::F32Read()
{
    if (m_cb >= 4)
    {
        uint u = m_pb[0] + (m_pb[1] << 8) + (m_pb[2] << 16) + (m_pb[3] << 24);
        m_pb += 4;
        m_cb -= 4;
        return *(float *)&u;
    }
    float f;
    Read(sizeof(float), &f);
    return f;
}

/** @sly2-annot addr=0x1179B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::Skip(int cb)
{
    Read(cb, NULL);
}

/** @sly2-annot addr=0x1179D8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd,
 *   docs/engine/overview.md#3-world--level-lifecycle,
 *   docs/formats/compression-lzss.md#compression-whole-file-lzss
 * @end-sly2-annot */
void CBinaryInputStream::SetDecompress(int n, byte *pb)
{
    m_grfbis |= 2;
    m_unk20 = n;
    m_pbDecomp = pb;
}

/* LZSS: 8K window, flag byte per 8 tokens (bit set = literal), references are
 * 13-bit offset + 3-bit length (copies length + 3 bytes). */
/** @sly2-annot addr=0x1179F0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd,
 *   docs/formats/compression-lzss.md#compression-whole-file-lzss, docs/formats/world-file.md#1-container
 * @end-sly2-annot */
void CBinaryInputStream::Decompress()
{
    int cbOut = 0;
    byte *pbOut = m_pbDecomp;
    int cbIn = m_cbRaw;
    byte *pbIn = m_pbRaw;
    int grf = m_grfDecomp;

    if (m_cbPending > 0)
    {
        memcpy(pbOut, pbOut + 0x2000, m_cbPending);
        cbOut = m_cbPending;
    }

    int grfMark = 0xFF00;   /* loop-hoisted constant in the original */
    while (cbOut < 0x2000)
    {
        grf >>= 1;
        if (!(grf & 0x100))
        {
            if (--cbIn < 0)
            {
                Fill();
                if (m_cbRaw <= 0)
                    break;
                pbIn = m_pbRaw;
                cbIn = m_cbRaw - 1;
            }
            grf = *pbIn++ + grfMark;
        }

        if (--cbIn < 0)
        {
            Fill();
            if (m_cbRaw <= 0)
                break;
            pbIn = m_pbRaw;
            cbIn = m_cbRaw - 1;
        }
        int b = *pbIn++;

        if (grf & 1)
        {
            pbOut[cbOut++] = b;
        }
        else
        {
            if (--cbIn < 0)
            {
                Fill();
                pbIn = m_pbRaw;
                cbIn = m_cbRaw - 1;
            }
            int v = b + (*pbIn++ << 8);
            int cb = (v >> 13) + 2;
            int ib = v & 0x1FFF;
            for (; cb >= 0; cb--)
            {
                pbOut[cbOut++] = pbOut[ib++ & 0x1FFF];
            }
        }
    }

    m_pb = pbOut;
    if (cbOut >= 0x2000)
    {
        m_cb = 0x2000;
        m_cbPending = cbOut - 0x2000;
    }
    else
    {
        m_cb = cbOut;
        m_cbPending = 0;
    }
    m_pbRaw = pbIn;
    m_cbRaw = cbIn;
    m_grfDecomp = grf;
}

/** @sly2-annot addr=0x117EB0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::ReadVector(VECTOR *pvec)
{
    Read(sizeof(float) * 3, pvec);
}

/** @sly2-annot addr=0x117ED0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::ReadMatrix(MATRIX3 *pmat)
{
    ReadVector((VECTOR *)pmat->mat[0]);
    ReadVector((VECTOR *)pmat->mat[1]);
    ReadVector((VECTOR *)pmat->mat[2]);
}

/** @sly2-annot addr=0x117F18
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/level-objects.md#world-file-part-2-world-entries-and-object-records-all-classes,
 *   docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::ReadMatrix4(MATRIX4 *pmat)
{
    ReadMatrix((MATRIX3 *)pmat);
    ReadVector((VECTOR *)pmat->mat[3]);
    pmat->mat[0][3] = 0.0f;
    pmat->mat[1][3] = 0.0f;
    pmat->mat[2][3] = 0.0f;
    pmat->mat[3][3] = 1.0f;
}

/** @sly2-annot addr=0x117F70
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/world-file.md#1-container, docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::ReadVector4(VECTOR4 *pvec)
{
    Read(sizeof(VECTOR4), pvec);
}

/** @sly2-annot addr=0x117F90
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::ReadCb12(void *pv)
{
    Read(12, pv);
}

/** @sly2-annot addr=0x117FB0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void CBinaryInputStream::ReadCb8(void *pv)
{
    Read(8, pv);
}
