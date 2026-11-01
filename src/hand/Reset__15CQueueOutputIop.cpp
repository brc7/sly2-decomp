class CQueueOutputIop {
public:
    int pad[6];
    int m_x18, m_x1c, m_x20, m_x24, m_x28, m_x2c;
    void Reset();
};
/** @sly2-annot addr=0x1B2A80
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void CQueueOutputIop::Reset()
{
    m_x24 = m_x18;
    m_x28 = 0;
    m_x20 = 0;
    m_x1c = 0;
    m_x2c = 0;
}
