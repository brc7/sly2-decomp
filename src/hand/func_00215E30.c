void func_00215CA8(void *p, int a);
int func_00215D40(void *p);
/** @sly2-annot addr=0x215E30
 * Purpose: count characters
 * Info: proposed name CRichText_Cch (T1); class CRichText; TU hud
 * Notes: names_v2 B proposed FIncludeBlotForPeg__FP4BLOTT0 (0.856)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_00215E30(void *p)
{
    int n = 0;
    func_00215CA8(p, 0);
    while (func_00215D40(p))
        n++;
    return n;
}
