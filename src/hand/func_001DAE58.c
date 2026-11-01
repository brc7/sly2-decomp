struct E { int a[4]; int n; int b[3]; };
extern struct E D_002BCD00[];
int func_00210B28(void);
/** @sly2-annot addr=0x1DAE58
 * Purpose: Item i of table D_002BCD00 is available when progress (0x210B28) >= its threshold
 * Info: proposed name FScreenItemUnlocked (T2); TU screen
 * Notes: runtime7b V03: returns NEpisodeBest() >= gadget[i].x10 (unlock episode); S71 list matches exactly
 * Confidence: low (draft) [T2 0.35] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int func_001DAE58(int i) {
    struct E *pe = &D_002BCD00[i];
    if (pe->n == -1) return 0;
    return func_00210B28() >= pe->n;
}
