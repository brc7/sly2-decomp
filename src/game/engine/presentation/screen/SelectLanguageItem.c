struct S { char p0[8]; void *p8; };
extern char D_003DD490[];
void SetLanguage(void *p);
void WipeToWorldWarp(void *g, int a, int b, int c, int d, int e);
/** @sly2-annot addr=0x1DD8E0
 * Purpose: Language menu item callback: SetLanguage(item) and warp to world 0x189
 * Info: proposed name SelectLanguageItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectLanguageItem(struct S *p) {
    SetLanguage(p->p8);
    WipeToWorldWarp(D_003DD490, 0, 0x189, -1, 1, -1);
}
