struct G { char pad[0x40]; };
struct S { char pad[0x14]; struct G *ag; };
int IglyffFromCh__8CFontBrxc(struct S *p, unsigned char ch);
/** @sly2-annot addr=0x216B40
 * Purpose: GLYFF* = &glyphs[i] for i = 0x215A50(ch)
 * Info: proposed name CFontBrx_PglyffFromCh (T1); TU hud
 * Notes: names_v2 C proposed ShowTotalsQMARK (0.167) | agent qualifier: pointer form | COLLISION:
 *   PglyffFromCh already used (same name proposed for 0x15F8A8); renamed to PglyffFromCh_216B40 | superseded:
 *   PglyffFromCh_216B40 (arbitration) | arbitration knock-on rename: CONFIRMED static: char arg (andi 0xff),
 *   returns GLYFF* or NULL = Sly1 CFontBrx::PglyffFromCh contract. | arbitration mangling: compile a
 *   declaration in the scratch build and compare before committing
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; arbitration; static naming pass;
 *   sources: r7c_hud+arbitration
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
struct G *func_00216B40(struct S *p, unsigned char ch)
{
    int i = IglyffFromCh__8CFontBrxc(p, ch);
    return i ? &p->ag[i] : 0;
}
