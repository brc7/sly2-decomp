struct T { char pad[0xCC]; int a[1]; };
extern struct G D_003DD490;
int PchzFindSwLangstr__FP2SWi(struct G *g, int x);
void func_0021ED58(void *p, int v);
/** @sly2-annot addr=0x211348
 * Purpose: STRINGGEN ids 0x2E5-0x2EE: langstr(owner WIDGET->aid[i] at +0xCC)
 * Info: proposed name BuildWidgetArgText (T1); TU hud
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void BuildWidgetArgText(void *p, int i, struct T *t)
{
    func_0021ED58(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, t->a[i]));
}
