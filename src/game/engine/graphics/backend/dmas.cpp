// dmas.cpp -- DMA startup and DMAS, the DMA chain builder every renderer uses. A DMAS writes PS2 DMA source-chain tags
// (CNT, REF, CALL, RET, END) and their payload into a buffer it owns (from the frame slot heap, AllocSw, or a caller's
// static array, AllocStatic); Detach / DetachCopySw close the chain and hand the buffer (or a heap copy) to the
// caller. VIFS and GIFS (vifs.cpp, gifs.cpp) derive from it. Entry points: StartupDma (boot: resets DMA and caches the
// channel handles), DMAS::AddDmaCnt / AddDmaCall / AddDmaRefs / EndDmaCnt. TU 0x157200..0x1578F8 (20 functions,
// address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_dmas
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_dmas
#include "dmas.h"
#include "sly2/port.h"
#include "sly2/slotheap.h"
#include "sly2/scrap.h"

// 0x1577A0 StaticInit_dmas and GlobalConstructors_dmas: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* SDK DMA library (libdma); the channel handles are kept in five globals. */
struct sceDmaChan;
extern "C" {
sceDmaChan *sceDmaGetChan(int id);
int sceDmaReset(int mode);
int sceGsResetPath();
sceDmaChan *g_pdcVif0;   /* VIF0 (dmas' .bss, address order) */
sceDmaChan *g_pdcVif1;   /* VIF1 */
sceDmaChan *g_pdcGif;   /* GIF */
sceDmaChan *g_pdcFromSpr;   /* fromSPR */
sceDmaChan *g_pdcToSpr;   /* toSPR */
}

// Boot-time DMA setup: resets the DMA controller and the GS path, then caches the VIF0, VIF1, GIF, fromSPR and toSPR
// channel handles in the five globals above.
void StartupDma()
{
    sceDmaReset(1);
    sceGsResetPath();
    g_pdcVif0 = sceDmaGetChan(0);
    g_pdcVif1 = sceDmaGetChan(1);
    g_pdcGif = sceDmaGetChan(2);
    g_pdcFromSpr = sceDmaGetChan(8);
    g_pdcToSpr = sceDmaGetChan(9);
}

// Constructs an empty chain builder (no buffer, no padding, no open GIF primitive).
DMAS::DMAS()
{
    m_fPad = 0;
    m_fEndPrim = 0;
    Clear();
}

// Forgets the buffer: all pointers to null (the memory belongs to the slot heap or the caller).
void DMAS::Clear()
{
    m_pbMax = NULL;
    m_ab = NULL;
    m_pqwCnt = NULL;
    m_pb = NULL;
}

// Rewinds the write pointer to the start of the buffer and drops any open CNT tag, keeping the buffer.
void DMAS::Reset()
{
    m_pqwCnt = NULL;
    m_pb = m_ab;
}

// Gives the builder a fresh buffer of cqw quadwords from the frame slot heap (16-byte aligned).
void DMAS::AllocSw(int cqw)
{
    SWALLOC req;
    int cb = cqw << 4;
    req.cb = cb;
    req.cbAlign = 0x10;
    uchar *ab = (uchar *)AllocFromStack(&req);
    m_ab = ab;
    m_pb = ab;
    m_pbMax = ab + cb;
}

// Uses a caller-provided array of cqw quadwords as the buffer.
void DMAS::AllocStatic(int cqw, QW *aqw)
{
    m_ab = (uchar *)aqw;
    m_pb = (uchar *)aqw;
    m_pbMax = (uchar *)(aqw + cqw);
}

// Closes the open CNT tag, then returns the chain's length (quadwords) and start, and detaches the buffer from the
// builder.
void DMAS::Detach(int *pcqw, QW **paqw)
{
    EndDmaCnt();
    Flush();
    if (pcqw)
    {
        *pcqw = (uint)(m_pb - m_ab) >> 4;
    }
    if (paqw)
    {
        *paqw = (QW *)m_ab;
    }
    Clear();
}

// DetachCopySw into the default heap (g_heapSw).
void DMAS::DetachCopySw(int *pcqw, QW **paqw, QW **paqwCopy)
{
    DetachCopySw(&g_heapSw, pcqw, paqw, paqwCopy);
}

// Closes the open CNT tag and detaches the buffer like Detach, also returning a copy of the chain allocated from the
// given heap (for chains that must outlive the frame's slot heap).
void DMAS::DetachCopySw(HEAP *pheap, int *pcqw, QW **paqw, QW **paqwCopy)
{
    EndDmaCnt();
    Flush();
    uint cqw = (uint)(m_pb - m_ab) >> 4;
    if (pcqw)
    {
        *pcqw = cqw;
    }
    if (paqw)
    {
        *paqw = (QW *)m_ab;
    }
    if (paqwCopy)
    {
        *paqwCopy = (QW *)AllocCopyFromHeap(pheap, cqw << 4, m_ab, 4);
    }
    Clear();
}

