typedef unsigned char byte;
class CQueueInput { public: virtual int CbRead(int cb, void *pv); };
class CQueueOutput { public: virtual int CbWrite(int cb, void *pv); };
class CQueueInputMemory : public CQueueInput {
public:
    byte *m_pb; int m_cb; int m_ib; int m_cbMax;
    CQueueInputMemory(byte *pb, int cb) { m_cbMax = cb; m_pb = pb; m_cb = cb; m_ib = 0; }
    int CbRead(int cb, void *pv);
};
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
    void Init(int cb, void *pv);
    void Reset();
    int CbFill(int cb, CQueueInput *pqi);
    int CbDrain(int cb, CQueueOutput *pqo);
    int CbFill(int cb, byte *pb);
    void FreeDrain(int cb);
};
/** @sly2-annot addr=0x13D7D0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bq-0x13d558-0x13da18
 * @end-sly2-annot */
int CByteQueue::CbFill(int cb, byte *pb)
{
    int cbRet;
    if (cb == 0) {
        cbRet = 0;
    } else {
        CQueueInputMemory qim(pb, cb);
        cbRet = CbFill(cb, &qim);
    }
    return cbRet;
}
