class CBinaryAsyncStream {
public:
    int m_bask; int m_fd; unsigned int m_isector; int m_cbSpooling;
    ~CBinaryAsyncStream();
    void Close();
};
/** @sly2-annot addr=0x1325B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bas-0x132570-0x132b00
 * @end-sly2-annot */
CBinaryAsyncStream::~CBinaryAsyncStream() { Close(); }