// Hook for derived streams to push buffered data out before detaching; the base version does nothing.
void DMAS::Flush()
{
}

// 0x1574A8..0x1574B4: 2 dead-code scraps (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x30");
SLY2_SCRAP("addiu $sp, $sp, 0x10");

// Opens a new DMA CNT tag (ID 1: transfer the data that follows, then continue with the next tag) at the write
// pointer, closing the previous one; its quadword count is filled in by EndDmaCnt.
void DMAS::AddDmaCnt()
{
    EndDmaCnt();
    QW *pqw = (QW *)m_pb;
    m_pqwCnt = pqw;
    m_pb = (uchar *)(pqw + 1);
    m_pqwCnt->aul[0] = 0x10000000;
    m_pqwCnt->aul[1] = 0;
}

// Adds a DMA REF tag (ID 4) that transfers cqw quadwords from aqw in place. If a CNT was open it is closed first and a
// new one reopened after the tag, so callers can keep appending inline data.
void DMAS::AddDmaRefs(int cqw, QW *aqw)
{
    int fCnt = 0;
    if (m_pqwCnt)
    {
        EndDmaCnt();
        fCnt = 1;
    }

    QW *pqw = (QW *)m_pb;
    m_pb = (uchar *)(pqw + 1);
    pqw->aul[0] = cqw | 0x40000000 | ((ulong)aqw << 32);
    pqw->aul[1] = 0;

    if (fCnt)
    {
        QW *pqwCnt = (QW *)m_pb;
        m_pqwCnt = pqwCnt;
        m_pb = (uchar *)(pqwCnt + 1);
        pqwCnt->aul[0] = 0x10000000;
        m_pqwCnt->aul[1] = 0;
    }
}

// Adds a DMA CALL tag (ID 5) that runs the chain at aqw and returns here (on its RET), closing and reopening an open
// CNT around it like AddDmaRefs. Used to splice prebuilt packets (models, frame contexts) into the frame chain.
void DMAS::AddDmaCall(QW *aqw)
{
    int fCnt = 0;
    if (m_pqwCnt)
    {
        EndDmaCnt();
        fCnt = 1;
    }

    QW *pqw = (QW *)m_pb;
    m_pb = (uchar *)(pqw + 1);
    pqw->aul[0] = ((ulong)aqw << 32) | 0x50000000;
    pqw->aul[1] = 0;

    if (fCnt)
    {
        QW *pqwCnt = (QW *)m_pb;
        m_pqwCnt = pqwCnt;
        m_pb = (uchar *)(pqwCnt + 1);
        pqwCnt->aul[0] = 0x10000000;
        m_pqwCnt->aul[1] = 0;
    }
}

// Closes the open CNT and adds a DMA RET tag (ID 6): ends a called sub-chain.
void DMAS::AddDmaRet()
{
    EndDmaCnt();
    QW *pqw = (QW *)m_pb;
    m_pb = (uchar *)(pqw + 1);
    pqw->aul[0] = 0x60000000;
    pqw->aul[1] = 0;
}

// Copies cqw quadwords of raw data into the chain at the write pointer (no tag of its own).
void DMAS::AddDmaBulk(int cqw, QW *aqw)
{
    CopyAqw(m_pb, aqw, cqw);
    m_pb += cqw * sizeof(QW);
}

// Closes the open CNT and adds a DMA END tag (ID 7): the end of the whole chain.
void DMAS::AddDmaEnd()
{
    EndDmaCnt();
    QW *pqw = (QW *)m_pb;
    m_pb = (uchar *)(pqw + 1);
    pqw->aul[0] = 0x70000000;
    pqw->aul[1] = 0;
}

// Closes the open CNT tag: pads the payload to a quadword boundary with zero words (when the stream pads) or ends the
// open GIF primitive, then stores the payload's quadword count in the tag's QWC field.
void DMAS::EndDmaCnt()
{
    if (m_fPad)
    {
        uchar *pb = m_pb;
        while ((uptr)pb & 0xF)
        {
            *(int *)pb = 0;
            pb += 4;
        }
        m_pb = pb;
    }
    else if (m_fEndPrim)
    {
        EndPrim(0);
    }

    if (m_pqwCnt)
    {
        m_pqwCnt->aui[0] |= ((int)(m_pb - (uchar *)m_pqwCnt) >> 4) - 1;
        m_pqwCnt = NULL;
    }
}

// Hook for derived streams to finish an open primitive (GIFS overrides it); the base version does nothing.
void DMAS::EndPrim(int n)
{
}
