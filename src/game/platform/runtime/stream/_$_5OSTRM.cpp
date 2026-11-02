class OSTRM { public: int a; void Flush(); ~OSTRM(); };
/** @sly2-annot addr=0x20D618
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#stream-0x20d5d0-0x20d9d0
 * @end-sly2-annot */
OSTRM::~OSTRM() { Flush(); }
