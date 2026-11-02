struct RGBA { unsigned char r, g, b, a; };
class CProg {
public:
    int m_cRetry; int m_nRemain; int m_nTarget; int m_nMax; char m_fActive; char pad[0x3f]; int m_n50;
    RGBA m_rgba0, m_rgba1, m_rgba2, m_rgba3;
    CProg(RGBA *p0, RGBA *p1, RGBA *p2, RGBA *p3);
    void Begin() { m_cRetry = 0; m_n50 = 0; m_nMax = 0; m_nTarget = 0; m_nRemain = 0; m_fActive = 0; }
};
/** @sly2-annot addr=0x1C8CC0
 * Purpose: CProg ctor (4 colors)
 * Info: proposed name __5CProgP4RGBAN31 (EXISTING); class CProg; TU prog
 * Notes: agent/verifier proposes CProg_ctor for this existing name (see existing_name_changes.csv)
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/engine/game-progression.md#7-source-files,
 *   docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen, docs/engine/hud-ui.md#4-source-files
 * @end-sly2-annot */
CProg::CProg(RGBA *p0, RGBA *p1, RGBA *p2, RGBA *p3)
{
    m_rgba0 = *p0; m_rgba1 = *p1; m_rgba2 = *p2; m_rgba3 = *p3;
    Begin();
}
