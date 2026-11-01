typedef unsigned char byte;
typedef unsigned int uint;
enum BASK { BASK_Nil = -1, BASK_Host = 0, BASK_Cd = 1 };
class CBinaryAsyncStream {
public:
    BASK m_bask;        /* 0x00 */
    int m_fd;           /* 0x04 */
    uint m_isector;     /* 0x08 */
    int m_cbSpooling;   /* 0x0c */
    byte *m_pbSpooling; /* 0x10 */
    byte *m_abSpool;    /* 0x14 */
    int m_unk18;        /* 0x18 (new in Sly 2) */
    int m_cb;           /* 0x1c */
    byte *m_pb;         /* 0x20 */
    int m_ibCur;        /* 0x24 */
    int m_cbUnspooled;  /* 0x28 */
    int m_cbFile;       /* 0x2c */
    bool FSpooling();
    bool FSpoolingComplete();
    void FinishSpooling();
};
extern "C" int sceIoctl(int fd, int req, void *pv);
extern "C" int sceCdSync(int mode);
extern "C" int sceCdGetError(void);
/** @sly2-annot addr=0x1327E0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bas-0x132570-0x132b00
 * @end-sly2-annot */
bool CBinaryAsyncStream::FSpoolingComplete()
{
    int status = 0;
    bool fComplete = false;

    if (m_cb == m_ibCur && FSpooling())
    {
        switch (m_bask)
        {
            case BASK_Host:
            {
                int fExecuting;
                status = sceIoctl(m_fd, 1, &fExecuting);
                fComplete = (fExecuting == 0);
                break;
            }
            case BASK_Cd:
            {
                fComplete = (sceCdSync(1) == 0);
                if (fComplete)
                {
                    status = sceCdGetError();
                }
                break;
            }
        }
    }

    if (status)
    {
        m_cbSpooling = 0;
        m_pbSpooling = 0;
        fComplete = false;
    }

    return fComplete;
}
