struct S { char p0[0x48]; int c; char p1[0x118-0x4C]; int i; };
void func_001DA9E8(struct S *p);
static inline int Clamp(int n, int lo, int hi) {
    if (n < lo) return lo;
    if (n > hi) return hi;
    return n;
}
/** @sly2-annot addr=0x1DCE90
 * Purpose: Select row i (clamped)
 * Info: proposed name SelectJobListItem (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SelectJobListItem(struct S *p, int i) {
    int c = p->c;
    int n;
    if (c == 0) n = -1;
    else n = Clamp(i, 0, c - 1);
    p->i = n;
    func_001DA9E8(p);
}
