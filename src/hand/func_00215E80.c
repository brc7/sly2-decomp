struct S { int a; char *pch; };
void func_00215CA8(struct S *p, int a);
int func_00215D40(struct S *p);
/** @sly2-annot addr=0x215E80
 * Purpose: truncate after n chars
 * Info: proposed name CRichText_Trim (T1); class CRichText; TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00215E80(struct S *p, int n)
{
    func_00215CA8(p, 0);
    while (func_00215D40(p) && n != 0)
        n--;
    *p->pch = 0;
}
