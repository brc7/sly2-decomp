struct CProg { int m_cRetry; int m_nRemain; int m_nTarget; int m_nMax; char m_fActive; char pad[0x3f]; int m_n50; };
/** @sly2-annot addr=0x1C8DB8
 * Purpose: clear progress state (same stores as Begin)
 * Info: proposed name CProg_End (T1); class CProg; TU prog
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5+verdicts_A2
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#prog-0x1c8cc0-0x1c9940
 * @end-sly2-annot */
void End__5CProg(struct CProg *p) { p->m_cRetry = 0; p->m_n50 = 0; p->m_nMax = 0; p->m_nTarget = 0; p->m_nRemain = 0; p->m_fActive = 0; }
