extern int D_003DA180;
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void strcpy(char *p, void *q);
/** @sly2-annot addr=0x1DCA30
 * Purpose: Text callback 0x436: popup prompt text by popup mode (-1/0/1 -> ids 0xDE/0xDF/0xE0)
 * Info: proposed name BuildThiefnetPromptText (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DCA30(char *pch) {
    int id;
    switch (D_003DA180) {
    case -1: id = 0xDE; break;
    case 0: id = 0xDF; break;
    case 1: id = 0xE0; break;
    default: *pch = 0; return;
    }
    strcpy(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, id));
}
