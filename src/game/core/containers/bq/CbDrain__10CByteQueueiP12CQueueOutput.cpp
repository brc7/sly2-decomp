typedef unsigned char byte;
class CQueueInput { public: virtual int CbRead(int cb, void *pv); };
class CQueueOutput { public: virtual int CbWrite(int cb, void *pv); virtual int FDrain(); };
class CByteQueue {
public:
    byte *m_ab;     /* 0x00 */
    int m_cb;       /* 0x04 */
    int m_ibFree;   /* 0x08 */
    int m_ibAlloc;  /* 0x0c */
    int m_cbFree;   /* 0x10 */
    int m_cbAlloc;  /* 0x14 */
    int m_cbDrain;  /* 0x18 */
    int m_cbTotal;  /* 0x1c */
    int CbFill(int cb, CQueueInput *pqi);
    int CbDrain(int cb, CQueueOutput *pqo);
};
static inline int Min(int a, int b) { return a < b ? a : b; }
/** @sly2-annot addr=0x13D6A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bq-0x13d558-0x13da18
 * @end-sly2-annot */
int CByteQueue::CbDrain(int cb, CQueueOutput *pqo)
{
    int cbDrain = 0;
    int cbLeft = cb;
    if (cbLeft <= 0 || cbLeft > m_cbAlloc)
        cbLeft = m_cbAlloc;
    while (cbLeft != 0) {
        int ib = m_ibAlloc;
        int cbT = pqo->CbWrite(Min(cbLeft, m_cb - ib), m_ab + ib);
        if (cbT == 0)
            break;
        m_ibAlloc += cbT;
        m_ibAlloc %= m_cb;
        m_cbAlloc -= cbT;
        if (pqo->FDrain())
            m_cbDrain += cbT;
        else
            m_cbFree += cbT;
        cbLeft -= cbT;
        cbDrain += cbT;
        if (cb <= 0)
            break;
    }
    return cbDrain;
}
