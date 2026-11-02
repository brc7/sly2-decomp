typedef unsigned int uint;
class CBinaryInputStream {
public:
    int FOpenSector(uint isector, uint cb);
    char m_pad0[0x10];
    unsigned char *m_pbRaw;      // 0x10
    char m_pad1[0x20];
    int m_cbSpool;               // 0x34
    unsigned char *m_abSpool;    // 0x38
    int m_pad3c;
    uint m_tickWaitLo;           // 0x40
    uint m_tickWaitHi;           // 0x44
    uint m_cbFile;               // 0x48
    int m_cbAsyncComplete;       // 0x4C
    uint m_cbAsyncRequest;       // 0x50
    int m_cbAsyncRemaining;      // 0x54
    int m_cbPartialRead;         // 0x58
    int m_isector;               // 0x5C
    int m_cbuf;                  // 0x60
};
/** @sly2-annot addr=0x184D98
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd,
 *   docs/formats/disc-records.md#2-the-read-path,
 *   docs/reference/functions/core-library.md#tu_1848a8-0x1848a8-0x184f50
 * @end-sly2-annot */
int CBinaryInputStream::FOpenSector(uint isector, uint cb)
{
    m_cbAsyncRequest = isector;
    m_cbFile = cb;
    m_pbRaw = m_abSpool;
    m_tickWaitLo = 0;
    m_cbAsyncComplete = 0;
    m_tickWaitHi = 0;

    int cbSpool = m_cbSpool;
    if (cbSpool < 0)
        cbSpool += 0xfffff;

    m_cbPartialRead = 0;

    int cbAsyncRemaining = 2;
    int shr = 20;
    if ((cbSpool >> shr) > cbAsyncRemaining)
        cbAsyncRemaining = cbSpool >> shr;

    m_isector = 0;
    m_cbAsyncRemaining = cbAsyncRemaining;
    m_cbuf = cbAsyncRemaining - 1;
    return 1;
}
