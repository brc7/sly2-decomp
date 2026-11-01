extern "C" {
void func_001AE5A0(void *d, const void *s, unsigned int n) __asm__("CopyAb__FPvT0Ui");
}
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
/** @sly2-annot addr=0x20D660
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#stream-0x20d5d0-0x20d9d0
 * @end-sly2-annot */
int OSTRM::CbWrite(void *pv, int cb)
{
    int cbWritten = 0;
    if (m_pbMem && m_cbMemMax) {
        int cbAvail;
        if (cb == 0) {
            cbWritten = 0;
            goto fail;
        }
        cbAvail = m_cbMemMax - m_cbMem;
        if (cbAvail < cb) cb = cbAvail;
    }
    if (cb == 0) {
fail:
        return 0;
    }
    if (m_pbMem) {
        func_001AE5A0(m_pbMem + m_cbMem, pv, cb);
        cbWritten = cb;
        m_cbMem += cb;
    } else {
        while (cbWritten < cb) {
            int cbFree = 0x400 - m_cbBuf;
            int cbRem = cb - cbWritten;
            int cbCopy = cbFree < cbRem ? cbFree : cbRem;
            if (cbCopy) {
                func_001AE5A0(m_ab + m_cbBuf, (char *)pv + cbWritten, cbCopy);
                cbWritten += cbCopy;
                m_cbBuf += cbCopy;
            }
            if (m_cbBuf == 0x400)
                Flush();
        }
    }
    m_cbTotal += cbWritten;
    return cb;
}
