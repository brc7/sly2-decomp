class CProg { public: int m_cRetry; int m_nRemain; int m_nTarget; int m_nMax; char m_fActive; char pad[0x3f]; int m_n50; void Begin(); };
/** @sly2-annot addr=0x1C8D20
 * Purpose: reset progress
 * Info: proposed name Begin__5CProg (EXISTING); class CProg; TU prog
 * Notes: agent/verifier proposes CProg_Begin for this existing name (see existing_name_changes.csv)
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/hud-ui.md#prog-0x1c8cc0-0x1c9940
 * @end-sly2-annot */
void CProg::Begin() { m_cRetry = 0; m_n50 = 0; m_nMax = 0; m_nTarget = 0; m_nRemain = 0; m_fActive = 0; }
