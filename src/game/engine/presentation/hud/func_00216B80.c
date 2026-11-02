struct S { char pad[0x1C]; float *ag; };
int IglyffFromCh__8CFontBrxc(struct S *p, unsigned char ch);
/** @sly2-annot addr=0x216B80
 * Purpose: width of glyph for ch (widths[i-1])
 * Info: proposed name CFontBrx_DxFromCh_216B80 (T3); class CFontBrx; TU hud
 * Notes: names_v2 C proposed HideTotalsQMARK (0.638) | COLLISION: CFontBrx_DxFromCh already used at 0x15F0F0
 *   (existing symbol); renamed CFontBrx_DxFromCh_216B80
 * Confidence: low (draft) [T3 0.65] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
float func_00216B80(struct S *p, unsigned char ch)
{
    int i = IglyffFromCh__8CFontBrxc(p, ch);
    return i ? p->ag[i - 1] : 0.0f;
}
