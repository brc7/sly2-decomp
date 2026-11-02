extern struct CLOCK { float t; } D_002DDED0;
struct S { char pad[0x14]; float g14; };
void func_001CF518(struct S *p);
/** @sly2-annot addr=0x1D00D0
 * Purpose: MATCH Render: wraps RenderRip
 * Info: proposed name RenderMatch (T1); TU rip
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D00D0(struct S *p) {
    if (D_002DDED0.t < p->g14) return;
    func_001CF518(p);
}
