extern "C" {
void *CopyAb__FPvT0Ui(void *d, const void *s, int n);
int func_00236960(const char *fmt, ...);
int sceWrite(int fd, const void *pv, int cb);
}
extern char D_002A8910[];
class OSTRM {
public:
    char *m_pbMem;
    int m_cbMemMax;
    int m_cbMem;
    int m_fd;
    int m_cbTotal;
    int m_cbBuf;
    char m_ab[0x400];
    void Flush();
    int CbWrite(void *pv, int cb);
};
/** @sly2-annot addr=0x20D798
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#stream-0x20d5d0-0x20d9d0
 * @end-sly2-annot */
void OSTRM::Flush()
{
    if (m_cbBuf == 0 || m_pbMem)
        return;
    if (m_fd < 0) {
        char *pb = m_ab;
        int ib;
        for (ib = 0; ib < m_cbBuf; ) {
            char ach[0x88];
            int cbRem = m_cbBuf - ib;
            int cb = 0x80;
            if (cbRem < 0x80)
                cb = cbRem;
            CopyAb__FPvT0Ui(ach, pb, cb);
            ib += cb;
            pb += cb;
            ach[cb] = 0;
            func_00236960(D_002A8910, ach);
        }
    } else {
        sceWrite(m_fd, m_ab, m_cbBuf);
    }
    m_cbBuf = 0;
}
