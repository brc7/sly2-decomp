#include "heap.h"

/** @sly2-annot addr=0x116918
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name PvAllocClear__FP4HEAPii (EXISTING); TU heap
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/engine/core-library.md#core-library-memory-containers-math-streams-cd-threads-text,
 *   docs/engine/core-library.md#1-memory, docs/engine/core-library.md#6-source-files
 * @end-sly2-annot */
void *PvAllocClear(HEAP *pheap, int cb, int n)
{
    void *pv = pheap->PvAlloc(cb, n);
    memset(pv, 0, cb);
    return pv;
}

/** @sly2-annot addr=0x116970
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#heap-0x116918-0x116ca0
 * @end-sly2-annot */
void *PvAllocCopy(HEAP *pheap, int cb, void *pv, int n)
{
    void *pvNew = pheap->PvAlloc(cb, n);
    CopyAb(pvNew, pv, cb);
    return pvNew;
}

/** @sly2-annot addr=0x1169D8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#heap-0x116918-0x116ca0
 * @end-sly2-annot */
char *PchzAllocCopy(HEAP *pheap, char *pchz, int n)
{
    return (char *)PvAllocCopy(pheap, strlen(pchz) + 1, pchz, n);
}

/** @sly2-annot addr=0x116A98
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name __11CLinearHeapiPUc (EXISTING); TU heap
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/core-library.md#heap-0x116918-0x116ca0
 * @end-sly2-annot */
CLinearHeap::CLinearHeap(int cbMax, byte *pbBase) : m_cbMax(cbMax), m_pbBase(pbBase)
{
    m_ib = 0;
}

/** @sly2-annot addr=0x116AB8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#heap-0x116918-0x116ca0
 * @end-sly2-annot */
void *CLinearHeap::PvAlloc(int cb, int n)
{
    int ib = m_ib;
    int ibNew = ib + ((cb + 15) & ~15);
    if (m_cbMax < ibNew)
        return NULL;
    m_ib = ibNew;
    return m_pbBase + ib;
}

/** @sly2-annot addr=0x116AF8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#heap-0x116918-0x116ca0
 * @end-sly2-annot */
void *CLinearHeap::PvRealloc(void *pv, int cb, int n)
{
    return PvAlloc(cb, n);
}

/** @sly2-annot addr=0x116D28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name Free__9CDynArray (EXISTING); TU dynarray
 * Notes: names_v2 v1 hint not used: __18CBinaryInputStreamPvii
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: static naming pass; pre-existing symbol
 *   in symbol_addrs.txt
 * See: docs/reference/functions/core-library.md#dynarray-0x116ca0-0x117030
 * @end-sly2-annot */
void CDynArray::Free()
{
    m_pheap->Free(m_pv);
    m_pv = NULL;
    m_cMax = 0;
    m_c = 0;
}

/** @sly2-annot addr=0x116D70
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#dynarray-0x116ca0-0x117030
 * @end-sly2-annot */
void CDynArray::Reserve(int c)
{
    if (m_cMax < c)
    {
        int cMax;
        if (c < 0x200)
        {
            cMax = m_cMax ? m_cMax : 8;
            while (cMax < c)
                cMax *= 2;
        }
        else
        {
            cMax = (c + 0x3FF) & ~0x3FF;
        }
        m_pv = m_pheap->PvRealloc(m_pv, cMax * m_cb, 4);
        m_cMax = cMax;
    }
}
