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
/** @sly2-annot addr=0x13D5A0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bq-0x13d558-0x13da18
 * @end-sly2-annot */
int CByteQueue::CbFill(int cb, CQueueInput *pqi)
{
    int cbFill = 0;
    int cbLeft = cb;
    if (cbLeft <= 0 || cbLeft > m_cbFree)
        cbLeft = m_cbFree;
    while (cbLeft != 0) {
        int ib = m_ibFree;
        int cbT = pqi->CbRead(Min(cbLeft, m_cb - ib), m_ab + ib);
        if (cbT == 0)
            break;
        cbFill += cbT;
        cbLeft -= cbT;
        m_ibFree += cbT;
        m_ibFree %= m_cb;
        m_cbAlloc += cbT;
        m_cbFree -= cbT;
        m_cbTotal += cbT;
        if (cb <= 0)
            break;
    }
    return cbFill;
}
